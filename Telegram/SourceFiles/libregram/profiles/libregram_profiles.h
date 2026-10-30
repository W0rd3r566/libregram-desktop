/*
This file is part of LibreGram Desktop.
For license and copyright information, see the LICENSE file.
*/
#pragma once

#include <QtCore/QString>
#include <QtCore/QStringList>

#include <vector>

namespace LibreGram::Profiles {

inline constexpr int kMaxSavedProfiles = 500;

struct Profile {
	QString id;
	QString name;
	QString createdAt;
	QString lastLaunchedAt;
	bool favorite = false;
	QString group;
	QStringList tags;
};

[[nodiscard]] std::vector<Profile> List(QString *error = nullptr);

[[nodiscard]] bool Create(
	const QString &name,
	Profile *created = nullptr,
	QString *error = nullptr);

[[nodiscard]] bool Update(
	const QString &id,
	const QString &name,
	const QString &group,
	const QStringList &tags,
	QString *error = nullptr);

[[nodiscard]] bool Launch(
	const QString &id,
	QString *error = nullptr);

[[nodiscard]] bool SetFavorite(
	const QString &id,
	bool favorite,
	QString *error = nullptr);

} // namespace LibreGram::Profiles
