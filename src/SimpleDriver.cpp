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


// Constructor sets the basic configuration for the Driver
SimpleDriver::SimpleDriver(Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path = NULL, bool manual = false, bool logging = false)
{
    // Es werden die Übergabeparameter in der Instanz gespeichert und entsprechend Keyboard und Log initialisiert, falls gewünscht.
    this->automatedControl = cntrl;
    this->logfile = logfile;
    this->manual = manual;
    if(manual)
        Keyboard_Init();
    this->logging = logging;
    if(logging)
        this->log = new vector<pair<CarState, CarControl> >;
    this->path = path;
    this->position = 0;
}

string SimpleDriver::drive(string sensors)
{
    //Standardwert setzen
    float accel = 0.0f; //Beschleunigung
    float brake = 0.0f; // Bremsen
    float steer = 0.0f; //Lenkrichtung (positiv links, negativ rechts)
    float clutch = 0.0f; //Kupplung
    //	int gear = 0; //Gang (-1 für Rückwärtsgang)

    CarState cs(sensors);

    // Der CarControl Vektor der an den Server gesandt wird heisst nextVector. Der Gang wird von der aktuellen Beobachtung übernommen.
    // Ohne die Übernahme weiß die manuelle Kontrolle nicht wohin sie rauf- oder runterschalten soll.
    CarControl nextVector = CarControl(accel, brake, cs.getGear(), steer, clutch);

    // Bei manueller Steuerung wird der Vektor von der manuellen Kontrolle verändert
    if(manual)
    {
        Keyboard_Update(&nextVector);
    }

    // Nun erstellen wir einen CarControl Vektor indem die Ergebnisse der automatischen Verabeitung gespeichert werden sollen.
    // Falls nur manuel gefahren wird braucht man diesen Vektor nicht. Es werden die Werte eingetragen, die von der manuellen Steuerung verändert wurden.
    CarControl automatedVector = CarControl(nextVector.getAccel(), nextVector.getBrake(), nextVector.getGear(), nextVector.getSteer(), nextVector.getClutch());

    // path == NULL überprüft ob ein vorhandener Pfad nachgefahren werden soll. Falls das so ist, braucht die automatedControl nicht ausgeführt zu werden.
    if(path == NULL && automatedControl != NULL)
    {
        // Es wird die Funktion generateVector an den Controller übergeben, der bei der Erstellung dieser Instanz angegeben wurde.
        // Die Ergebnisse landen dann in automatedVector.
        this->automatedControl->generateVector(&cs, &automatedVector);

        if(!manual)
        {
            nextVector = automatedVector;
        }
    }
    else if(!manual)
    {
        // Diese Routine setzt den nächsten Punkt aus dem vorgegebenen Pfad in den nextVector ein.
        if(position < (*path).size())
        {
            nextVector = (*path)[position].second;
            position++;
        }
        else
        {
            cout << "path has ended" << endl;
        }
    }

    // Für das Logging werden Sensor- und Aktorendaten an den log Vektor angehängt. Diese werden am Ende eines Rennens in die Datei logfile geschrieben.
    if(logging)
    {
        log->push_back(pair<CarState, CarControl>(cs, nextVector));
    }

    // Übergabe des nextVectors in Richtung Server.
    return nextVector.toString();
}

// Log schreiben und gegebenenfalls das Keyboardfenster entfernen
void SimpleDriver::onShutdown()
{
    if (manual)
    {
        Keyboard_Quit();
    }

    if(logging)
    {
        cout << "Writing log to harddrive" << endl;

        fstream logfile;
        logfile.open(this->logfile.c_str(),fstream::out);
        for(unsigned int i = 0; i < (*log).size(); i++)
        {
            logfile << (*log)[i].first.toString() << endl;
            logfile << (*log)[i].second.toString() << endl;
        }
        logfile.close();

        (*log).clear();
    }

    cout << "Bye bye!" << endl;
}

// Verteilung der Winkel für die Sensoren
void SimpleDriver::init(float *angles)
{
    for (int i=0; i<5; i++)
    {
        angles[i]=-90+i*15;
        angles[18-i]=90-i*15;
    }
    for (int i=5; i<9; i++)
    {
        angles[i]=-20+(i-5)*5;
        angles[18-i]=20-(i-5)*5;
    }
    angles[9]=0;

    return;
}

