/***************************************************************************

file                 : client.cpp
copyright            : (C) 2007 Daniele Loiacono

 ***************************************************************************/

/***************************************************************************
 * Modifications:                                                          *
 * DATE         AUTHOR          CHANGES                                    *
 * ------------------------------------------------------------------------*
 * 2011-10-29	Stefan Moebius	Changes to the orginal are marked          *
 with //EDIT DATE                           *
 * 2012-04-13   Robin Lehmann   Some small changes, not really marked      *
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <map>
#include <vector>
#include <string>
#include <fstream>

#include "controller.h"
#include "SimpleDriver.h"

/*** defines for UDP *****/
#define UDP_MSGLEN 1000
#define UDP_CLIENT_TIMEUOT 1000000
//#define __UDP_CLIENT_VERBOSE__
/************************/

using namespace std;

void parse_args(int argc, char *argv[], char *hostName, unsigned int &serverPort, char *id, unsigned int &maxEpisodes,
        unsigned int &maxSteps, char *trackName, BaseDriver::tstage &stage);

int startClient(int argc, char* argv[], Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path, bool manual, bool logging);

int main(int argc, char *argv[]) {
    Controller* cntrl = NULL;
    int input;

    do {
        cout << "   Option Menu" << endl;
        cout << "1. Start manual control with logging" << endl;
        cout << "2. Start automatic control" << endl;
        cout << "0. to quit." << endl;

        cin >> input;

        if (input == 1) {
            string logfile;
            std::cout << "Logfile path? data/";
            std::cin >> logfile;
            // Manueller Modus
            startClient(argc, argv, NULL, "data/" + logfile, NULL, true, true);
        } else if (input == 2) {
            // Lazy initialization für Controller
            if (cntrl == NULL)
                cntrl = new Controller();

            // Automatischer Modus
            startClient(argc, argv, cntrl, "", NULL, false, false);
        }
    } while (input != 0);

    delete cntrl;
}

// Im restlichen Teil des Codes wird der Datentransfer zwischen dem TORCs Server und dem Client hier geregelt.
// Außerdem wird eine Driver Instanz d angelegt. An diese werden dann die Daten vom Server weitergeleitet und dessen Controlanweisungen werden an den Server gesandt.

int startClient(int argc, char *argv[], Controller* cntrl, string logfile, vector<pair<CarState, CarControl> >* path = NULL, bool manual = false, bool logging = false)
{
    int socketDescriptor;
    int numRead;

    char hostName[1000];
    unsigned int serverPort;
    char id[1000];
    unsigned int maxEpisodes;
    unsigned int maxSteps;
    char trackName[1000];
    BaseDriver::tstage stage;

    struct sockaddr_in serverAddress;
    struct hostent *hostInfo;
    struct timeval timeVal;
    fd_set readSet;
    char buf[UDP_MSGLEN];


    parse_args(argc,argv,hostName,serverPort,id,maxEpisodes,maxSteps,trackName,stage);

    hostInfo = gethostbyname(hostName);
    if (hostInfo == NULL)
    {
        cout << "Error: problem interpreting host: " << hostName << "\n";
        exit(1);
    }

    // Print command line option used
    cout << "***********************************" << endl;
    cout << "HOST: "   << hostName    << endl;
    cout << "PORT: " << serverPort  << endl;
    cout << "ID: "   << id     << endl;
    cout << "MAX_STEPS: " << maxSteps << endl;
    cout << "MAX_EPISODES: " << maxEpisodes << endl;
    cout << "TRACKNAME: " << trackName << endl;

    if (stage == BaseDriver::WARMUP)
        cout << "STAGE: WARMUP" << endl;
    else if (stage == BaseDriver::QUALIFYING)
        cout << "STAGE:QUALIFYING" << endl;
    else if (stage == BaseDriver::RACE)
        cout << "STAGE: RACE" << endl;
    else
        cout << "STAGE: UNKNOWN" << endl;

    cout << "***********************************" << endl;

    // Create a socket (UDP on IPv4 protocol)
    socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketDescriptor < 0)
    {
        cerr << "cannot create socket\n";
        exit(1);
    }

    // Set some fields in the serverAddress structure.
    serverAddress.sin_family = hostInfo->h_addrtype;
    memcpy((char *) &serverAddress.sin_addr.s_addr,
            hostInfo->h_addr_list[0], hostInfo->h_length);
    serverAddress.sin_port = htons(serverPort);

    SimpleDriver d(cntrl, logfile, path, manual, logging);
    strcpy(d.trackName,trackName);
    d.stage = stage;

    bool shutdownClient=false;
    unsigned long curEpisode=0;
    do
    {
        /***********************************************************************************
         ************************* UDP client identification ********************************
         ***********************************************************************************/
        do
        {
            // Initialize the angles of rangefinders
            float angles[19];
            d.init(angles);
            string initString = SimpleParser::stringify(string("init"),angles,19);
            cout << "Sending id to server: " << id << endl;
            initString.insert(0,id);
            cout << "Sending init string to the server: " << initString << endl;
            if (sendto(socketDescriptor, initString.c_str(), initString.length(), 0,
                        (struct sockaddr *) &serverAddress,
                        sizeof(serverAddress)) < 0)
            {
                cerr << "cannot send data ";
                close(socketDescriptor);
                exit(1);
            }

            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;

            if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal))
            {
                // Read data sent by the solorace server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server...";
                }
                else
                {
                    cout << "Received: " << buf << endl;

                    if (strcmp(buf,"***identified***")==0)
                        break;
                }
            }

        } while(1);

        unsigned long currentStep=0;

        do
        {
            // wait until answer comes back, for up to UDP_CLIENT_TIMEUOT micro sec
            FD_ZERO(&readSet);
            FD_SET(socketDescriptor, &readSet);
            timeVal.tv_sec = 0;
            timeVal.tv_usec = UDP_CLIENT_TIMEUOT;

            if (select(socketDescriptor+1, &readSet, NULL, NULL, &timeVal))
            {
                // Read data sent by the solorace server
                memset(buf, 0x0, UDP_MSGLEN);  // Zero out the buffer.
                numRead = recv(socketDescriptor, buf, UDP_MSGLEN, 0);
                if (numRead < 0)
                {
                    cerr << "didn't get response from server?";
                    close(socketDescriptor);
                    exit(1);
                }

#ifdef __UDP_CLIENT_VERBOSE__
                cout << "Received: " << buf << endl;
#endif

                if (strcmp(buf,"***shutdown***")==0)
                {
                    d.onShutdown();
                    shutdownClient = true;
                    cout << "Client Shutdown" << endl;
                    break;
                }

                if (strcmp(buf,"***restart***")==0)
                {
                    d.onRestart();
                    cout << "Client Restart" << endl;
                    break;
                }
                /**************************************************
                 * Compute The Action to send to the solorace sever
                 **************************************************/

                if ( (++currentStep) != maxSteps)
                {
                    string action = d.drive(string(buf));
                    memset(buf, 0x0, UDP_MSGLEN);
                    sprintf(buf,"%s",action.c_str());
                }
                else
                    sprintf (buf, "(meta 1)");

                if (sendto(socketDescriptor, buf, strlen(buf)+1, 0,
                            (struct sockaddr *) &serverAddress,
                            sizeof(serverAddress)) < 0)
                {
                    cerr << "cannot send data ";
                    close(socketDescriptor);
                    exit(1);
                }
#ifdef __UDP_CLIENT_VERBOSE__
                else
                    cout << "Sending " << buf << endl;
#endif
            }
            else
            {
                cout << "** Server did not respond in 1 second.\n";
            }
        } while(1);
    } while(shutdownClient==false && ( (++curEpisode) != maxEpisodes) );

    if(shutdownClient == false)
        d.onShutdown();

    close(socketDescriptor);

    return 0;
}

