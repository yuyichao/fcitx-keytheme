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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcitx/ime.h>
#include <fcitx-config/fcitx-config.h>
#include <fcitx-config/xdg.h>
#include <fcitx-config/hotkey.h>
#include <fcitx-utils/log.h>
#include <fcitx-utils/utils.h>
#include <fcitx-utils/utf8.h>
#include <fcitx/instance.h>
#include <fcitx/context.h>
#include <fcitx/module.h>
#include <fcitx/keys.h>
#include <fcitx/ui.h>
#include <libintl.h>

#include "eim.h"
#include "config.h"

CONFIG_DESC_DEFINE(GetFcitxKeyThemeConfigDesc, "fcitx-keytheme.desc")
static void *FcitxKeyThemeCreate(FcitxInstance *instance);
static void FcitxKeyThemeDestroy(void *arg);

FCITX_EXPORT_API
const FcitxModule module = {
    .Create = FcitxKeyThemeCreate,
    .Destroy = FcitxKeyThemeDestroy,
    .SetFD = NULL,
    .ProcessEvent = NULL,
    .ReloadConfig = NULL
};
FCITX_EXPORT_API
const int ABI_VERSION = FCITX_ABI_VERSION;

#define eprintf(format, args...)                        \
    fprintf(stderr, "\e[35m\e[1m"format"\e[0m", ##args)
#define __pfunc__() eprintf("%s\n", __func__)

struct {
    FcitxHotkey *hotkey;
    const char *name;
} HotkeyList[] = {
    {FCITX_DELETE, "Delete"},
    {FCITX_BACKSPACE, "Backspace"},
    {FCITX_HOME, "Home"},
    {FCITX_END, "End"},
    {FCITX_RIGHT, "Right"},
    {FCITX_LEFT, "Left"},
    {FCITX_ESCAPE, "Escape"},
    {FCITX_ENTER, "Enter"},
    {FCITX_SEMICOLON, "Semicolon"},
    {FCITX_SPACE, "Space"},
    {FCITX_COMMA, "Comma"},
    {FCITX_PERIOD, "Period"},
    {FCITX_CTRL_5, "Reload"},
    {FCITX_SEPARATOR, "Separator"},
};

static void*
FcitxKeyThemeCreate(FcitxInstance *instance)
{
    /* FcitxGlobalConfig* config = FcitxInstanceGetGlobalConfig(instance); */
    /* FcitxInputState *input = FcitxInstanceGetInputState(instance); */
    FcitxConfigFileDesc *config_desc = GetFcitxKeyThemeConfigDesc();
    if (!config_desc)
        return NULL;

    __pfunc__();
    bindtextdomain("fcitx-keytheme", LOCALEDIR);
    FcitxHotkey mhotkey = {
        .desc = NULL,
        .sym = FcitxKey_D,
        .state = FcitxKeyState_Ctrl
    };
    FCITX_DELETE[1] = mhotkey;
    return instance;
}

static void
FcitxKeyThemeDestroy(void *arg)
{
}
