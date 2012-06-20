/***************************************************************************
 *   Copyright (C) 2012~2012 by Yichao Yu                                  *
 *   yyc1992@gmail.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#ifndef EIM_H
#define EIM_H

#include <fcitx/ime.h>
#include <fcitx/instance.h>

#define _(x) dgettext("fcitx-theme", x)

enum {
    KEYTHEME_KEY_DELETE,
    KEYTHEME_KEY_BACKSPACE,
    KEYTHEME_KEY_HOME,
    KEYTHEME_KEY_END,
    KEYTHEME_KEY_RIGHT,
    KEYTHEME_KEY_LEFT,
    KEYTHEME_KEY_ESCAPE,
    KEYTHEME_KEY_ENTER,
    KEYTHEME_KEY_SEMICOLON,
    KEYTHEME_KEY_SPACE,
    KEYTHEME_KEY_COMMA,
    KEYTHEME_KEY_PERIOD,
    KEYTHEME_KEY_RELOAD,
    KEYTHEME_KEY_SEPARATOR,
    _KEYTHEME_KEY_LAST
};

enum {
    SHORTCUT_KEY_SEL_FIRST,
    SHORTCUT_KEY_GO_SINGLE,
    SHORTCUT_KEY_GO_FIRST,
    SHORTCUT_KEY_GO_LAST,
    _SHORTCUT_KEY_LAST,
};

typedef struct _FcitxKeyThemeConfig {
    FcitxGenericConfig config;
    boolean single_forward;
    FcitxHotkey redirect_list[_KEYTHEME_KEY_LAST][2];
    FcitxHotkey shortcut_list[_SHORTCUT_KEY_LAST][2];
} FcitxKeyThemeConfig;

typedef struct _FcitxKeyTheme {
    FcitxKeyThemeConfig config;
    FcitxInstance* owner;
} FcitxKeyTheme;

CONFIG_BINDING_DECLARE(FcitxKeyThemeConfig);

#endif
