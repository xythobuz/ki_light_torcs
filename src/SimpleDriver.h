/***************************************************************************

file                 : SimpleDriver.h
copyright            : (C) 2007 Daniele Loiacono

 ***************************************************************************/

/***************************************************************************
 * Modifications:                                                          *
 * DATE         AUTHOR          CHANGES                                    *
 * ------------------------------------------------------------------------*
 * 2011-10-29	Stefan Moebius	Removed unused declarations                *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef SIMPLEDRIVER_H_
#define SIMPLEDRIVER_H_

#include <iostream>
#include <cmath>
#include <map>
#include <vector>

#include "CarState.h"
#include "CarControl.h"
#include "SimpleParser.h"
#include "controller.h"

using namespace std;

class SimpleDriver
{
    private:
        bool logging;
        string logfile;
        vector<pair<CarState, CarControl> >* log;

        unsigned int position;
        vector<pair<CarState, CarControl> >* path;

        bool manual;

        // controller for this car
        Controller* automatedControl;

    public:

        typedef enum {
            WARMUP,
            QUALIFYING,
            RACE,
            UNKNOWN
        } tstage;

        tstage stage;
        char trackName[100];

        // Constructor
        SimpleDriver(Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path, bool manual, bool logging);

        // Print a shutdown message
        void onShutdown();

        // Initialization of the desired angles for the rangefinders
        void init(float *angles);

        // the drive function wiht string input and output
        string drive(string sensors);
};

#endif /*SIMPLEDRIVER_H_*/
