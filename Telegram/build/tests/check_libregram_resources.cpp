#include <QCoreApplication>
#include <QFile>
#include <QImage>
#include <QResource>

#include <iostream>

int main(int argc, char **argv) {
	auto app = QCoreApplication(argc, argv);
	if (argc != 2) {
		return 5;
	}
	if (!QResource::registerResource(argv[1])) {
		return 1;
	}
	const auto logo = QImage(":/gui/art/libregram.png");
	if (logo.isNull() || logo.width() != 512 || logo.height() != 512) {
		return 2;
	}
	auto theme = QFile(":/gui/libregram-graphite.tdesktop-theme");
	if (!theme.open(QIODevice::ReadOnly)
		|| !theme.readAll().contains("windowBg: #20242b;")) {
		return 3;
	}
	for (const auto size : { 16, 22, 32, 48, 64, 256 }) {
		if (logo.scaled(
				size,
				size,
				Qt::KeepAspectRatio,
				Qt::SmoothTransformation).isNull()) {
			return 4;
		}
	}
	std::cout << "Qt resource load, logo decode/scaling, theme load: PASS\n";
}
