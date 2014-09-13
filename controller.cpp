/*	controller class template for AI Light Torcs project
	author: Stefan Moebius
	date: 11.04.2012
*/
#include "controller.h"
#include <fstream>


Controller::Controller()
{
	
	//TODO: evtl. KI Initialisieren
	//TODO: Trainingsdaten von Datei laden
	inFileSensors = fopen ("training.txt", "a+b");
}

Controller::~Controller()
{
	//TODO: Trainingsdaten in Datei abspeichern
	fclose(inFileSensors);
	
}

float Controller::accelControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code einfügen, der die Beschleunigung des Autos regelt
	float accel = 1.0f; //Für einen ersten test hier einfach auf "Vollgas" gesetzt...
	return accel;
}

float Controller::brakeControl(CarState* cs, CarControl* cc)
{
	//TODO: hier code einfügen, der das Auto abbremst, z.B. in Kurven...
	float brake = 0.0f;
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
	return steer;
}


float Controller::clutchControl(CarState* cs, CarControl* cc)
{
	//TODO: Kupplung implementieren (nicht unbedingt nötig)
	float clutch =0.0f;
	return clutch;
}

int Controller::gearControl(CarState* cs, CarControl* cc)
{
	//TODO: Gangschaltung implementieren
	// mögliche Werte sind -1 (Rückwärtsgang), 0 (Leerlauf), 1, 2, 3, 4, 5, 6
	int gear = 1; //hier in diesem Beispiel einfach immer 1 Gang verwenden
	return gear;
}

void Controller::generateVector(CarState* cs, CarControl* cc)
{
	//Instanz des Structs und speichern der Sensorwerte
	struct sensors *temp;
	temp->angle = cs->getAngle();
	temp->gear = cs->getGear();
	temp->rpm = cs->getRpm();
	temp->speedX =  cs->getSpeedX();
	temp->speedY = cs->getSpeedY();
	
	//Speichern der Sensordaten in der traning.txt
	fwrite(temp, sizeof(struct sensors), 1, inFileSensors);
	cout << "test";
	
	//TODO: KI Trainieren
	//TODO: Ausgabe durch KI berechnen
	cc->setAccel(accelControl(cs, cc));
	cc->setBrake(brakeControl(cs, cc));
	cc->setSteer(steerControl(cs, cc));
	cc->setClutch(clutchControl(cs, cc));
	cc->setGear(gearControl(cs, cc));
}