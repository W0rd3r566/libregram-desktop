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

	void start(bool tcpFallback);
	void stop();
	[[nodiscard]] rpl::producer<State> state() const;

private:
	void readReady();
	void fail();

	Application &_app;
	QProcess _process;
	QTimer _deadline;
	QByteArray _ready;
	rpl::variable<State> _state = State::Off;

};

} // namespace Core
