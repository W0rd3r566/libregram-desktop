/*
This file is part of LibreGram Desktop.
For license and copyright information, see the LICENSE file.
*/
#include "libregram/profiles/libregram_profiles.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileDevice>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QProcess>
#include <QtCore/QSaveFile>
#include <QtCore/QSet>
#include <QtCore/QStandardPaths>
#include <QtCore/QUuid>

#include <algorithm>

namespace LibreGram::Profiles {
namespace {

constexpr auto kRegistrySchema = 1;
constexpr auto kProfileNameLimit = 64;

QString DataRoot() {
	const auto base = QStandardPaths::writableLocation(
		QStandardPaths::GenericDataLocation);
	return base.isEmpty()
		? QString()
		: QDir(base).filePath(u"LibreGram"_q);
}

QString ProfilesDirectory() {
	return QDir(DataRoot()).filePath(u"profiles"_q);
}

QString RegistryPath() {
	return QDir(DataRoot()).filePath(u"profiles.json"_q);
}

QString ProfileDirectory(const QString &id) {
	return QDir(ProfilesDirectory()).filePath(id);
}

bool IsValidId(const QString &id) {
	const auto uuid = QUuid(id);
	return !uuid.isNull()
		&& (uuid.toString(QUuid::WithoutBraces).toLower() == id.toLower());
}

bool SetError(QString *error, const QString &message) {
	if (error) {
		*error = message;
	}
	return false;
}

bool ReadRegistry(std::vector<Profile> *profiles, QString *error) {
	profiles->clear();

	const auto path = RegistryPath();
	if (path.isEmpty()) {
		return SetError(error, u"LibreGram data directory is unavailable."_q);
	}

	QFile file(path);
	if (!file.exists()) {
		return true;
	}
	if (!file.open(QIODevice::ReadOnly)) {
		return SetError(error, u"Could not read the profiles registry."_q);
	}

	auto parseError = QJsonParseError();
	const auto document = QJsonDocument::fromJson(file.readAll(), &parseError);
	if (parseError.error != QJsonParseError::NoError
		|| !document.isObject()) {
		return SetError(error, u"The profiles registry is damaged. It was left untouched."_q);
	}

	const auto root = document.object();
	if (root.value(u"schema"_q).toInt() != kRegistrySchema
		|| !root.value(u"profiles"_q).isArray()) {
		return SetError(error, u"The profiles registry uses an unsupported format."_q);
	}

	const auto array = root.value(u"profiles"_q).toArray();
	if (array.size() > kMaxSavedProfiles) {
		return SetError(error, u"The profiles registry exceeds the supported limit."_q);
	}

	auto seen = QSet<QString>();
	profiles->reserve(size_t(array.size()));
	for (const auto &value : array) {
		if (!value.isObject()) {
			return SetError(error, u"The profiles registry contains an invalid entry."_q);
		}
		const auto object = value.toObject();
		Profile profile{
			.id = object.value(u"id"_q).toString(),
			.name = object.value(u"name"_q).toString().trimmed(),
			.createdAt = object.value(u"createdAt"_q).toString(),
			.lastLaunchedAt = object.value(u"lastLaunchedAt"_q).toString(),
		};
		if (!IsValidId(profile.id)
			|| profile.name.isEmpty()
			|| profile.name.size() > kProfileNameLimit
			|| seen.contains(profile.id.toLower())) {
			return SetError(error, u"The profiles registry contains invalid profile metadata."_q);
		}
		seen.insert(profile.id.toLower());
		profiles->push_back(std::move(profile));
	}

	return true;
}

bool WriteRegistry(
		const std::vector<Profile> &profiles,
		QString *error) {
	const auto root = DataRoot();
	if (root.isEmpty() || !QDir().mkpath(root)) {
		return SetError(error, u"Could not create the LibreGram data directory."_q);
	}

	auto array = QJsonArray();
	for (const auto &profile : profiles) {
		auto object = QJsonObject();
		object.insert(u"id"_q, profile.id);
		object.insert(u"name"_q, profile.name);
		object.insert(u"createdAt"_q, profile.createdAt);
		object.insert(u"lastLaunchedAt"_q, profile.lastLaunchedAt);
		array.append(object);
	}

	auto document = QJsonObject();
	document.insert(u"schema"_q, kRegistrySchema);
	document.insert(u"profiles"_q, array);

	QSaveFile file(RegistryPath());
	if (!file.open(QIODevice::WriteOnly)) {
		return SetError(error, u"Could not write the profiles registry."_q);
	}
	const auto bytes = QJsonDocument(document).toJson(QJsonDocument::Indented);
	if (file.write(bytes) != bytes.size() || !file.commit()) {
		return SetError(error, u"Could not safely save the profiles registry."_q);
	}
	QFile::setPermissions(
		RegistryPath(),
		QFileDevice::ReadOwner | QFileDevice::WriteOwner);
	return true;
}

bool ValidateName(const QString &value, QString *name, QString *error) {
	*name = value.trimmed();
	if (name->isEmpty()) {
		return SetError(error, u"Enter a name for the profile."_q);
	}
	if (name->size() > kProfileNameLimit) {
		return SetError(error, u"Profile names can contain up to 64 characters."_q);
	}
	for (const auto ch : *name) {
		if (ch.isControl()) {
			return SetError(error, u"Profile names cannot contain control characters."_q);
		}
	}
	return true;
}

} // namespace

std::vector<Profile> List(QString *error) {
	auto result = std::vector<Profile>();
	if (!ReadRegistry(&result, error)) {
		return {};
	}
	std::sort(result.begin(), result.end(), [](const Profile &a, const Profile &b) {
		return a.name.toCaseFolded() < b.name.toCaseFolded();
	});
	return result;
}

bool Create(const QString &value, Profile *created, QString *error) {
	auto name = QString();
	if (!ValidateName(value, &name, error)) {
		return false;
	}

	auto profiles = std::vector<Profile>();
	if (!ReadRegistry(&profiles, error)) {
		return false;
	}
	if (profiles.size() >= size_t(kMaxSavedProfiles)) {
		return SetError(error, u"You have reached the limit of 500 saved profiles."_q);
	}

	const auto root = DataRoot();
	if (root.isEmpty()
		|| !QDir().mkpath(root)
		|| !QDir().mkpath(ProfilesDirectory())) {
		return SetError(error, u"Could not create the profiles directory."_q);
	}

	const auto id = QUuid::createUuid().toString(QUuid::WithoutBraces).toLower();
	const auto path = ProfileDirectory(id);
	if (!QDir().mkpath(path)) {
		return SetError(error, u"Could not create the profile data directory."_q);
	}
	QFile::setPermissions(
		path,
		QFileDevice::ReadOwner
			| QFileDevice::WriteOwner
			| QFileDevice::ExeOwner);

	const auto profile = Profile{
		.id = id,
		.name = name,
		.createdAt = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs),
		.lastLaunchedAt = QString(),
	};
	profiles.push_back(profile);
	if (!WriteRegistry(profiles, error)) {
		QDir(path).removeRecursively();
		return false;
	}
	if (created) {
		*created = profile;
	}
	return true;
}

