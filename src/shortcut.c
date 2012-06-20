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
#include <fcitx/hook.h>
#include <fcitx/candidate.h>
#include "shortcut.h"
#include "config.h"

typedef boolean (*KTShortcutAction)(FcitxKeyTheme *theme,
                                    INPUT_RETURN_VALUE *retval);

static boolean
FcitxKeyThemePreHook(void *arg, FcitxKeySym sym, unsigned int state,
                     INPUT_RETURN_VALUE *retval);

void ShortcutInit(FcitxKeyTheme *theme)
{
    FcitxKeyFilterHook key_hook;

    key_hook.arg = theme;
    key_hook.func = FcitxKeyThemePreHook;
    FcitxInstanceRegisterPreInputFilter(theme->owner, key_hook);
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
    /* .... this IS the first word on the page ....
     * So what if I want to really get the first word then?
     * go to first page and select?.....
     */
    *retval = FcitxCandidateWordChooseByIndex(word_list, 0);
    return true;
}

static int KeyThemeFindSingle(FcitxCandidateWordList *word_list)
{
    FcitxCandidateWord *word;
    int i = 0;
    for (word = FcitxCandidateWordGetFirst(word_list);
         word;
         word = FcitxCandidateWordGetNext(word_list, word), i++) {
             size_t len = word->strWord ? fcitx_utf8_strlen(word->strWord) : 0;
             if (len == 1)
                 return i;
         }
    return -1;
}

static boolean KeyThemeGotoSingle(FcitxKeyTheme *theme,
                                  INPUT_RETURN_VALUE *retval)
{
    int i;
    int single_index;
    int page_size;
    int page_index;
    int cur_page;
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
    single_index = KeyThemeFindSingle(word_list);
    if (single_index < 0)
        return false;
    page_size = FcitxCandidateWordGetPageSize(word_list);
    if (page_size <= 0)
        return false;
    page_index = single_index / page_size;
    cur_page = FcitxCandidateWordGetCurrentPage(word_list);
    if (cur_page < 0)
        return false;
    if (cur_page > page_index) {
        if (theme->config.single_forward)
            return false;
        for (i = cur_page - page_index;i > 0;i--) {
            FcitxCandidateWordGoPrevPage(word_list);
        }
    } else {
        for (i = page_index - cur_page;i > 0;i--) {
            FcitxCandidateWordGoNextPage(word_list);
        }
    }
    *retval = IRV_FLAG_UPDATE_INPUT_WINDOW;
    return true;
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
    }/* else if (FcitxHotkeyIsHotKey(sym, state, theme->config.goto_first)) {
        if (KeyThemeGotoSingle(theme, retval)) {
            return true;
        }
        return false;
    } else if (FcitxHotkeyIsHotKey(sym, state, theme->config.goto_last)) {
        if (KeyThemeGotoSingle(theme, retval)) {
            return true;
        }
        return false;
    } */
    return false;
}
