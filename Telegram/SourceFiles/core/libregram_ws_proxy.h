/*
This file is part of LibreGram Desktop.
For license and upstream attribution, see LICENSE and LEGAL.
*/
#pragma once

#include <QtCore/QProcess>
#include <QtCore/QTimer>

namespace Core {

class Application;

class LibreGramWsProxy final {
public:
	enum class State {
		Off,
		Starting,
		Listening,
		Stopping,
		Unavailable,
		Failed,
	};

	explicit LibreGramWsProxy(Application &app);
	~LibreGramWsProxy();

	void startOnLaunch();
	void start();
	void stop();
	void restart();
	void toggle();

	void setAutoStart(bool enabled);
	[[nodiscard]] rpl::producer<bool> autoStartValue() const;
	void setTcpFallback(bool enabled);
	[[nodiscard]] rpl::producer<bool> tcpFallbackValue() const;
	[[nodiscard]] rpl::producer<State> state() const;
	[[nodiscard]] static bool IsActive(State state);

private:
	void stopProcess();
	void readReady();
	void fail();

	Application &_app;
	QProcess _process;
	QTimer _deadline;
	QByteArray _ready;
	rpl::variable<bool> _autoStart;
	rpl::variable<bool> _tcpFallback;
	rpl::variable<State> _state = State::Off;
	bool _startupChecked = false;
	bool _restartPending = false;

};

} // namespace Core