bool Launch(const QString &id, QString *error) {
	if (!IsValidId(id)) {
		return SetError(error, u"Invalid profile identifier."_q);
	}

	auto profiles = std::vector<Profile>();
	if (!ReadRegistry(&profiles, error)) {
		return false;
	}
	const auto i = std::find_if(
		profiles.begin(),
		profiles.end(),
		[&](const Profile &profile) {
			return profile.id.compare(id, Qt::CaseInsensitive) == 0;
		});
	if (i == profiles.end()) {
		return SetError(error, u"Profile not found."_q);
	}

	const auto path = ProfileDirectory(i->id);
	if (!QDir().mkpath(path)) {
		return SetError(error, u"Could not open the profile data directory."_q);
	}

	const auto previousTimestamp = i->lastLaunchedAt;
	i->lastLaunchedAt = QDateTime::currentDateTimeUtc().toString(Qt::ISODateWithMs);
	if (!WriteRegistry(profiles, error)) {
		return false;
	}

	const auto program = QCoreApplication::applicationFilePath();
	if (program.isEmpty()
		|| !QProcess::startDetached(
			program,
			{ u"-workdir"_q, QDir(path).absolutePath() },
			QCoreApplication::applicationDirPath())) {
		i->lastLaunchedAt = previousTimestamp;
		WriteRegistry(profiles, nullptr);
		return SetError(error, u"Could not start LibreGram with this profile."_q);
	}
	return true;
}

} // namespace LibreGram::Profiles
