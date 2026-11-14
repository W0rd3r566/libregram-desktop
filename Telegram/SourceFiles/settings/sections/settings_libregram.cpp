/*
This file is part of LibreGram Desktop.
For license and upstream attribution, see LICENSE and LEGAL.
*/
#include "settings/sections/settings_libregram.h"

#include "core/application.h"
#include "core/libregram_ws_proxy.h"
#include "kotato/kotato_settings.h"
#include "kotato/kotato_settings_menu.h"
#include "lang/lang_keys.h"
#include "main/main_session.h"
#include "main/main_session_settings.h"
#include "settings/sections/settings_chat.h"
#include "settings/sections/settings_main.h"
#include "settings/sections/settings_notifications.h"
#include "settings/settings_builder.h"
#include "settings/settings_common_session.h"
#include "settings/settings_power_saving.h"
#include "ui/layers/generic_box.h"
#include "ui/power_saving.h"
#include "ui/vertical_list.h"
#include "ui/widgets/buttons.h"
#include "ui/widgets/labels.h"
#include "ui/wrap/vertical_layout.h"
#include "window/themes/window_theme.h"
#include "window/window_session_controller.h"

#include "styles/style_layers.h"
#include "styles/style_menu_icons.h"
#include "styles/style_settings.h"

namespace Settings {
namespace {

using namespace Builder;

class LibreGram final : public Section<LibreGram> {
public:
	LibreGram(QWidget *parent, not_null<Window::SessionController*> controller);

