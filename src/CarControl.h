/***************************************************************************

file                 : CarControl.h
copyright            : (C) 2007 Daniele Loiacono

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef CARCONTROL_H_
#define CARCONTROL_H_

#include <string>

class CarControl {
public:

    CarControl(std::string sensors);
    CarControl(float accel, float brake, int gear, float steer, float clutch, int focus = 0, int meta = 0);
    std::string toString();

    float getAccel();
    void setAccel (float accel);

    float getBrake();
    void setBrake (float brake);

    int getGear();
    void setGear(int gear);

    float getSteer();
    void setSteer(float steer);

    int getMeta();
    void setMeta(int gear);

    float getClutch();
    void setClutch(float clutch);

    int getFocus();
    void setFocus(int focus);

    const static int META_RESTART = 1;

private:
    float accel;
    float brake;
    int gear;
    float steer;
    float clutch;
    int meta;
    int focus;
};

#endif

