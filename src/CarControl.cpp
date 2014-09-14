/***************************************************************************

file                 : CarControl.cpp
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

#include "SimpleParser.h"
#include "CarControl.h"

CarControl::CarControl(float accel, float brake, int gear, float steer, float clutch, int focus, int meta) {
    this->accel = accel;
    this->brake = brake;
    this->gear  = gear;
    this->steer = steer;
    this->clutch = clutch;
    this->focus = focus;
    this->meta = meta;
}

CarControl::CarControl(string sensors) {
    if (SimpleParser::parse(sensors, "accel", accel)==false)
        accel=0.0;
    if (SimpleParser::parse(sensors, "brake", brake)==false)
        brake=0.0;
    if (SimpleParser::parse(sensors, "gear",  gear)==false)
        gear=1;
    if (SimpleParser::parse(sensors, "steer", steer)==false)
        steer=0.0;
    if (SimpleParser::parse(sensors, "clutch", clutch)==false)
        clutch=0.0;
    if (SimpleParser::parse(sensors, "meta", meta)==false)
        meta=0;
    if (SimpleParser::parse(sensors, "focus", focus)==false)
        focus=0;
    if (focus < -90 || focus > 90)
        focus=360;
}

string CarControl::toString() {
    string str;
    str  = SimpleParser::stringify("accel", accel);
    str += SimpleParser::stringify("brake", brake);
    str += SimpleParser::stringify("gear",  gear);
    str += SimpleParser::stringify("steer", steer);
    str += SimpleParser::stringify("clutch", clutch);
    str += SimpleParser::stringify("focus",  focus);
    str += SimpleParser::stringify("meta", meta);
    return str;
}

float CarControl::getAccel() {
    return this->accel;
}

void CarControl::setAccel (float accel) {
    this->accel = accel;
}

float CarControl::getBrake() {
    return this->brake;
}

void CarControl::setBrake (float brake) {
    this->brake = brake;
}

int CarControl::getGear() {
    return this->gear;
}

void CarControl::setGear(int gear) {
    this->gear = gear;
}

float CarControl::getSteer() {
    return this->steer;
}

void CarControl::setSteer(float steer) {
    this->steer = steer;
}

int CarControl::getMeta() {
    return this->meta;
}

void CarControl::setMeta(int meta) {
    this->meta = meta;
}

float CarControl::getClutch() {
    return clutch;
}

void CarControl::setClutch(float clutch) {
    this->clutch = clutch;
}

int CarControl::getFocus() {
    return this->focus;
}

void CarControl::setFocus(int focus) {
    this->focus = focus;
}
