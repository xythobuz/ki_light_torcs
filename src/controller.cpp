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

const static int dOut = 3;
const static int dIn = 8;
const static double eps = 0.5;

Controller::Controller() {
    fail = false;

    std::ifstream dataIn("data/logfile.txt");
    int count = 0;
    for (std::string line; std::getline(dataIn,line);){
        count++;
    }

    int n = count / 2;

    sensor = annAllocPts(n, dIn);
    actor = annAllocPts(n, dOut);

    dataIn.clear();
    dataIn.seekg(0);
    int i = 0;
    for (std::string line; std::getline(dataIn,line); i++){
        CarState cs(line);
        if (!std::getline(dataIn,line)){
            std::cout << "Unerwartetes Dateiende" << std::endl;
            throw 42;
        }
        CarControl cc(line);
        sensor[i][0] = cs.getAngle();
        sensor[i][1] = cs.getSpeedX();
        sensor[i][2] = cs.getSpeedY();
        sensor[i][3] = cs.getTrack(0);
        sensor[i][4] = cs.getTrack(18);
        sensor[i][5] = cs.getTrack(9);
        sensor[i][6] = cs.getTrack(5);
        sensor[i][7] = cs.getTrack(13);

        actor[i][0] = cc.getAccel();
        actor[i][1] = cc.getSteer();
        actor[i][2] = cc.getBrake();

        int pc = i * 100 / n;
        if (i < 100) {
            std::cout << "0";
            if (i < 10) {
                std::cout << "0";
            }
        }
        std::cout << "Training: " << pc << "%    \r";
    }

    std::cout << "Finished reading training data (~"
        << n * (dIn + dOut) * sizeof(double) / 1024 << "KB)!" << std::endl;

    tree = new ANNkd_tree(sensor, n, dIn);
}

Controller::~Controller() {
    delete tree;
    annDeallocPts(actor);
    annDeallocPts(sensor);
}

void Controller::generateVector(CarState* cs, CarControl* cc) {
    ANNpoint q = new ANNcoord[dIn];
    ANNcoord dist;
    ANNidx i;

    q[0] = cs->getAngle();
    q[1] = cs->getSpeedX();
    q[2] = cs->getSpeedY();
    q[3] = cs->getTrack(0);
    q[4] = cs->getTrack(18);
    q[5] = cs->getTrack(9);
    q[6] = cs->getTrack(5);
    q[7] = cs->getTrack(13);

    if ((cs->getTrackPos() > 1.0f) || (cs->getTrackPos() < -1.0f)) {
        if (!fail) {
            fail = true;
            std::cout << "We have left the map! Trying to recover..." << std::endl;
        }
    } else {
        if (fail) {
            fail = false;
            std::cout << "It seems as if we got back on track..." << std::endl;
        }
    }

    if (fail) {
        // Use fail-safe algorithm
        cc->setAccel(0.5f);
        cc->setBrake(0.0f);
        cc->setGear(-1);
        cc->setSteer(0.0f);
    } else {
        // Use Nearest Neighbour search
        tree->annkSearch(q, 1, &i, &dist, eps);

        cc->setAccel(actor[i][0]);
        cc->setSteer(actor[i][1]);
        cc->setBrake(actor[i][2]);

        automatic(cs, cc);
    }

    delete [] q;
}

const static int gearCount = 6;
const static int rpmDown[gearCount] = { -1, 3800, 5500, 5800, 6100, 5800 };
const static int rpmUp[gearCount]   = { 9000, 9000, 9000, 9000, 9000, -1 };

void Controller::automatic(CarState* cs, CarControl* cc) {
    int gear = cs->getGear();
    int rpm = cs->getRpm();

    if (gear == 0) {
        cc->setGear(1);
    } else if ((gear >= 1) && (gear <= gearCount)) {
        if (rpmDown[gear] != -1) {
            if (rpm < rpmDown[gear]) {
                cc->setGear(gear - 1);
                return;
            }
        }

        if (rpmUp[gear] != -1) {
            if (rpm > rpmUp[gear]) {
                cc->setGear(gear + 1);
                return;
            }
        }
    }
}

