/*
This file is part of LibreGram Desktop.
For license and copyright information, see the LICENSE file.
*/
#pragma once

#include <QtCore/QString>

#include <vector>

namespace LibreGram::Profiles {

inline constexpr int kMaxSavedProfiles = 500;

struct Profile {
	QString id;
	QString name;
	QString createdAt;
	QString lastLaunchedAt;
};

[[nodiscard]] std::vector<Profile> List(QString *error = nullptr);

[[nodiscard]] bool Create(
	const QString &name,
	Profile *created = nullptr,
	QString *error = nullptr);

[[nodiscard]] bool Launch(
	const QString &id,
	QString *error = nullptr);

} // namespace LibreGram::Profiles