//void parse_args(int argc, char *argv[], char *hostName, unsigned int &serverPort, char *id, unsigned int &maxEpisodes,
//		  unsigned int &maxSteps,bool &noise, double &noiseAVG, double &noiseSTD, long &seed, char *trackName, BaseDriver::tstage &stage)
void parse_args(int argc, char *argv[], char *hostName, unsigned int &serverPort, char *id, unsigned int &maxEpisodes,
        unsigned int &maxSteps, char *trackName, BaseDriver::tstage &stage)
{
    int		i;

    // Set default values
    maxEpisodes=0;
    maxSteps=0;
    serverPort=3001;
    strcpy(hostName,"localhost");
    strcpy(id,"championship2011");
    //    noise=false;
    //    noiseAVG=0;
    //    noiseSTD=0.05;
    //    seed=0;
    strcpy(trackName,"unknown");
    stage=BaseDriver::UNKNOWN;


    i = 1;
    while (i < argc)
    {
        if (strncmp(argv[i], "host:", 5) == 0)
        {
            sprintf(hostName, "%s", argv[i]+5);
            i++;
        }
        else if (strncmp(argv[i], "port:", 5) == 0)
        {
            sscanf(argv[i],"port:%d",&serverPort);
            i++;
        }
        else if (strncmp(argv[i], "id:", 3) == 0)
        {
            sprintf(id, "%s", argv[i]+3);
            i++;
        }
        else if (strncmp(argv[i], "maxEpisodes:", 12) == 0)
        {
            sscanf(argv[i],"maxEpisodes:%ud",&maxEpisodes);
            i++;
        }
        else if (strncmp(argv[i], "maxSteps:", 9) == 0)
        {
            sscanf(argv[i],"maxSteps:%ud",&maxSteps);
            i++;
        }
        //    	else if (strncmp(argv[i], "seed:", 5) == 0)
        //    	{
        //    	    	sscanf(argv[i],"seed:%ld",&seed);
        //    	    	i++;
        //    	}
        else if (strncmp(argv[i], "track:", 6) == 0)
        {
            sscanf(argv[i],"track:%s",trackName);
            i++;
        }
        else if (strncmp(argv[i], "stage:", 6) == 0)
        {
            int temp;
            sscanf(argv[i],"stage:%d",&temp);
            stage = (BaseDriver::tstage) temp;
            i++;
            if (stage<BaseDriver::WARMUP || stage > BaseDriver::RACE)
                stage = BaseDriver::UNKNOWN;
        }
        else {
            i++;		/* ignore bad args */
        }
    }
}
