/*
This file is part of LibreGram Desktop.
For license and upstream attribution, see LICENSE and LEGAL.
*/
#pragma once

#include "settings/settings_type.h"

namespace Ui {
class GenericBox;
} // namespace Ui

namespace Settings {

[[nodiscard]] Type LibreGramId();
void WsProxyBox(not_null<Ui::GenericBox*> box);

} // namespace Settings
