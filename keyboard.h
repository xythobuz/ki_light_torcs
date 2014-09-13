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

#ifdef WIN32
#include <windows.h>
#define KEY_LEFT VK_LEFT
#define KEY_RIGHT VK_RIGHT
#define KEY_UP VK_UP
#define KEY_DOWN VK_DOWN
#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_E 69
#define KEY_F 70
#define KEY_G 71
#define KEY_H 72
#define KEY_I 73
#define KEY_J 74
#define KEY_K 75
#define KEY_L 76
#define KEY_M 77
#define KEY_N 78
#define KEY_O 79
#define KEY_P 80
#define KEY_Q 81
#define KEY_R 82
#define KEY_S 83
#define KEY_T 84
#define KEY_U 85
#define KEY_V 86
#define KEY_W 87
#define KEY_X 88
#define KEY_Y 89
#define KEY_Z 90
#else
#ifdef __APPLE__
#include <SDL.h>
#else
#include <SDL/SDL.h>
#endif
#define KEY_LEFT SDLK_LEFT
#define KEY_RIGHT SDLK_RIGHT
#define KEY_UP SDLK_UP
#define KEY_DOWN SDLK_DOWN
#define KEY_A SDLK_a
#define KEY_B SDLK_b
#define KEY_C SDLK_c
#define KEY_D SDLK_d
#define KEY_E SDLK_e
#define KEY_F SDLK_f
#define KEY_G SDLK_g
#define KEY_H SDLK_h
#define KEY_I SDLK_i
#define KEY_J SDLK_j
#define KEY_K SDLK_k
#define KEY_L SDLK_l
#define KEY_M SDLK_m
#define KEY_N SDLK_n
#define KEY_O SDLK_o
#define KEY_P SDLK_p
#define KEY_Q SDLK_q
#define KEY_R SDLK_r
#define KEY_S SDLK_s
#define KEY_T SDLK_t
#define KEY_U SDLK_u
#define KEY_V SDLK_v
#define KEY_W SDLK_w
#define KEY_X SDLK_x
#define KEY_Y SDLK_y
#define KEY_Z SDLK_z
#endif

void Keyboard_Init();
void Keyboard_Quit();
void Keyboard_Update();
int Keyboard_GetState(int key);
#endif
