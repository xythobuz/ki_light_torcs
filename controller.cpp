/*	controller class template for AI Light Torcs project
	author: Stefan Moebius
	date: 11.04.2012
*/
#include "controller.h"
#include "pid.h"
#include "stdio.h"

#include <time.h>

PID pid(0.75f, 0.02f, 0.35f, -1.0f, 1.0f);

Controller::Controller()
{
	//TODO: evtl. KI Initialisieren
	//TODO: Trainingsdaten von Datei laden
}

Controller::~Controller()
{
	//TODO: Trainingsdaten in Datei abspeichern
}

float getMaxSpeed(CarState *cs) {
    float maxSpeed = 50.0f;
    float dist = cs->getTrack((TRACK_SENSORS_NUM / 2) + 1);
    float distStop = (cs->getSpeedX() / 10) * (cs->getSpeedX() / 10) * 0.5f;
    if (dist >= distStop)
        maxSpeed = 125.0f;
    if ((dist / 2.0f) >= distStop)
        maxSpeed = 175.0f;
    if (dist < (distStop / 2.0f))
        maxSpeed = 15.0f;
    return maxSpeed;
}

float Controller::accelControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code einfügen, der die Beschleunigung des Autos regelt
	float accel = 1.0f; //Für einen ersten test hier einfach auf "Vollgas" gesetzt...
    float maxSpeed = getMaxSpeed(cs);
	if (cs->getSpeedX() >= maxSpeed)
		accel = 0.0f;
	return accel;
}

float Controller::brakeControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code einfügen, der das Auto abbremst, z.B. in Kurven...
	float brake = 0.0f;
    float maxSpeed = getMaxSpeed(cs);
    if (cs->getSpeedX() >= maxSpeed)
        brake = 1.0f;
	return brake;
}

float Controller::steerControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code für die Lenkung einfügen
	//Es muss eine Fleißkommazahl im Wertebereich von -1.0 bis 1.0 zurückgegeben werden.
	//-1.0: stark nach rechts lenken
	// 0.0: in keine Richtung lenken
	// 1.0: stark nach links lenken
	float steer = 0.0f;

	float is = cs->getTrack(TRACK_SENSORS_NUM - 1);
	is -= cs->getTrack(0);
	steer = pid.execute(0.0f, is);

	return steer;
}


float Controller::clutchControl(CarState* cs, CarControl* cc)
{
	//TODO: Kupplung implementieren (nicht unbedingt nötig)
	float clutch = 0.0f;
	return clutch;
}

int Controller::gearControl(CarState* cs, CarControl* cc)
{
	//TODO: Gangschaltung implementieren
	// mögliche Werte sind -1 (Rückwärtsgang), 0 (Leerlauf), 1, 2, 3, 4, 5, 6
	static int gear = 1; //hier in diesem Beispiel einfach immer 1 Gang verwenden

	if (cs->getRpm() >= 5000) {
		if (gear < 6) {
			gear++;
		}
	} else if (cs->getRpm() <= 1250) {
		if (gear > 1) {
			gear--;
		}
	}

	return gear;
}

void Controller::generateVector(CarState* cs, CarControl* cc)
{
	//TODO: KI Trainieren
	//TODO: Ausgabe durch KI berechnen
	cc->setAccel(accelControl(cs, cc));
	cc->setBrake(brakeControl(cs, cc));
	cc->setSteer(steerControl(cs, cc));
	cc->setClutch(clutchControl(cs, cc));
	cc->setGear(gearControl(cs, cc));
}
