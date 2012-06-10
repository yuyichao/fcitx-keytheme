/***************************************************************************
 *   Copyright (C) 2012~2012 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
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
CONFIG_BINDING_REGISTER("KeyTheme", "Delete", _delete)
CONFIG_BINDING_REGISTER("KeyTheme", "Backspace", backspace)
CONFIG_BINDING_REGISTER("KeyTheme", "Home", home)
CONFIG_BINDING_REGISTER("KeyTheme", "End", end)
CONFIG_BINDING_REGISTER("KeyTheme", "Right", right)
CONFIG_BINDING_REGISTER("KeyTheme", "Left", left)
CONFIG_BINDING_REGISTER("KeyTheme", "Escape", escape)
CONFIG_BINDING_REGISTER("KeyTheme", "Semicolon", semicolon)
CONFIG_BINDING_REGISTER("KeyTheme", "Space", space)
CONFIG_BINDING_REGISTER("KeyTheme", "Comma", comma)
CONFIG_BINDING_REGISTER("KeyTheme", "Period", period)
CONFIG_BINDING_REGISTER("KeyTheme", "Reload", reload)
CONFIG_BINDING_REGISTER("KeyTheme", "Separator", separator)
CONFIG_BINDING_END()
