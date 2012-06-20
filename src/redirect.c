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
#include "redirect.h"
#include "config.h"

#define REDIRECT_ITEM(keyname)                                            \
    {FCITX_##keyname, KEYTHEME_KEY_##keyname, {{NULL, 0, 0}, {NULL, 0, 0}}}
#define REDIRECT_ITEM_FULL(keyname, confname)                             \
    {FCITX_##keyname, KEYTHEME_KEY_##confname, {{NULL, 0, 0}, {NULL, 0, 0}}}
#define REDIRECT_ITEM_LAST \
    {NULL, -1, {{NULL, 0, 0}, {NULL, 0, 0}}}

typedef struct {
    FcitxHotkey *hotkey;
    int index;
    FcitxHotkey origkey[2];
} RedirectItem;

static RedirectItem HotkeyList[] = {
    REDIRECT_ITEM(DELETE),
    REDIRECT_ITEM(BACKSPACE),
    REDIRECT_ITEM(HOME),
    REDIRECT_ITEM(END),
    REDIRECT_ITEM(RIGHT),
    REDIRECT_ITEM(LEFT),
    REDIRECT_ITEM(ESCAPE),
    REDIRECT_ITEM(ENTER),
    REDIRECT_ITEM(SEMICOLON),
    REDIRECT_ITEM(SPACE),
    REDIRECT_ITEM(COMMA),
    REDIRECT_ITEM(PERIOD),
    REDIRECT_ITEM_FULL(CTRL_5, RELOAD),
    REDIRECT_ITEM(SEPARATOR),
    REDIRECT_ITEM_LAST
};

void
RedirectInit()
{
    int i;
    RedirectItem *item;
    for (i = 0;HotkeyList[i].index >= 0;i++) {
        item = HotkeyList + i;
        item->origkey[0] = item->hotkey[0];
        item->origkey[1] = item->hotkey[1];
    }
}

void
ApplyRedirectKeyConfig(FcitxKeyThemeConfig* fc)
{
    int i;
    RedirectItem *item;
    FcitxHotkey *tmpkey;
    for (i = 0;HotkeyList[i].index >= 0;i++) {
        item = HotkeyList + i;
        tmpkey = fc->hotkey_list[item->index];
        if (tmpkey[0].sym != 0 && tmpkey[0].state != 0) {
            item->hotkey[1] = tmpkey[0];
        } else {
            item->hotkey[1] = item->origkey[1];
        }
        if (tmpkey[1].sym != 0 && tmpkey[1].state != 0) {
            item->hotkey[0] = tmpkey[1];
        } else {
            item->hotkey[0] = item->origkey[0];
        }
    }
}
