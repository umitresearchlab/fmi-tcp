/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 25, 2013
 *
 */

#include "fmiserver.h"
#include "../../commands.h"
#include "../../helper.h"

FMICoSimulationServer* createFMICoSimulationServer(char hostName[PATH_MAX], long port, int numFMUS, double tStart, double stepSize,
    double tStop, enum METHOD method, int numConnections, connection connections[MAX_CONNECTIONS]) {
  FMICoSimulationServer* FMICSServer = malloc(sizeof(FMICoSimulationServer));
  FMICSServer->numFMUS = numFMUS;
  FMICSServer->numClients = 0;
  FMICSServer->tStart = tStart;
  FMICSServer->stepSize = stepSize;
  FMICSServer->tStop = tStop;
  FMICSServer->method = method;
  FMICSServer->numConnections = numConnections;
  int i;
  for (i = 0 ; i < numConnections ; i++) {
    FMICSServer->connections[i] = connections[i];
    FMICSServer->connections[i].state = connectionInvalid;
  }
  /* server */
  FMICSServer->pump = lw_eventpump_new();
  FMICSServer->server = lw_server_new(FMICSServer->pump);
  /* save this object in the server tag so we can use it later on. */
  lw_server_set_tag(FMICSServer->server, (void*)FMICSServer);
  /* connect the hooks */
  lw_server_on_connect(FMICSServer->server, serverOnConnect);
  lw_server_on_data(FMICSServer->server, serverOnData);
  lw_server_on_disconnect(FMICSServer->server, serverOnDisconnect);
  lw_server_on_error(FMICSServer->server, serverOnError);
  /* setup the server host name and port */
  lw_addr host = lw_addr_new_port(hostName, port);
  lw_filter filter = lw_filter_new();
  lw_filter_set_ipv6(filter, lw_false);
  lw_filter_set_local(filter, host);
  /* host/start the server */
  lw_server_host_filter(FMICSServer->server, filter);
  lw_filter_delete(filter);
  /* start the eventloop */
  lw_eventpump_start_eventloop(FMICSServer->pump);
/*fprintf(stderr, "closing server");fflush(NULL);
  lw_server_delete (server);
  lw_pump_delete (pump);*/
  return FMICSServer;
}

void sendCommand(lw_client client, int index, char* data, size_t size) {
  debugPrint(debugFlag, stderr, "server sending to client=%d, data=%.*s\n", index, size, data);fflush(NULL);
  char cmd[size+1];
  strcpy(cmd, data);
  strcat(cmd, "\n");
  lw_stream_write(client, cmd, size+1);
}

int findClientIndex(FMICoSimulationServer *FMICSServer, lw_client client) {
  int index = 0;
  lw_server_client client1 = lw_server_client_first(FMICSServer->server);
  while (client1) {
    if (client1 == client)
      break;
    if (index + 1 < FMICSServer->numClients)
      client1 = lw_server_client_next(client1);
    else
      break;
    index++;
  }
  return index;
}

int hasAllClientsState(FMICoSimulationServer *FMICSServer, CLIENTSTATE state) {
  lw_server_client client = lw_server_client_first(FMICSServer->server);
  int count = 1;
  while (client) {
    FMIClientInfo *clientInfo = (FMIClientInfo*)lw_stream_tag(client);
    if (clientInfo->state != state)
      return 0;
    if (count < FMICSServer->numClients)
      client = lw_server_client_next(client);
    else
      break;
    count++;
  }
  return 1;
}

lw_server_client findClientByIndex(FMICoSimulationServer *FMICSServer, int index) {
  lw_server_client client = lw_server_client_first(FMICSServer->server);
  int count = 1;
  while (client) {
    if (count - 1 == index)
      return client;
    if (count < FMICSServer->numClients)
      client = lw_server_client_next(client);
    else
      break;
    count++;
  }
  return 0;
}

