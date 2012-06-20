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
#include <fcitx/keys.h>
#include "eim.h"
#include "config.h"

#define HOTKEY_ITEM(keyname)                                            \
    {FCITX_##keyname, KEYTHEME_KEY_##keyname, {{NULL, 0, 0}, {NULL, 0, 0}}}
#define HOTKEY_ITEM_FULL(keyname, confname)                             \
    {FCITX_##keyname, KEYTHEME_KEY_##confname, {{NULL, 0, 0}, {NULL, 0, 0}}}
#define HOTKEY_ITEM_LAST \
    {NULL, -1, {{NULL, 0, 0}, {NULL, 0, 0}}}

KeyThemeItem HotkeyList[] = {
    HOTKEY_ITEM(DELETE),
    HOTKEY_ITEM(BACKSPACE),
    HOTKEY_ITEM(HOME),
    HOTKEY_ITEM(END),
    HOTKEY_ITEM(RIGHT),
    HOTKEY_ITEM(LEFT),
    HOTKEY_ITEM(ESCAPE),
    HOTKEY_ITEM(ENTER),
    HOTKEY_ITEM(SEMICOLON),
    HOTKEY_ITEM(SPACE),
    HOTKEY_ITEM(COMMA),
    HOTKEY_ITEM(PERIOD),
    HOTKEY_ITEM_FULL(CTRL_5, RELOAD),
    HOTKEY_ITEM(SEPARATOR),
    HOTKEY_ITEM_LAST
};

