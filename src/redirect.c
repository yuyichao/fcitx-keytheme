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
#include <time.h>
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
RedirectKeyInit(FcitxKeyTheme *theme)
{
    int i;
    RedirectItem *item;
    /* When using ProcessKey to do real redirection, this backup is probably
     * not necessary anymore (Don't think other plugins may be as crazy as
     * this one which modify some constant in the program). Keep it for now
     * since it doesn't cost too much either.
     */
    for (i = 0;HotkeyList[i].index >= 0;i++) {
        item = HotkeyList + i;
        item->origkey[0] = item->hotkey[0];
        item->origkey[1] = item->hotkey[1];
    }
}

void
ApplyRedirectKeyConfig(FcitxKeyThemeConfig *fc)
{
}

boolean
RedirectKeyPreHook(FcitxKeyTheme *theme, FcitxKeySym sym, unsigned int state,
                   INPUT_RETURN_VALUE *retval)
{
    static volatile boolean busy = false;
    int i;
    RedirectItem *item;
    /* FcitxKeySym r_sym; */
    /* unsigned int r_state; */
    /* FcitxInputState *input_state; */
    /* input_state = FcitxInstanceGetInputState(theme->owner); */
    /* r_sym = FcitxInputStateGetKeySym(input_state); */
    /* r_state = FcitxInputStateGetKeyState(input_state); */
    FcitxKeyThemeConfig *fc = &theme->config;
    boolean res = false;

    if (busy)
        return false;
    busy = true;

    for (i = 0;(item = HotkeyList + i)->index >= 0;i++) {
        if (FcitxHotkeyIsHotKey(sym, state, fc->redirect_list[item->index])) {
            *retval = FcitxInstanceProcessKey(theme->owner, FCITX_PRESS_KEY,
                                              time(NULL), item->origkey->sym,
                                              item->origkey->state);
            /* Just in case. */
            if (!*retval || (*retval & IRV_FLAG_FORWARD_KEY))
                *retval = IRV_FLAG_FORWARD_KEY;
            else
                *retval = IRV_FLAG_BLOCK_FOLLOWING_PROCESS;
            res = true;
            break;
        }
    }

    busy = false;
    return res;
}
