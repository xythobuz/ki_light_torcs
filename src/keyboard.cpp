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

#include <SDL.h>
#include <iostream>
#include <cstdlib>

#include "keyboard.h"

static SDL_Joystick *joystick;
static float steer;
static float accel;
static float brake;

void Keyboard_Init() {
    //TODO: Abfragen, ob initalisierung korrekt
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    SDL_SetVideoMode (100, 100, 24, SDL_SWSURFACE);
    if(SDL_NumJoysticks() < 1){
        std::cout << "Fehler, kein Joystick gefunden!" << std::endl;
        exit(1);
    }
    SDL_JoystickEventState(SDL_ENABLE);
    joystick = SDL_JoystickOpen(0);
    steer = accel = brake = 0;
}

void Keyboard_Quit() {
    SDL_Quit();
}

void Keyboard_Update(CarControl* cc) {
    cc->setAccel(accel);
    cc->setBrake(brake);
    cc->setSteer(steer);

    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_JOYAXISMOTION:  /* Handle Joystick Motion */
                if( event.jaxis.axis == 0){
                    //L
                    //std::cout << "L" << event.jaxis.value << endl;
                    float g;
                    if ( ( event.jaxis.value < -4500 ) || (event.jaxis.value > 4500 ) )
                        g = ((float) event.jaxis.value) / 32768.0f;
                    else
                        g = 0;
                    cc->setSteer(-g);
                    steer = -g;
                }
                if( event.jaxis.axis == 4){
                    //RT
                    //std::cout << "RT" << event.jaxis.value << endl;
                    float g;
                    if ( ( event.jaxis.value < -4500 ) || (event.jaxis.value > 4500 ) )
                        g = ((float) event.jaxis.value + 32768) / 65535.0f;
                    else
                        g = 0;
                    cc->setAccel(g);
                    accel = g;
                }
                if( event.jaxis.axis == 5){
                    //LT
                    //std::cout << "LT" << event.jaxis.value << endl;
                    float g;
                    if ( ( event.jaxis.value < -4500 ) || (event.jaxis.value > 4500 ) )
                        g = ((float) event.jaxis.value + 32768) / 65535.0f;
                    else
                        g = 0;
                    cc->setBrake(g);
                    brake = g;
                }
                break;

            /*
            case SDL_JOYBUTTONDOWN:
                if ( event.jbutton.button == 1 ){
                    //B
                    if (cc->getGear() == -1){
                        cc->setGear(1);
                    }
                    else if (cc->getGear() < 6){
                        cc->setGear(cc->getGear() + 1);
                    }
                }
                if ( event.jbutton.button == 0 ){
                    //A
                    if (cc->getGear() == 1){
                        cc->setGear(-1);
                    }
                    else if ( cc->getGear() > 1){
                        cc->setGear(cc->getGear() - 1);
                    }
                }
                break;
            */
        }
    }
}

