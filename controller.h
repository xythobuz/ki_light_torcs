/*	controller class template for AI Light Torcs project
	author: Robin Lehmann
	date: 12.04.2012
*/
#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "CarState.h"
#include "CarControl.h"

	
class Controller
{
public:
	Controller();
	~Controller();

	// automated Control for the car
	virtual void generateVector(CarState* cs, CarControl* cc);

	// get controller name
	virtual string getControllerName() { return "standard controller"; };  

};

#endif