void callfmi1DoStepJacobi(FMICoSimulationServer *FMICSServer) {
  /* Before calling fmiDostep check if all connections of all slaves are fulfilled.*/
  int i;
  int found = 0;
  for (i = 0 ; i < FMICSServer->numConnections ; i++) {
    if (connectionComplete != FMICSServer->connections[i].state) {
      found = 1;
      connection c = FMICSServer->connections[i];
      logPrint(stdout, "Can't do fmiDoStep(), still waiting for pending connection to complete, connection %d,%d,%d,%d\n", c.fromFMU, c.fromOutputVR, c.toFMU, c.toInputVR);fflush(NULL);
    }
  }
  if (!found) {
    /* set all connection states to invalid again for the next step. */
    for (i = 0 ; i < FMICSServer->numConnections ; i++) {
      FMICSServer->connections[i].state = connectionInvalid;
    }
    /* call fmiDoStep for all clients */
    lw_server_client client = lw_server_client_first(FMICSServer->server);
    int count = 1;
    while (client) {
      sendCommand(client, count - 1, fmiDoStep, strlen(fmiDoStep));
      if (count < FMICSServer->numClients)
        client = lw_server_client_next(client);
      else
        break;
      count++;
    }
  }
}

void callfmi1DoStepGaussSeidel(FMICoSimulationServer *FMICSServer, lw_server_client client) {
  /* Before calling fmiDostep check if all connections of this slave are fulfilled.*/
  int i;
  int isConnectionPending = 0;
  int clientIndex = findClientIndex(FMICSServer, client);
  for (i = 0 ; i < FMICSServer->numConnections ; i++) {
    connection c = FMICSServer->connections[i];
    if (clientIndex == c.toFMU && connectionComplete != c.state) {
      isConnectionPending = 1;
      logPrint(stdout, "Can't do fmiDoStep(), still waiting for pending connection to complete, connection %d,%d,%d,%d\n", c.fromFMU, c.fromOutputVR, c.toFMU, c.toInputVR);fflush(NULL);
    }
  }
  if (!isConnectionPending) {
    /* set all connection states to invalid again for the next step. */
    for (i = 0 ; i < FMICSServer->numConnections ; i++) {
      if (clientIndex == FMICSServer->connections[i].toFMU) {
        FMICSServer->connections[i].state = connectionInvalid;
      }
    }
    /* call fmiDoStep */
    sendCommand(client, clientIndex, fmiDoStep, strlen(fmiDoStep));
  }
}

void serverOnConnect(lw_server server, lw_client client)
{
  debugPrint(debugFlag, stderr, "server on_connect \n");fflush(NULL);
  FMICoSimulationServer *FMICSServer = (FMICoSimulationServer*)lw_server_tag(server);
  FMICSServer->numClients += 1;
  int index = FMICSServer->numClients - 1;
  debugPrint(debugFlag, stderr, "FMICSServer->numClients = %d \n", FMICSServer->numClients);fflush(NULL);

  FMIClientInfo *clientInfo = malloc(sizeof(FMIClientInfo));
  clientInfo->state = stateNone;
  lw_stream_set_tag(client, (void*)clientInfo);

  if (FMICSServer->numClients < FMICSServer->numFMUS) {
    char cmd[50];
    sprintf(cmd, waitingForFMUs, FMICSServer->numFMUS - FMICSServer->numClients);
    sendCommand(client, index, cmd, strlen(cmd));
  } else {
    char tStartCmd[50];
    sprintf(tStartCmd, "%s%f", fmiTStart, FMICSServer->tStart);
    char stepSizeCmd[50];
    sprintf(stepSizeCmd, "%s%f", fmiStepSize, FMICSServer->stepSize);
    char tEndCmd[50];
    sprintf(tEndCmd, "%s%f", fmiTEnd, FMICSServer->tStop);
    int i;
    /* start simulation for the first client. */
    if (FMICSServer->numClients > 1) {
      lw_server_client client1 = lw_server_client_first(server);
      sendCommand(client1, 0, tStartCmd, strlen(tStartCmd));
      sendCommand(client1, 0, stepSizeCmd, strlen(stepSizeCmd));
      sendCommand(client1, 0, tEndCmd, strlen(tEndCmd));
      /* start simulations for the n-1 clients. */
      for (i = 2 ;  i < FMICSServer->numClients ; i++) {
        /* fetch the next client */
        client1 = lw_server_client_next(client1);
        sendCommand(client1, i - 1, tStartCmd, strlen(tStartCmd));
        sendCommand(client1, i - 1, stepSizeCmd, strlen(stepSizeCmd));
        sendCommand(client1, i - 1, tEndCmd, strlen(tEndCmd));
      }
    }
    /* start simulation for the last client. */
    sendCommand(client, FMICSServer->numClients - 1, tStartCmd, strlen(tStartCmd));
    sendCommand(client, FMICSServer->numClients - 1, stepSizeCmd, strlen(stepSizeCmd));
    sendCommand(client, FMICSServer->numClients - 1, tEndCmd, strlen(tEndCmd));
  }
}