	[[nodiscard]] rpl::producer<QString> title() override;

};

void AddToggle(
		SectionBuilder &builder,
		SectionBuilder::ButtonArgs args,
		Fn<bool()> current,
		Fn<void(bool)> save) {
	const auto button = builder.addButton(std::move(args));
	if (button) {
		button->toggledChanges(
		) | rpl::filter([=](bool value) {
			return value != current();
		}) | rpl::on_next(std::move(save), button->lifetime());
	}
}

void BuildAppearance(SectionBuilder &builder) {
	builder.addSkip();
	builder.addSubsectionTitle(tr::lng_libregram_appearance());
	builder.addButton({
		.id = u"libregram/graphite"_q,
		.title = tr::lng_libregram_graphite(),
		.icon = { &st::menuIconPalette },
		.onClick = [controller = builder.controller()] {
			if (!Window::Theme::Apply(
					u":/gui/libregram-graphite.tdesktop-theme"_q)) {
				controller->showToast(tr::lng_libregram_theme_error(tr::now));
			}
		},
		.keywords = { u"theme"_q, u"dark"_q, u"graphite"_q },
	});
	builder.addDividerText(tr::lng_libregram_graphite_about());
	builder.addSectionButton({
		.title = tr::lng_libregram_customize(),
		.targetSection = ChatId(),
		.icon = { &st::menuIconChatBubble },
		.keywords = { u"libretheme"_q, u"appearance"_q },
	});

	const auto compact = [] {
		return ::Kotato::JsonSettings::GetInt(u"chat_list_lines"_q) == 1;
	};
	AddToggle(builder, {
		.id = u"libregram/compact"_q,
		.title = tr::lng_libregram_compact(),
		.st = &st::settingsButtonNoIcon,
		.toggled = rpl::single(compact()) | rpl::then(
			::Kotato::JsonSettings::Events(u"chat_list_lines"_q)
			| rpl::map(compact)),
		.keywords = { u"compact"_q, u"chat list"_q },
	}, compact, [](bool enabled) {
		::Kotato::JsonSettings::Set(u"chat_list_lines"_q, enabled ? 1 : 2);
		::Kotato::JsonSettings::Write();
	});

	const auto reducedMotion = [] {
		return bool(PowerSaving::Current() & PowerSaving::kAnimations);
	};
	AddToggle(builder, {
		.id = u"libregram/reduced-motion"_q,
		.title = tr::lng_libregram_reduced_motion(),
		.st = &st::settingsButtonNoIcon,
		.toggled = rpl::single(reducedMotion()) | rpl::then(
			PowerSaving::Changes() | rpl::map(reducedMotion)),
		.keywords = { u"animations"_q, u"reduce motion"_q },
	}, reducedMotion, [](bool enabled) {
		const auto flags = PowerSaving::Current();
		PowerSaving::Set(enabled
			? (flags | PowerSaving::kAnimations)
			: (flags & ~PowerSaving::Flags(PowerSaving::kAnimations)));
		Core::App().saveSettingsDelayed();
	});
	builder.addDividerText(tr::lng_libregram_reduced_motion_about());

	builder.addButton({
		.id = u"libregram/motion"_q,
		.title = tr::lng_libregram_motion(),
		.icon = { &st::menuIconPowerUsage },
		.onClick = [controller = builder.controller()] {
			controller->show(Box(PowerSavingBox, PowerSaving::Flags()));
		},
		.keywords = { u"animations"_q, u"motion"_q, u"battery"_q },
	});
	builder.addSkip();
}

const auto kMeta = BuildHelper({
	.id = LibreGram::Id(),
	.parentId = MainId(),
	.title = &tr::lng_libregram_settings,
	.icon = &st::menuIconSettings,
}, [](SectionBuilder &builder) {
	BuildAppearance(builder);
	builder.addDivider();
	builder.addSkip();
	builder.addSubsectionTitle(tr::lng_libregram_client());
	builder.addButton({
		.id = u"libregram/ws-proxy"_q,
		.title = tr::lng_libregram_ws_proxy(),
		.icon = { &st::menuIconManage },
		.onClick = [controller = builder.controller()] {
			controller->show(Box(WsProxyBox));
		},
		.keywords = { u"websocket"_q, u"proxy"_q },
	});
	const auto session = builder.session();
	AddToggle(builder, {
		.id = u"libregram/hide-phone"_q,
		.title = tr::lng_libregram_hide_phone(),
		.st = &st::settingsButtonNoIcon,
		.toggled = session->settings().phoneNumberHiddenValue(),
		.keywords = { u"phone"_q, u"privacy"_q },
	}, [=] {
		return session->settings().phoneNumberHidden();
	}, [=](bool hidden) {
		session->settings().setPhoneNumberHidden(hidden);
		session->saveSettingsDelayed();
	});
	builder.addSectionButton({
		.title = tr::lng_settings_section_notify(),
		.targetSection = NotificationsId(),
		.icon = { &st::menuIconNotifications },
		.keywords = { u"alerts"_q, u"notifications"_q },
	});
	builder.addSectionButton({
		.title = tr::lng_libregram_upstream_settings(),
		.targetSection = Kotato::Id(),
		.icon = { &st::menuIconManage },
		.keywords = { u"kotatogram"_q, u"fonts"_q, u"stickers"_q },
	});
	builder.addSkip();
	builder.addDividerText(tr::lng_libregram_about());
});

LibreGram::LibreGram(
	QWidget *parent,
	not_null<Window::SessionController*> controller)
: Section(parent, controller) {
	const auto content = Ui::CreateChild<Ui::VerticalLayout>(this);
	build(content, kMeta.build);
	Ui::ResizeFitChild(this, content);
}

rpl::producer<QString> LibreGram::title() {
	return tr::lng_libregram_settings();
}

} // namespace

void WsProxyBox(not_null<Ui::GenericBox*> box) {
	using State = Core::LibreGramWsProxy::State;
	box->setTitle(tr::lng_libregram_ws_proxy());
	box->setWidth(st::boxWideWidth);
	const auto content = box->verticalLayout();
	Ui::AddDividerText(content, tr::lng_libregram_ws_about());
	const auto proxy = &Core::App().libreGramWsProxy();
	const auto autostart = AddButtonWithIcon(
		content,
		tr::lng_libregram_ws_auto_start(),
		st::settingsButtonNoIcon)->toggleOn(proxy->autoStartValue());
	autostart->toggledChanges() | rpl::on_next([=](bool enabled) {
		proxy->setAutoStart(enabled);
	}, autostart->lifetime());
	const auto fallback = AddButtonWithIcon(
		content,
		tr::lng_libregram_ws_tcp_fallback(),
		st::settingsButtonNoIcon)->toggleOn(proxy->tcpFallbackValue());
	fallback->toggledChanges() | rpl::on_next([=](bool enabled) {
		proxy->setTcpFallback(enabled);
	}, fallback->lifetime());
	const auto status = Ui::AddDividerText(
		content,
		rpl::single(QString()));
	const auto start = box->addButton(tr::lng_libregram_ws_start(), [=] {
		proxy->start();
	});
	const auto stop = box->addButton(tr::lng_libregram_ws_stop(), [=] {
		proxy->stop();
	});
	const auto restart = box->addLeftButton(tr::lng_restart_button(), [=] {
		proxy->restart();
	});
	box->addButton(tr::lng_close(), [=] { box->closeBox(); });
	proxy->state() | rpl::on_next([=](State state) {
		const auto message = [&] {
			switch (state) {
			case State::Off: return tr::lng_libregram_ws_off(tr::now);
			case State::Starting: return tr::lng_libregram_ws_starting(tr::now);
			case State::Listening: return tr::lng_libregram_ws_listening(tr::now);
			case State::Stopping: return tr::lng_libregram_ws_stopping(tr::now);
			case State::Unavailable: return tr::lng_libregram_ws_unavailable(tr::now);
			case State::Failed: return tr::lng_libregram_ws_failed(tr::now);
			}
			Unexpected("WS Proxy state");
		}();
		status->setText(message);
		const auto active = Core::LibreGramWsProxy::IsActive(state);
		const auto canStart = !active && (state != State::Stopping);
		start->setDisabled(!canStart);
		fallback->setDisabled(!canStart);
		stop->setDisabled(!active);
		restart->setDisabled(state != State::Listening);
	}, box->lifetime());
}

Type LibreGramId() {
	return LibreGram::Id();
}

} // namespace Settings
