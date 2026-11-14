/*
This file is part of LibreGram Desktop.
For license and upstream attribution, see LICENSE and LEGAL.
*/
#include "core/libregram_ws_proxy.h"

#include "core/application.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QFileInfo>
#include <QtCore/QUrlQuery>

namespace Core {
namespace {

constexpr auto kStartTimeout = 10000;
constexpr auto kStopTimeout = 2000;
constexpr auto kReadyLimit = 512;

} // namespace

LibreGramWsProxy::LibreGramWsProxy(Application &app)
: _app(app) {
	_deadline.setSingleShot(true);
	_process.setStandardErrorFile(QProcess::nullDevice());
	QObject::connect(&_process, &QProcess::readyReadStandardOutput,
		&_process, [=, this] { readReady(); });
	QObject::connect(&_process, &QProcess::errorOccurred,
		&_process, [=, this](QProcess::ProcessError error) {
			if (error == QProcess::FailedToStart) {
				fail();
			}
		});
	QObject::connect(&_process, &QProcess::finished, &_process, [=, this] {
		_deadline.stop();
		_app.setTemporaryWsProxy({});
		if (_state.current() == State::Stopping) {
			_state = State::Off;
		} else {
			_state = State::Failed;
		}
	});
	QObject::connect(&_deadline, &QTimer::timeout, &_process, [=, this] {
		if (_state.current() == State::Stopping) {
			_process.kill();
		} else {
			fail();
		}
	});
}

LibreGramWsProxy::~LibreGramWsProxy() {
	_process.disconnect();
	if (_process.state() != QProcess::NotRunning) {
		_process.closeWriteChannel();
		if (!_process.waitForFinished(kStopTimeout)) {
			_process.kill();
			_process.waitForFinished(kStopTimeout);
		}
	}
}

void LibreGramWsProxy::start(bool tcpFallback) {
	if (_process.state() != QProcess::NotRunning) {
		return;
	}
	const auto program = QCoreApplication::applicationDirPath()
#ifdef Q_OS_WIN
		+ u"/libregram-ws-proxy.exe"_q;
#else
		+ u"/libregram-ws-proxy"_q;
#endif
	if (!QFileInfo(program).isExecutable()) {
		_state = State::Unavailable;
		return;
	}
	auto env = QProcessEnvironment::systemEnvironment();
	for (const auto &key : env.keys()) {
		if (key.startsWith(u"TG_"_q, Qt::CaseInsensitive)) {
			env.remove(key);
		}
	}
	_ready.clear();
	_process.setProcessEnvironment(env);
	_state = State::Starting;
	_deadline.start(kStartTimeout);
	_process.start(program, {
		u"--host"_q, u"127.0.0.1"_q,
		u"--link-ip"_q, u"127.0.0.1"_q,
		u"--port"_q, u"0"_q,
		u"--pool-size"_q, u"2"_q,
		u"--no-outbound-proxy"_q,
		u"--quiet"_q,
		u"--pinned-upstream"_q,
		tcpFallback ? u"ws,tcp"_q : u"ws"_q,
	});
}

void LibreGramWsProxy::stop() {
	if (_process.state() == QProcess::NotRunning
		|| _state.current() == State::Stopping) {
		return;
	}
	_state = State::Stopping;
	_app.setTemporaryWsProxy({});
	_process.closeWriteChannel();
	_deadline.start(kStopTimeout);
}

rpl::producer<LibreGramWsProxy::State> LibreGramWsProxy::state() const {
	return _state.value();
}

void LibreGramWsProxy::readReady() {
	const auto incoming = _process.readAllStandardOutput();
	if (_state.current() != State::Starting) {
		return;
	}
	_ready += incoming;
	if (_ready.size() > kReadyLimit) {
		fail();
		return;
	}
	if (!_ready.endsWith('\n')) {
		return;
	}
	const auto link = QUrl::fromEncoded(_ready.trimmed(), QUrl::StrictMode);
	_ready.clear();
	const auto fields = QUrlQuery(link);
	const auto host = fields.queryItemValue(u"server"_q);
	const auto port = fields.queryItemValue(u"port"_q).toUInt();
	const auto secret = fields.queryItemValue(u"secret"_q);
	const auto proxy = MTP::ProxyData{
		.type = MTP::ProxyData::Type::Mtproto,
		.host = host,
		.port = port,
		.password = secret,
	};
	if (!link.isValid()
		|| link.scheme() != u"tg"_q
		|| link.host() != u"proxy"_q
		|| host != u"127.0.0.1"_q
		|| !port
		|| port > 65535
		|| !proxy.valid()) {
		fail();
		return;
	}
	_deadline.stop();
	_app.setTemporaryWsProxy(proxy);
	_state = State::Listening;
}

void LibreGramWsProxy::fail() {
	_deadline.stop();
	_process.kill();
	_ready.clear();
	_app.setTemporaryWsProxy({});
	_state = State::Failed;
}

} // namespace Core
