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
const static int dIn = 10;
const static double eps = 0.5;

Controller::Controller()
{
	std::ifstream dataIn("data/logfile.txt");
	int count = 0;
	for (std::string line; std::getline(dataIn,line);){
		count++;
	}
	
	int n = count/2;
	std::cout << "n:" << n << std::endl;
	
	sensor = annAllocPts(n, dIn);
	actor = annAllocPts(n, dOut);
	
	dataIn.clear();
	dataIn.seekg(0);
	int i = 0;
	for (std::string line; std::getline(dataIn,line); i++){
		CarState cs(line);
		if(!std::getline(dataIn,line)){
			std::cout << "unerwartetes Dateiende" << endl;
			throw 42;
		}
		if(i == 0){
			std::cout << "line:" << line << std::endl;
		}
		CarControl cc(line);
		sensor[i][0] = cs.getAngle();
		sensor[i][1] = cs.getGear();
		sensor[i][2] = cs.getRpm();
		sensor[i][3] = cs.getSpeedX();
		sensor[i][4] = cs.getSpeedY();
		sensor[i][5] = cs.getTrack(0);
		sensor[i][6] = cs.getTrack(18);
		sensor[i][7] = cs.getTrack(9);
		sensor[i][8] = cs.getTrack(5);
		sensor[i][9] = cs.getTrack(13);
		
		actor[i][0] = cc.getAccel();
		actor[i][1] = cc.getGear();
		actor[i][2] = cc.getSteer();
		actor[i][3] = cc.getBrake();
	}
	
	std::cout << sensor[0][0] << " " << sensor[0][1] << " " << sensor[0][2] << " " << sensor[0][3] << std::endl;
	std::cout << actor[0][0] << " " << actor[0][1] << " " << actor[0][2] << " " << actor[0][3] << std::endl;

	tree = new ANNkd_tree(sensor, n, dIn);
}

Controller::~Controller()
{
	delete tree;
	annDeallocPts(actor);
	annDeallocPts(sensor);
}

void Controller::generateVector(CarState* cs, CarControl* cc)
{
	ANNpoint q = new ANNcoord[dIn]; 
	ANNcoord dist;
	ANNidx i;
	q[0] = cs->getAngle();
	q[1] = cs->getGear();
	q[2] = cs->getRpm();
	q[3] = cs->getSpeedX();
	q[4] = cs->getSpeedY();
	q[5] = cs->getTrack(0);
	q[6] = cs->getTrack(18);
	q[7] = cs->getTrack(9);
	q[8] = cs->getTrack(5);
	q[9] = cs->getTrack(13);
	
	tree->annkSearch(q,1,&i,&dist,eps);
	std::cout << "found at " << i << " distance " << dist << std::endl; 
	
	cc->setAccel(actor[i][0]);
	cc->setGear(actor[i][1]);
	cc->setSteer(actor[i][2]);
	cc->setBrake(actor[i][3]);
	
	delete [] q;
}