void serverOnData(lw_server server, lw_client client, const char* data, size_t size)
{
  FMICoSimulationServer *FMICSServer = (FMICoSimulationServer*)lw_server_tag(server);
  int clientIndex = findClientIndex(FMICSServer, client);
  FMIClientInfo *clientInfo = (FMIClientInfo*)lw_stream_tag(client);

  char* response = (char*)malloc(size+1);
  strncpy(response, data, size);
  response[size] = '\0';
  debugPrint(debugFlag, stderr, "server received from client=%d, Data=%s\n", clientIndex, response);fflush(NULL);

  char* token;
  token = strtok(response, "\n");
  while (token != NULL)
  {
    debugPrint(debugFlag, stdout, "token = %s\n", token);fflush(NULL);
    if (strncmp(token, fmiTEndOk, strlen(fmiTEndOk)) == 0) {
      sendCommand(client, clientIndex, fmiInstantiateSlave, strlen(fmiInstantiateSlave));
    } else if ((strncmp(token, fmiInstantiateSlaveSuccess, strlen(fmiInstantiateSlaveSuccess)) == 0) ||
              (strncmp(token, fmiInstantiateSlaveWarning, strlen(fmiInstantiateSlaveWarning)) == 0)) {
      /*
       * handle fmiInstantiateSlave response.
       * if the response is success then call fmiInitializeSlave
       */
      logPrint(stdout, "Slave %d has successfully done Instantiation.\n", clientIndex);fflush(NULL);
      sendCommand(client, clientIndex, fmiInitializeSlave, strlen(fmiInitializeSlave));
    } else if (strncmp(token, fmiInstantiateSlaveError, strlen(fmiInitializeSlaveError)) == 0) {
      logPrint(stderr,"Could not instantiate model.\n");fflush(NULL);
      exit(EXIT_FAILURE);
    } else if (strncmp(token, fmiInitializeSlaveOk, strlen(fmiInitializeSlaveOk)) == 0) {
      /*
       * handle fmiInitializeSlave response.
       * if the response is OK then call setInitialValues
       */
      logPrint(stdout, "Slave %d has successfully done Initialization.\n", clientIndex);fflush(NULL);
      sendCommand(client, clientIndex, setInitialValues, strlen(setInitialValues));
    } else if (strncmp(token, fmiInitializeSlaveError, strlen(fmiInitializeSlaveError)) == 0) {
      logPrint(stderr,"Could not initialize model.\n");fflush(NULL);
      exit(EXIT_FAILURE);
    } else if ((strncmp(token, setInitialValuesOk, strlen(setInitialValuesOk)) == 0) ||
        (strncmp(token, fmiSetValueReturn, strlen(fmiSetValueReturn)) == 0) ||
        (strncmp(token, fmiDoStepOk, strlen(fmiDoStepOk)) == 0)) {
      /* if fmiDoStepOK is returned then just inform user about successful step. */
      if (strncmp(token, fmiDoStepOk, strlen(fmiDoStepOk)) == 0) {
        logPrint(stdout, "Slave %d has successfully taken a step.\n", clientIndex);fflush(NULL);
      }
      /* if no connections just call fmiDoStep */
      if (FMICSServer->numConnections == 0) {
        sendCommand(client, clientIndex, fmiDoStep, strlen(fmiDoStep));
      } else {
        int clientsInitialized = 1;
        if (strncmp(token, setInitialValuesOk, strlen(setInitialValuesOk)) == 0) {
          clientInfo->state = stateInitialized;
          clientsInitialized = hasAllClientsState(FMICSServer, stateInitialized);
        }
        lw_server_client serverClient;
        switch (FMICSServer->method) {
        case jacobi:  /* parallel */
          if (clientsInitialized) {
            /* check the client's connections */
            int i;
            int found = 0;
            for (i = 0 ; i < FMICSServer->numConnections ; i++) {
              connection c = FMICSServer->connections[i];
              if (c.state == connectionInvalid) {
                lw_server_client fromClient = findClientByIndex(FMICSServer, c.fromFMU);
                if (fromClient) {     /* it might be that the client we want here is already finished. */
                  found = 1;
                  c.state = connectionRequested;
                  FMICSServer->connections[i] = c;
                  char cmd[50];
                  sprintf(cmd, "%s%d", fmiGetValue, c.fromOutputVR);
                  sendCommand(fromClient, c.fromFMU, cmd, strlen(cmd));
                  break;
                }
              }
            }
            /* if not found then we assume we have fulfilled all the connections of this client so we can now call fmiDoStep. */
            if (!found) {
              callfmi1DoStepJacobi(FMICSServer);
            }
          }
          break;
        case gs:      /* Sequential */
          if (clientsInitialized) {
            /* loop through all the clients */
            serverClient = lw_server_client_first(FMICSServer->server);
            int count = 1;
            while (serverClient) {
              FMIClientInfo *serverClientInfo = (FMIClientInfo*)lw_stream_tag(serverClient);
              serverClientInfo->state = stateStepping;
              int i;
              int found = 0;
              for (i = 0 ; i < FMICSServer->numConnections ; i++) {
                connection c = FMICSServer->connections[i];
                if (findClientIndex(FMICSServer, serverClient) == c.toFMU && c.state == connectionInvalid) {
                  lw_server_client fromClient = findClientByIndex(FMICSServer, c.fromFMU);
                  if (fromClient) {     /* it might be that the client we want here is already finished. */
                    found = 1;
                    c.state = connectionRequested;
                    FMICSServer->connections[i] = c;
                    char cmd[50];
                    sprintf(cmd, "%s%d", fmiGetValue, c.fromOutputVR);
                    sendCommand(fromClient, c.fromFMU, cmd, strlen(cmd));
                  }
                }
              }
              /* if not found then this client doesn't have any connection so just call fmiDoStep for it. */
              if (!found) {
                callfmi1DoStepGaussSeidel(FMICSServer, serverClient);
              }
              if (count < FMICSServer->numClients)
                serverClient = lw_server_client_next(serverClient);
              else
                break;
              count++;
            }
          }
          break;
        }
      }
    } else if (strncmp(token, fmiGetValueReturn, strlen(fmiGetValueReturn)) == 0) {
      double value = unparseDoubleResult(token, fmiGetValueReturn, strlen(token));
      int i;
      for (i = 0 ; i < FMICSServer->numConnections ; i++) {
        connection c = FMICSServer->connections[i];
        if (clientIndex == c.fromFMU && c.state == connectionRequested) {
          lw_server_client toClient = findClientByIndex(FMICSServer, c.toFMU);
          c.state = connectionComplete;
          FMICSServer->connections[i] = c;
          char cmd[100];
          sprintf(cmd, "%s%d#%s%f", fmiSetValueVr, c.toInputVR, fmiSetValue, value);
          sendCommand(toClient, c.toFMU, cmd, strlen(cmd));
          break;
        }
      }
    } else if (strncmp(token, fmiDoStepPending, strlen(fmiDoStepPending)) == 0) {
      /*
       * We must start a timer here which should ask the slave about fmiGetStatus.
       * No need to free the timer object. It is single shot timer and will automatically call the destroy after 5 seconds.
       */
      FMICoSimulationTimer *FMICSTimer = createFMICoSimulationTimer(client, FMICSServer);
    } else if (strncmp(token, fmiDoStepError, strlen(fmiDoStepError)) == 0) {
      logPrint(stderr,"doStep() of FMU didn't return fmiOK! Exiting...\n");fflush(NULL);
      exit(EXIT_FAILURE);
    } else if (strncmp(token, fmiDoStepFinished, strlen(fmiDoStepFinished)) == 0) {
      clientInfo->state = stateSteppingFinished;
      switch (FMICSServer->method) {
      case jacobi:  /* parallel */
        sendCommand(client, clientIndex, fmiTerminateSlave, strlen(fmiTerminateSlave));
        break;
      case gs:
        if (hasAllClientsState(FMICSServer, stateSteppingFinished)) {
          /* loop through all the clients and call fmiTerminate */
          lw_server_client serverClient = lw_server_client_first(FMICSServer->server);
          int count = 1;
          while (serverClient) {
            sendCommand(serverClient, findClientIndex(FMICSServer, serverClient), fmiTerminateSlave, strlen(fmiTerminateSlave));
            if (count < FMICSServer->numClients)
              serverClient = lw_server_client_next(serverClient);
            else
              break;
            count++;
          }
        }
        break;
      }
    }
    token = strtok(NULL, "\n");
  }
  free(response);
}

