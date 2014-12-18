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
const static int dIn = 10;
const static double eps = 0.2;

static void fillArray(CarState* cs, ANNpoint q) {
    // Range: [-pi, pi]
    q[0] = cs->getAngle() / 3.14159;

    // Range: [0, 200]
    const static double rangeFactor = 200.0;
    q[1] = cs->getTrack(0) / rangeFactor;
    q[2] = cs->getTrack(18) / rangeFactor;
    q[3] = cs->getTrack(9) / rangeFactor;
    q[4] = cs->getTrack(5) / rangeFactor;
    q[5] = cs->getTrack(13) / rangeFactor;
    q[6] = cs->getTrack(7) / rangeFactor;
    q[7] = cs->getTrack(11) / rangeFactor;

    // Range: [0, inf]
    q[8] = cs->getDistFromStart() / 1000.0;

    // Range: [-inf, inf]
    q[9] = cs->getSpeedX() / 300.0;
}

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

        fillArray(&cs, sensor[i]);
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

    fillArray(cs, q);

    if ((cs->getTrackPos() > 1.0f) || (cs->getTrackPos() < -1.0f)) {
        if (!fail) {
            fail = true;
            std::cout << "We have left the map! Trying to recover..." << std::endl;
        }
    } else {
        if (fail) {
            fail = false;
            std::cout << "It seems as if we got back on track..." << std::endl;
            cc->setGear(1);
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
const static int speedDown[gearCount] = { -1, 45, 115, 170, 215, 255 };
const static int speedUp[gearCount]   = { 50, 120, 175, 220, 260, -1 };

void Controller::automatic(CarState* cs, CarControl* cc) {
    int gear = cs->getGear();
    int speed = cs->getSpeedX();

    if (gear == 0) {
        cc->setGear(1);
    } else if ((gear >= 1) && (gear <= gearCount)) {
        if (speedDown[gear - 1] != -1) {
            if (speed < speedDown[gear - 1]) {
                cc->setGear(gear - 1);
                return;
            }
        }

        if (speedUp[gear - 1] != -1) {
            if (speed > speedUp[gear - 1]) {
                cc->setGear(gear + 1);
                return;
            }
        }
    }
}

