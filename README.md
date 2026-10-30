<div align="center">

# LibreGram Desktop

**Telegram-клиент, который можно настроить под себя.**

Независимый настольный клиент на основе Kotatogram Desktop. Цель LibreGram — объединить глубокую настройку интерфейса, локальные расширения, удобное управление аккаунтами и дополнительные инструменты подключения в одном цельном приложении.

[![Base](https://img.shields.io/badge/base-Kotatogram-7965d8?style=flat-square)](https://github.com/kotatogram/kotatogram-desktop)
[![Status](https://img.shields.io/badge/status-early%20development-orange?style=flat-square)](#статус-проекта)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue?style=flat-square)](LICENSE)

</div>

> [!WARNING]
> **Проект находится на ранней стадии разработки.** Описанные ниже функции являются планами, пока не отмечены как реализованные и не прошли тестирование. Это не готовая замена Telegram Desktop.

## 🇷🇺 О проекте

LibreGram строится на кодовой базе Kotatogram Desktop (C++ / Qt). Мы хотим взять стабильную основу Telegram-клиента и развить её в сторону гибкой персонализации и удобства.

### Основные направления

- **LibreTheme** — глубокая настройка интерфейса: цвета, шрифты, отступы, фоны, темы и CSS-подобный формат пользовательского оформления.
- **Local Premium** — дополнительные локальные функции клиента. Это не Telegram Premium и не разблокировка серверных возможностей.
- **Multi-account** — удобный менеджер сохранённых аккаунтов с быстрым переключением. Долгосрочная цель — до 500 сохранённых профилей, а не 500 одновременных подключений.
- **LibreProxy** — необязательная интеграция с [Flowseal/tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy).
- **UX improvements** — аккуратные анимации, улучшенная работа с медиа, быстрые действия и другие улучшения повседневного использования.

### Документация

| Документ | Описание |
|---|---|
| [Концепция и функции](docs/PRODUCT.md) | Подробное описание идеи, модулей и ограничений |
| [Архитектура](docs/architecture.md) | Технические принципы и предполагаемое устройство |
| [Дорожная карта](docs/ROADMAP.md) | Этапы разработки |
| [Сборка Linux](docs/building-linux.md) | Унаследованная инструкция сборки |
| [WEB proxy](docs/web-proxy-plan.md) | Документация отдельного механизма WEB proxy из базовой кодовой базы |

## 🇬🇧 About

LibreGram Desktop is an independent desktop Telegram client based on Kotatogram Desktop. The project aims to combine deep interface customization, local client-side enhancements, convenient multi-account management and optional connectivity tools in one cohesive application.

### Main goals

- **LibreTheme:** extensive UI customization, custom themes and a CSS-inspired styling format.
- **Local Premium:** client-side enhancements only, not Telegram Premium entitlements.
- **Multi-account:** convenient management of saved profiles, with a long-term target of up to 500 stored accounts.
- **LibreProxy:** optional integration with [Flowseal/tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy).
- **UX improvements:** refined animations, media handling, shortcuts and everyday workflow improvements.

See the [full product specification](docs/PRODUCT.md), [architecture](docs/architecture.md) and [roadmap](docs/ROADMAP.md).

## Статус проекта

| Направление | Состояние |
|---|---|
| Выбор основы Kotatogram | Готово |
| Рабочая ветка LibreGram | Готово |
| Первичная документация | В работе |
| Аудит и проверка сборки | Не начато |
| LibreTheme | Запланировано |
| Multi-account | Запланировано |
| LibreProxy | Запланировано |
| Local Premium | Запланировано |

## Принципы

1. **Сначала стабильность.** Сначала подтверждаем базовую сборку, затем добавляем функции.
2. **Локальное значит локальное.** Не выдаём функции клиента за серверные привилегии Telegram.
3. **Безопасность аккаунтов.** Не публикуем сессии, ключи и секреты.
4. **Уважение к upstream.** Сохраняем обязательные уведомления, проверяем лицензии и требования Telegram API.
5. **Никаких фиктивных обещаний.** Функция считается готовой только после тестирования.

## Лицензия и независимость

LibreGram основан на Kotatogram Desktop и Telegram Desktop. Проект распространяется с соблюдением **GNU GPL v3.0** и применимых уведомлений об авторских правах. Перед распространением производных сборок необходимо соблюдать условия лицензии и сохранять атрибуцию.

LibreGram — независимый проект сообщества. Он не связан с Telegram и не является официальным продуктом Telegram.

---

<sub>Development branch: `libregram/dev` · Project documentation is evolving alongside the codebase.</sub>
