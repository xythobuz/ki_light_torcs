/*	controller class template for AI Light Torcs project
	author: Stefan Moebius
	date: 11.04.2012
*/
#include "controller.h"
#include <fstream>
#include "ANN/ANN.h"

static ANNkd_tree *tree;
static ANNpointArray sensor;
static ANNpointArray actor;

const static int dOut = 4;

Controller::Controller()
{
	int n = 10;
	int d = 5;
	
	// TODO: Daten einlesen
	
	sensor = annAllocPts(n, d);
	actor = annAllocPts(n, dOut);
	tree = new ANNkd_tree(sensor, n, d);
}

Controller::~Controller()
{
	delete tree;
	annDeallocPts(actor);
	annDeallocPts(sensor);
}

void Controller::generateVector(CarState* cs, CarControl* cc)
{
	// TODO: Ausgabe durch KI berechnen
}
