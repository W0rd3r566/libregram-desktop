# Сборка LibreGram Desktop в Linux

Эта инструкция относится к ветке `libregram/dev`. Проект использует C++/Qt и собирается штатной системой CMake. Внешний вид приложения не требует отдельного фронтенда или веб-сборки.

## 1. Требования

Для рекомендуемого контейнерного способа нужны:

- Git с поддержкой submodules;
- Docker Engine и запущенный Docker daemon;
- Python 3 и Poetry;
- собственные Telegram API credentials: `api_id` и `api_hash`.

Получение credentials описано в [api_credentials.md](api_credentials.md). Не добавляйте реальные значения в Git, исходники или публичные логи.

## 2. Получение исходников

Клонируйте именно репозиторий LibreGram с подмодулями:

```bash
git clone --recursive https://github.com/W0rd3r566/libregram-desktop.git
cd libregram-desktop
```

Проверьте, что подмодули загружены:

```bash
git submodule status --recursive
```

Если репозиторий был клонирован без `--recursive`, выполните:

```bash
git submodule update --init --recursive
```

## 3. Подготовка контейнера сборки

Из корня репозитория запустите:

```bash
./Telegram/build/prepare/linux.sh
```

Скрипт устанавливает Python-зависимости окружения через Poetry и собирает Docker-образ `tdesktop:centos_env`. Это унаследованное техническое имя образа сборочной среды, а не название приложения.

## 4. Сборка Release

Передайте свои credentials в CMake во время сборки:

```bash
docker run --rm -it \
  -u "$(id -u)" \
  -v "$PWD:/usr/src/tdesktop" \
  -e CONFIG=Release \
  tdesktop:centos_env \
  /usr/src/tdesktop/Telegram/build/docker/centos_env/build.sh \
  -D TDESKTOP_API_ID=YOUR_API_ID \
  -D TDESKTOP_API_HASH=YOUR_API_HASH
```

Замените оба заполнителя собственными значениями. Не публикуйте команду с настоящим `api_hash` в логах, скриншотах или публичных issue.

Для Debug-сборки замените `CONFIG=Release` на `CONFIG=Debug`.

## 5. Где находится результат

CMake generator и конфигурация определяют точный каталог результата. Найти исполняемый файл можно командой:

```bash
find out -type f -name LibreGram -executable -print
```

Запускайте найденный бинарный файл из терминала, чтобы видеть диагностические сообщения.

## 6. Особенности LibreGram

- Исполняемый файл называется `LibreGram`.
- Для сборки нужны credentials собственного приложения Telegram. Они не встроены в репозиторий.
- Автоматическое обновление пока отключено: у LibreGram ещё нет собственного проверенного канала обновлений с подписанными релизами.
- Внутренние имена унаследованных CMake-целей и сборочного контейнера могут оставаться прежними. Это не меняет имя приложения.
- Существующие окна, компоновка и элементы Qt сохраняются. Новые возможности добавляются поверх текущего интерфейса.

## 7. Частые проблемы

**Docker daemon недоступен.** Убедитесь, что Docker Engine установлен и запущен, а текущий пользователь имеет право обращаться к Docker.

**Не найдены библиотеки или подмодули.** Повторите `git submodule update --init --recursive`, затем снова запустите подготовку контейнера.

**Ошибка API credentials.** Проверьте, что `api_id` является числом, `api_hash` введён полностью, а оба значения принадлежат одному Telegram-приложению.

**Сборка падает после обновления ветки.** Сохраните полный вывод ошибки, commit SHA и конфигурацию CMake. Не удаляйте `tdata` и локальные сессии ради исправления ошибки сборки.

## Ссылки

- [LibreGram Desktop](https://github.com/W0rd3r566/libregram-desktop)
- [Ветка разработки](https://github.com/W0rd3r566/libregram-desktop/tree/libregram/dev)
- [Telegram API credentials](api_credentials.md)
- [Аудит исходников](UPSTREAM_AUDIT.md)
