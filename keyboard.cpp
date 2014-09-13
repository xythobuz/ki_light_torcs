/*
 * File:        keyboard.c
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
#include "keyboard.h"

void Keyboard_Init()
{
	#ifdef WIN32
		// No init required
	#else
	//TODO: Abfragen, ob initalisierung korrekt
	//SDL_Rect rect;
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetVideoMode (100, 100, 24, SDL_SWSURFACE);
	//SDL_WM_GrabInput(SDL_GRAB_ON);
	SDL_EnableKeyRepeat (SDL_DEFAULT_REPEAT_DELAY, 1);
	#endif
}

void Keyboard_Quit()
{
	#ifdef WIN32
		// No quit required
	#else
	SDL_Quit();
	#endif
}

void Keyboard_Update()
{
	#ifdef WIN32
		// No update required
	#else
	SDL_Event e;
	SDL_PollEvent(&e);
	#endif
}

int Keyboard_GetState(int key)
{
	#ifdef WIN32
	return GetAsyncKeyState(key);
	#else
	return SDL_GetKeyState(NULL)[key];
	#endif
}





