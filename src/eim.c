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
#include <fcitx/candidate.h>
#include <fcitx/module.h>
#include <fcitx/keys.h>
#include <fcitx/hook.h>
#include <fcitx/ui.h>
#include <libintl.h>

#include "eim.h"
#include "config.h"

CONFIG_DESC_DEFINE(GetFcitxKeyThemeConfigDesc, "fcitx-keytheme.desc")
static void *FcitxKeyThemeCreate(FcitxInstance *instance);
static void FcitxKeyThemeDestroy(void *arg);
static void FcitxKeyThemeReloadConfig(void *arg);
static boolean FcitxKeyThemePreHook(void *arg, FcitxKeySym sym,
                                    unsigned int state,
                                    INPUT_RETURN_VALUE *retval);

FCITX_EXPORT_API
const FcitxModule module = {
    .Create = FcitxKeyThemeCreate,
    .Destroy = FcitxKeyThemeDestroy,
    .SetFD = NULL,
    .ProcessEvent = NULL,
    .ReloadConfig = FcitxKeyThemeReloadConfig
};

FCITX_EXPORT_API
const int ABI_VERSION = FCITX_ABI_VERSION;

#define HOTKEY_ITEM(keyname)                                            \
    {FCITX_##keyname, KEYTHEME_KEY_##keyname, {{NULL, 0, 0}, {NULL, 0, 0}}}
#define HOTKEY_ITEM_FULL(keyname, confname)                             \
    {FCITX_##keyname, KEYTHEME_KEY_##confname, {{NULL, 0, 0}, {NULL, 0, 0}}}

typedef struct {
    FcitxHotkey *hotkey;
    int index;
    FcitxHotkey origkey[2];
} HotkeyItem;
static HotkeyItem HotkeyList[] = {
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
};

typedef FcitxHotkey DoubleHotkey[2];

static void
ApplyKeyThemeConfig(FcitxKeyThemeConfig* fc)
{
    int i;
    HotkeyItem *hotkey_item;
    FcitxHotkey *tmpkey;
    for (i = 0;i < sizeof(HotkeyList) / sizeof(HotkeyList[0]);i++) {
        hotkey_item = HotkeyList + i;
        tmpkey = fc->hotkey_list[hotkey_item->index];
        if (tmpkey[0].sym != 0 && tmpkey[0].state != 0) {
            hotkey_item->hotkey[1] = tmpkey[0];
        } else {
            hotkey_item->hotkey[1] = hotkey_item->origkey[1];
        }
        if (tmpkey[1].sym != 0 && tmpkey[1].state != 0) {
            hotkey_item->hotkey[0] = tmpkey[1];
        } else {
            hotkey_item->hotkey[0] = hotkey_item->origkey[0];
        }
    }
}

static void
SaveKeyThemeConfig(FcitxKeyThemeConfig* fc)
{
    FcitxConfigFileDesc *configDesc = GetFcitxKeyThemeConfigDesc();
    FILE *fp = FcitxXDGGetFileUserWithPrefix("conf", "fcitx-keytheme.config",
                                             "w", NULL);
    if (!fp)
        return;
    FcitxConfigSaveConfigFileFp(fp, &fc->config, configDesc);
    fclose(fp);
}

static boolean
LoadKeyThemeConfig(FcitxKeyThemeConfig* fs)
{
    FILE *fp;
    FcitxConfigFile *cfile;
    FcitxConfigFileDesc *configDesc = GetFcitxKeyThemeConfigDesc();
    if (!configDesc)
        return false;

    fp = FcitxXDGGetFileUserWithPrefix("conf", "fcitx-keytheme.config",
                                       "r", NULL);

    if (!fp) {
        if (errno == ENOENT)
            SaveKeyThemeConfig(fs);
    }
    cfile = FcitxConfigParseConfigFileFp(fp, configDesc);

    FcitxKeyThemeConfigConfigBind(fs, cfile, configDesc);
    FcitxConfigBindSync(&fs->config);

    if (fp)
        fclose(fp);
    return true;
}

static void*
FcitxKeyThemeCreate(FcitxInstance *instance)
{
    int i;
    HotkeyItem *hotkey_item;
    FcitxKeyFilterHook key_hook;
    FcitxConfigFileDesc *config_desc = GetFcitxKeyThemeConfigDesc();
    FcitxKeyTheme* theme = fcitx_utils_new(FcitxKeyTheme);
    theme->owner = instance;
    bindtextdomain("fcitx-keytheme", LOCALEDIR);
    if (!config_desc)
        return NULL;

    for (i = 0;i < sizeof(HotkeyList) / sizeof(HotkeyList[0]);i++) {
        hotkey_item = HotkeyList + i;
        hotkey_item->origkey[0] = hotkey_item->hotkey[0];
        hotkey_item->origkey[1] = hotkey_item->hotkey[1];
    }

    if (!LoadKeyThemeConfig(&theme->config)) {
        free(theme);
        return NULL;
    }
    ApplyKeyThemeConfig(&theme->config);

    key_hook.arg = theme;
    key_hook.func = FcitxKeyThemePreHook;
    FcitxInstanceRegisterPreInputFilter(instance, key_hook);

    return theme;
}

static boolean KeyThemeSelectFirst(FcitxKeyTheme *theme,
                                   INPUT_RETURN_VALUE *retval)
{
    FcitxInputState *input_state;
    FcitxCandidateWordList *word_list;
    input_state = FcitxInstanceGetInputState(theme->owner);
    if (!input_state)
        return false;
    word_list = FcitxInputStateGetCandidateList(input_state);
    if (!word_list)
        return false;
    if (FcitxCandidateWordGetListSize(word_list) <= 0)
        return false;
    /* First word? Maybe First word on current page is better.. */
    *retval = FcitxCandidateWordChooseByIndex(word_list, 0);
    return true;
}

static boolean KeyThemeGotoSingle(FcitxKeyTheme *theme,
                                  INPUT_RETURN_VALUE *retval)
{
    FcitxInputState *input_state;
    FcitxCandidateWordList *word_list;
    input_state = FcitxInstanceGetInputState(theme->owner);
    if (!input_state)
        return false;
    word_list = FcitxInputStateGetCandidateList(input_state);
    if (!word_list)
        return false;
    if (FcitxCandidateWordGetListSize(word_list) <= 0)
        return false;
    return false;
}

static boolean FcitxKeyThemePreHook(void *arg, FcitxKeySym sym,
                                    unsigned int state,
                                    INPUT_RETURN_VALUE *retval)
{
    FcitxKeyTheme *theme = (FcitxKeyTheme*)arg;
    if (FcitxHotkeyIsHotKey(sym, state, theme->config.sel_first)) {
        if (KeyThemeSelectFirst(theme, retval)) {
            return true;
        }
        return false;
    } else if (FcitxHotkeyIsHotKey(sym, state, theme->config.goto_single)) {
        if (KeyThemeGotoSingle(theme, retval)) {
            return true;
        }
        return false;
    }
    return false;
}
static void
FcitxKeyThemeDestroy(void *arg)
{
    free(arg);
}

static void
FcitxKeyThemeReloadConfig(void* arg) {
    FcitxKeyTheme* theme = (FcitxKeyTheme*)arg;
    LoadKeyThemeConfig(&theme->config);
    ApplyKeyThemeConfig(&theme->config);
}
