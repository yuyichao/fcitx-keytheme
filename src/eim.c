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
#include <fcitx-utils/log.h>
#include <fcitx-utils/utils.h>
#include <fcitx-utils/utf8.h>
#include <fcitx/instance.h>
#include <fcitx/context.h>
#include <fcitx/module.h>
#include <fcitx/hook.h>
#include <libintl.h>

#include "eim.h"
#include "config.h"
#include "redirect.h"
#include "shortcut.h"

CONFIG_DESC_DEFINE(GetFcitxKeyThemeConfigDesc, "fcitx-keytheme.desc")
static void *FcitxKeyThemeCreate(FcitxInstance *instance);
static void FcitxKeyThemeDestroy(void *arg);
static void FcitxKeyThemeReloadConfig(void *arg);

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

typedef FcitxHotkey DoubleHotkey[2];

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

static boolean
FcitxKeyThemePreHook(void *arg, FcitxKeySym sym, unsigned int state,
                     INPUT_RETURN_VALUE *retval)
{
    FcitxKeyTheme *theme = (FcitxKeyTheme*)arg;
    if (!retval)
        return false;
    if (RedirectKeyPreHook(theme, sym, state,  retval))
        return true;
    if (ShortcutPreHook(theme, sym, state,  retval))
        return true;
    return false;
}

static void*
FcitxKeyThemeCreate(FcitxInstance *instance)
{
    FcitxConfigFileDesc *config_desc = GetFcitxKeyThemeConfigDesc();
    FcitxKeyTheme* theme = fcitx_utils_new(FcitxKeyTheme);
    FcitxKeyFilterHook key_hook = {
        .arg = theme,
        .func = FcitxKeyThemePreHook,
    };
    theme->owner = instance;
    bindtextdomain("fcitx-keytheme", LOCALEDIR);
    if (!config_desc)
        return NULL;

    if (!LoadKeyThemeConfig(&theme->config)) {
        free(theme);
        return NULL;
    }
    RedirectKeyInit(theme);
    ShortcutInit(theme);

    ApplyRedirectKeyConfig(&theme->config);
    ApplyShortcutConfig(&theme->config);

    FcitxInstanceRegisterPreInputFilter(theme->owner, key_hook);
    return theme;
}

static void
FcitxKeyThemeDestroy(void *arg)
{
    free(arg);
}

static void
FcitxKeyThemeReloadConfig(void* arg)
{
    FcitxKeyTheme* theme = (FcitxKeyTheme*)arg;
    LoadKeyThemeConfig(&theme->config);
    ApplyRedirectKeyConfig(&theme->config);
}