void serverOnDisconnect(lw_server server, lw_client client) {
  logPrint(stdout, "Client disconnected\n");fflush(NULL);
  FMICoSimulationServer *FMICSServer = (FMICoSimulationServer*)lw_server_tag(server);
  FMICSServer->numClients -= 1;
}

void serverOnError(lw_server server, lw_error error) {
  const char* errorString = lw_error_tostring(error);
  logPrint(stderr, "Client received an error \"%s\"\n", errorString);fflush(NULL);
  lw_server_delete(server);
}

/* timer methods */
FMICoSimulationTimer* createFMICoSimulationTimer(lw_client client, FMICoSimulationServer *FMICSServer) {
  FMICoSimulationTimer *FMICSTimer = malloc(sizeof(FMICoSimulationTimer));
  FMICSTimer->client = client;
  FMICSTimer->FMICSServer = FMICSServer;

  FMICSTimer->timer = lw_timer_new(FMICSTimer->FMICSServer->pump);
  lw_timer_on_tick(FMICSTimer->timer, timerOnTick);
  /* save this object in the timer tag so we can use it later on. */
  lw_timer_set_tag(FMICSTimer->timer, (void*)FMICSTimer);
  /*
   * start the timer.
   * call timerOnTick after every 5 seconds.
   */
  lw_timer_start(FMICSTimer->timer, 5000);
  return FMICSTimer;
}

void destroyFMICoSimulationTimer(FMICoSimulationTimer *FMICSTimer) {
  lw_timer_delete(FMICSTimer->timer);
  free(FMICSTimer);
}

void timerOnTick(lw_timer timer) {
  FMICoSimulationTimer *FMICSTimer = (FMICoSimulationTimer*)lw_timer_tag(timer);
  sendCommand(FMICSTimer->client, findClientIndex(FMICSTimer->FMICSServer, FMICSTimer->client), fmiDoStepStatus, strlen(fmiDoStepStatus));
  destroyFMICoSimulationTimer(FMICSTimer);
}
