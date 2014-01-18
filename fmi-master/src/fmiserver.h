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
  enum METHOD method;
  int numConnections;
  connection connections[MAX_CONNECTIONS];
  lw_pump pump;
  lw_server server;
} FMICoSimulationServer;

typedef enum CLIENTSTATE {
  stateNone,
  stateInitialized
} CLIENTSTATE;

typedef struct {
  enum CLIENTSTATE state;
} FMIClientInfo;

FMICoSimulationServer* createFMICoSimulationServer(char hostName[PATH_MAX], long port, int numFMUS, double tStart, double stepSize,
    double tStop, enum METHOD method, int numConnections, connection connections[MAX_CONNECTIONS]);

void sendCommand(lw_client client, int index, char* data, size_t size);
int findClientIndex(FMICoSimulationServer *FMICSServer, lw_client client);
lw_server_client findClientByIndex(FMICoSimulationServer *FMICSServer, int index);
int allClientsInitialized(FMICoSimulationServer *FMICSServer);
void callfmi1DoStepJacobi(FMICoSimulationServer *FMICSServer);
void callfmi1DoStepGaussSeidel(FMICoSimulationServer *FMICSServer, int clientIndex);

void serverOnConnect(lw_server server, lw_client client);
void serverOnData(lw_server server, lw_client client, const char * data, size_t size);
void serverOnDisconnect(lw_server server, lw_client client);
void serverOnError(lw_server server, lw_error error);

/*
 * A single shot timer to check the fmiDoStep status.
 */
typedef struct {
  lw_client client;
  FMICoSimulationServer *FMICSServer;
  lw_timer timer;
} FMICoSimulationTimer;

FMICoSimulationTimer* createFMICoSimulationTimer(lw_client client, FMICoSimulationServer *FMICSServer);
void destroyFMICoSimulationTimer(FMICoSimulationTimer *FMICSTimer);

void timerOnTick(lw_timer timer);

#endif /* FMISERVER_H_ */
