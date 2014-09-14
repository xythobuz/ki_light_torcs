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
#include "BaseDriver.h"
#include "CarState.h"
#include "CarControl.h"
#include "SimpleParser.h"
#include "WrapperBaseDriver.h"
#include "controller.h"

using namespace std;

class SimpleDriver : public WrapperBaseDriver
{
private:
	bool logging;
	string logfile;
	vector<pair<CarState, CarControl> >* log;

	unsigned int position;
	vector<pair<CarState, CarControl> >* path;

	bool manual;
	// Keyboard Control for the car
	virtual void manualControl(CarControl* cc);	

	// controller for this car
	Controller* automatedControl;

public:
	
	// Constructor
	SimpleDriver(Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path, bool manual, bool logging);

	// SimpleDriver implements a simple and heuristic controller for driving
	virtual CarControl wDrive(CarState cs);

	// Print a shutdown message 
	virtual void onShutdown();
	
	// Print a restart message 
	virtual void onRestart();

	// Initialization of the desired angles for the rangefinders
	virtual void init(float *angles);
	
};

#endif /*SIMPLEDRIVER_H_*/
