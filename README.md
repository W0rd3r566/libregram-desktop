<div align="center">

# LibreGram Desktop

**Твой Telegram. Твои правила. Твой интерфейс.**

Независимый настольный Telegram-клиент с упором на глубокую персонализацию, удобное управление аккаунтами, локальные расширения и контроль над подключением.

[![Status](https://img.shields.io/badge/status-early%20development-orange?style=flat-square)](#статус-проекта)
[![License](https://img.shields.io/badge/license-GPL--3.0-blue?style=flat-square)](LICENSE)

</div>

> [!WARNING]
> **LibreGram находится на ранней стадии разработки.** Возможности, описанные в документации, являются планами, пока они не реализованы и не прошли тестирование.

## 🇷🇺 О проекте

LibreGram — самостоятельный проект с собственным видением настольного Telegram-клиента. Мы хотим создать не просто очередной форк, а цельное приложение, в котором интерфейс, инструменты и дополнительные функции подчиняются единой концепции.

### Что планируется

- **LibreTheme** — мощная система оформления: цвета, шрифты, размеры, отступы, обои, анимации, пользовательские темы и CSS-подобная настройка.
- **Local Premium** — набор расширенных локальных возможностей клиента. Не путать с Telegram Premium: серверные привилегии не разблокируются.
- **Multi-account** — каталог до 500 отдельных рабочих профилей с изолированными данными. Первый слой создания и запуска реализован; поиск, группы и быстрый обзор ещё в работе.
- **LibreProxy** — необязательный модуль управления локальным прокси на основе [Flowseal/tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy).
- **Refined UX** — плавные анимации, продуманные переходы, улучшенная работа с медиа, быстрые действия и настраиваемые элементы интерфейса.
- **Privacy controls** — понятное управление локальными данными, настройками и дополнительными инструментами подключения.

### Документация

| Документ | Содержание |
|---|---|
| [Концепция и функции](docs/PRODUCT.md) | Полное описание идеи, модулей, поведения и ограничений |
| [Архитектура](docs/architecture.md) | Техническое устройство и принципы разработки |
| [Дорожная карта](docs/ROADMAP.md) | Этапы от концепции до первой публичной версии |
| [Аудит исходников](docs/UPSTREAM_AUDIT.md) | Что уже реализовано в базе и как расширять её без дублирования |
| [Профили и аккаунты](docs/MULTI_ACCOUNT.md) | Изолированные профили рабочих областей и цель в 500 сохранённых профилей |
| [Сборка Linux](docs/building-linux.md) | Инструкция по сборке исходной кодовой базы |
| [WEB proxy](docs/web-proxy-plan.md) | Отдельная документация унаследованного механизма WEB proxy |

## 🇬🇧 About

LibreGram Desktop is an independent desktop Telegram client with its own product vision. The goal is to build a cohesive application focused on deep customization, convenient account management, local enhancements and transparent connection controls.

### Planned highlights

- **LibreTheme:** extensive UI customization, wallpapers, animations, theme presets and a CSS-inspired styling format.
- **Local Premium:** client-side enhancements only, not Telegram Premium entitlements.
- **Multi-account:** account workspace with quick switching, search, groups and labels. Long-term target: up to 500 saved profiles.
- **LibreProxy:** optional integration with [Flowseal/tg-ws-proxy](https://github.com/Flowseal/tg-ws-proxy).
- **Refined UX:** polished animations, media handling, shortcuts and customizable interface elements.
- **Privacy controls:** clear management of local data and connection tools.

Read the [product specification](docs/PRODUCT.md), [architecture](docs/architecture.md), [roadmap](docs/ROADMAP.md) and [source audit](docs/UPSTREAM_AUDIT.md).

## Статус проекта

| Направление | Состояние |
|---|---|
| Концепция и документация | В работе |
| Аудит исходной кодовой базы | Выполнен, см. UPSTREAM_AUDIT.md |
| Идентичность приложения | Настроена в метаданных, сборка не проверена |
| Базовая сборка | Не проверена |
| LibreTheme | Запланировано |
| Multi-account | Запланировано |
| LibreProxy | Запланировано |
| Local Premium | Запланировано |

## Принципы

1. **Самостоятельная идентичность.** LibreGram имеет собственное название, интерфейс, документацию и направление развития.
2. **Сначала стабильность.** Новые функции не должны ломать базовую работу клиента.
3. **Локальное значит локальное.** Не выдаём возможности приложения за серверные привилегии Telegram.
4. **Безопасность аккаунтов.** Не публикуем сессии, ключи и секреты.
5. **Открытая разработка.** Изменения должны быть понятными, проверяемыми и документированными.

## Лицензия и происхождение

LibreGram использует открытую кодовую базу Telegram Desktop и компоненты проекта Kotatogram Desktop. Поэтому при распространении необходимо соблюдать применимые условия **GNU GPL v3.0**, сохранять авторские уведомления и указывать происхождение заимствованных компонентов. См. [LICENSE](LICENSE).

LibreGram — независимый проект. Он не связан с Telegram и не является официальным продуктом Telegram.

---

<sub>Development branch: `libregram/dev` · Independent community project.</sub>
