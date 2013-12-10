/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 25, 2013
 *
 */

#ifndef FMISERVER_H_
#define FMISERVER_H_

#include "main.h"

typedef struct {
  int numFMUS;
  int numClients;
  double tStart;
  double stepSize;
  double tStop;
  int numConnections;
  connection connections[MAX_CONNECTIONS];
  lw_pump pump;
  lw_server server;
} FMICoSimulationServer;

FMICoSimulationServer* createFMICoSimulationServer(int numFMUS, double tStart, double stepSize, double tStop, int numConnections,
    connection connections[MAX_CONNECTIONS]);

void sendCommand(lw_client client, int index, char* data, size_t size);
int findClientIndex(FMICoSimulationServer *FMICSServer, lw_client client);
lw_server_client findClientByIndex(FMICoSimulationServer *FMICSServer, int index);
void callfmi1DoStep(FMICoSimulationServer *FMICSServer);

void serverOnConnect(lw_server server, lw_client client);
void serverOnData(lw_server server, lw_client client, const char * data, size_t size);
void serverOnDisconnect(lw_server server, lw_client client);
void serverOnError(lw_server server, lw_error error);

#endif /* FMISERVER_H_ */
