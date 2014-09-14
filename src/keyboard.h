/*
 * File:        keyboard.h
 * Title:
 * Author:      Christian Eppler, Stefan Moebius
 * Created on:  22.05.2011
 * Version:
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program;
 * if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KEYBOARDHELPER
#define KEYBOARDHELPER

#include "CarControl.h"

void Keyboard_Init();
void Keyboard_Quit();
void Keyboard_Update(CarControl* cc);

#endif

