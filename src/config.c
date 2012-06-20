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

#include "eim.h"

/* USE fcitx provided macro to bind config and variable */
CONFIG_BINDING_BEGIN(FcitxKeyThemeConfig)
CONFIG_BINDING_REGISTER("Shortcut", "SelectFirst",
                        shortcut_list[SHORTCUT_KEY_SEL_FIRST])
CONFIG_BINDING_REGISTER("Shortcut", "GoToSingleWord",
                        shortcut_list[SHORTCUT_KEY_GO_SINGLE])
CONFIG_BINDING_REGISTER("Shortcut", "GoToFirstPage",
                        shortcut_list[SHORTCUT_KEY_GO_FIRST])
CONFIG_BINDING_REGISTER("Shortcut", "GoToLastPage",
                        shortcut_list[SHORTCUT_KEY_GO_LAST])
CONFIG_BINDING_REGISTER("Shortcut", "SingleOnlyForward", single_forward)
CONFIG_BINDING_REGISTER("Redirect", "Delete",
                        redirect_list[KEYTHEME_KEY_DELETE])
CONFIG_BINDING_REGISTER("Redirect", "Backspace",
                        redirect_list[KEYTHEME_KEY_BACKSPACE])
CONFIG_BINDING_REGISTER("Redirect", "Home",
                        redirect_list[KEYTHEME_KEY_HOME])
CONFIG_BINDING_REGISTER("Redirect", "End", redirect_list[KEYTHEME_KEY_END])
CONFIG_BINDING_REGISTER("Redirect", "Right", redirect_list[KEYTHEME_KEY_RIGHT])
CONFIG_BINDING_REGISTER("Redirect", "Left", redirect_list[KEYTHEME_KEY_LEFT])
CONFIG_BINDING_REGISTER("Redirect", "Escape",
                        redirect_list[KEYTHEME_KEY_ESCAPE])
CONFIG_BINDING_REGISTER("Redirect", "Semicolon",
                        redirect_list[KEYTHEME_KEY_SEMICOLON])
CONFIG_BINDING_REGISTER("Redirect", "Space", redirect_list[KEYTHEME_KEY_SPACE])
CONFIG_BINDING_REGISTER("Redirect", "Comma", redirect_list[KEYTHEME_KEY_COMMA])
CONFIG_BINDING_REGISTER("Redirect", "Period",
                        redirect_list[KEYTHEME_KEY_PERIOD])
CONFIG_BINDING_REGISTER("Redirect", "Reload",
                        redirect_list[KEYTHEME_KEY_RELOAD])
CONFIG_BINDING_REGISTER("Redirect", "Separator",
                        redirect_list[KEYTHEME_KEY_SEPARATOR])
CONFIG_BINDING_END()
