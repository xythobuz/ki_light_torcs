/***************************************************************************

file                 : SimpleDriver.cpp
copyright            : (C) 2007 Daniele Loiacono

Modifications:


 ***************************************************************************/

/***************************************************************************
 * Modifications:                                                          *
 * DATE         AUTHOR          CHANGES                                    *
 * ------------------------------------------------------------------------*
 * 2011-10-29	Stefan Moebius	Removed unused functions and replaced      *
 wDrive                                     *
 * 2012-04-13	Robin Lehmann	Some minor changes 						   *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "SimpleDriver.h"
#include "keyboard.h"
#include <fstream>

SimpleDriver::SimpleDriver(Controller* cntrl, string logfile) {
    this->automatedControl = cntrl;
    this->logfile = logfile;

    if (automatedControl == NULL) {
        Keyboard_Init();
        this->log = new vector<pair<CarState, CarControl> >;
    }
}

string SimpleDriver::drive(string sensors) {
    CarState cs(sensors);
    CarControl nextVector = CarControl(0.0f, 0.0f, cs.getGear(), 0.0f, 0.0f);

    if (automatedControl == NULL) {
        Keyboard_Update(&nextVector);
        log->push_back(pair<CarState, CarControl>(cs, nextVector));
    } else {
        this->automatedControl->generateVector(&cs, &nextVector);
    }

    return nextVector.toString();
}

void SimpleDriver::onShutdown() {
    if (automatedControl == NULL) {
        Keyboard_Quit();

        cout << "Writing log to harddrive" << endl;
        ofstream logfile(this->logfile.c_str());
        for (unsigned int i = 0; i < (*log).size(); i++) {
            logfile << (*log)[i].first.toString() << endl;
            logfile << (*log)[i].second.toString() << endl;
        }
        log->clear();
    }
}

void SimpleDriver::init(float *angles) {
    for (int i = 0; i < 5; i++) {
        angles[i] = -90 + i * 15;
        angles[18 - i] = 90 - i * 15;
    }

    for (int i = 5; i < 9; i++) {
        angles[i] = -20 + (i - 5) * 5;
        angles[18 - i] = 20 - (i - 5) * 5;
    }

    angles[9] = 0;
}

