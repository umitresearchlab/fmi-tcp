#include "Master.h"
#include "Logger.h"
#include "StrongConnection.h"
#include "WeakConnection.h"
#include "Slave.h"
#include "string.h"
#include "stdlib.h"
#include "commands.h"
#include "url_parser.h"


/*
 * @brief Parses the result like fmiStepSize=0.200000
 * @param data - represents the full result data.
 * @param name - represents the name in the result data e.g fmiStepSize=
 * @param size - the size of the data.
 *
 * Make a new string with size of the value. Copy the string where name= is finished until value is end.
 * Convert the string to double using atof. Free the temporary str before returning.
 *
 */
double unparseDoubleResult(const char* data, const char* name, long size) {
  int valueLength = strlen(name);
  char* str = (char*)malloc(size - valueLength);
  strncpy(str, data+valueLength, size - valueLength);
  double res = atof(str);
  free(str);
  return res;
}

int unparseIntResult(const char* data, const char* name, long size) {
  int valueLength = strlen(name);
  char* str = (char*)malloc(size - valueLength);
  strncpy(str, data+valueLength, size - valueLength);
  int res = atoi(str);
  free(str);
  return res;
}


Master::Master(){
    init();
}

Master::Master(const Logger& logger){
    m_logger = logger;
    init();
}

void Master::init(){
    // One pump for all clients
    m_pump = lw_eventpump_new();

    // Set state
    m_state = MASTER_IDLE;
    m_slaveIdCounter = 0;
}

Master::~Master(){
    lw_eventpump_post_eventloop_exit(m_pump);
    lw_pump_delete(m_pump);

    // Delete all connections
    for (int i = 0; i < m_strongConnections.size(); ++i){
        delete m_strongConnections[i];
    }
    for (int i = 0; i < m_weakConnections.size(); ++i){
        delete m_weakConnections[i];
    }

    // Delete all remaining slaves
    for (int i = 0; i < m_slaves.size(); ++i){
        delete m_slaves[i];
    }
}

void clientOnConnect(lw_client client) {
    Master * master = (Master*)lw_stream_tag(client);
    master->clientConnected(client);
}

void clientOnData(lw_client client, const char* data, long size) {
    Master * master = (Master*)lw_stream_tag(client);
    master->clientData(client,data,size);
}

void clientOnDisconnect(lw_client client) {
    Master * master = (Master*)lw_stream_tag(client);
    master->clientDisconnected(client);
}

void clientOnError(lw_client client, lw_error error) {
    const char* errorString = lw_error_tostring(error);
    lw_pump pump = lw_stream_pump(client);

    if (strcmp(errorString, "Error connecting") == 0) {
        lw_addr address = lw_client_server_addr(client);
        lw_stream_delete(client);
        return;
    }

    lw_stream_delete(client);
}

void Master::initializeSlaves(){
    m_state = MASTER_INITIALIZING;

    for (int i = 0; i < m_slaves.size(); ++i){

        // Start to initialize
        //m_slaves[i]->initialize();
    }
}

void Master::transferWeakConnectionData(){
    m_state = MASTER_TRANSFERRING_WEAK;

    // Assume parallel
    for (int i = 0; i < m_weakConnections.size(); ++i){

        // Start to transfer value
        m_weakConnections[i];
    }
}

int Master::connectSlave(const char uri[PATH_MAX]){

    struct parsed_url * url = parse_url(uri);
    long port = atoi(url->port);

    // Create new client
    lw_client client = lw_client_new(m_pump);

    // Set the master object as tag
    lw_stream_set_tag(client, (void*)this);

    // connect the event handlers
    lw_client_on_connect(   client, clientOnConnect);
    lw_client_on_data(      client, clientOnData);
    lw_client_on_disconnect(client, clientOnDisconnect);
    lw_client_on_error(     client, clientOnError);

    Slave * slave = new Slave(client);
    int slaveId = m_slaveIdCounter++;
    slave->setId(slaveId);
    m_slaves.push_back(slave);

    // connect the client to the server
    lw_client_connect(client, url->host, port);

    parsed_url_free(url);

    return slaveId;
}

void Master::simulate(){

    // start the eventloop
    lw_eventpump_start_eventloop(m_pump);
}

Slave * Master::getSlave(lw_client client){
    for(int i=0; i<m_slaves.size(); i++){
        if(m_slaves[i]->getClient() == client){
            return m_slaves[i];
        }
    }
    return NULL;
}

Slave * Master::getSlave(int id){
    for(int i=0; i<m_slaves.size(); i++){
        if(m_slaves[i]->getId() == id){
            return m_slaves[i];
        }
    }
    return NULL;
}

void Master::clientConnected(lw_client client){
    m_logger.log(NETWORK,"Client connected\n");
    Slave * slave = getSlave(client);
}

void Master::clientDisconnected(lw_client client){
    m_logger.log(NETWORK,"Client disconnected\n");

    // Remove from slave vector
    for(int i=0; i<m_slaves.size(); i++){
        if(m_slaves[i]->getClient() == client){
            delete m_slaves[i];
            m_slaves.erase(m_slaves.begin()+i);
            lw_stream_delete(client);
            break;
        }
    }

    // No slaves left - try exit
    if(m_slaves.size() == 0)
        lw_eventpump_post_eventloop_exit(m_pump);
}

void Master::clientData(lw_client client, const char* data, long size){
    m_logger.log(NETWORK,"Client data\n");
    Slave * slave = getSlave(client);

    char* response = (char*)malloc(size+1);
    strncpy(response, data, size);
    response[size] = '\0';
    //debugPrint(debugFlag, stderr, "<-- %d: %s\n", clientIndex, response);fflush(NULL);

    char* token;
    token = strtok(response, "\n");
    while (token != NULL) {
        /*debugPrint(debugFlag, stdout, "token = %s\n", token);fflush(NULL);*/
        if (strncmp(token, fmiTEndOk, strlen(fmiTEndOk)) == 0) {
          //sendCommand(client, clientIndex, fmiInstantiateSlave, strlen(fmiInstantiateSlave));
          slave->instantiate();
        } else if ((strncmp(token, fmiInstantiateSlaveSuccess, strlen(fmiInstantiateSlaveSuccess)) == 0) ||
                  (strncmp(token, fmiInstantiateSlaveWarning, strlen(fmiInstantiateSlaveWarning)) == 0)) {
          /*
           * handle fmiInstantiateSlave response.
           * if the response is success then call fmiInitializeSlave
           */
          m_logger.log(NETWORK,"Slave %d has successfully done Instantiation.\n", slave->getId());
          slave->initialize();

        } else if (strncmp(token, fmiInstantiateSlaveError, strlen(fmiInitializeSlaveError)) == 0) {
          m_logger.log(NETWORK,"Could not instantiate model.\n");
          exit(EXIT_FAILURE);

        } else if (strncmp(token, fmiInitializeSlaveOk, strlen(fmiInitializeSlaveOk)) == 0) {
          /*
           * handle fmiInitializeSlave response.
           * if the response is OK then call setInitialValues
           */
          m_logger.log(NETWORK,"Slave %d has successfully done Initialization.\n", slave->getId());
          slave->setInitialValues();

        } else if (strncmp(token, fmiInitializeSlaveError, strlen(fmiInitializeSlaveError)) == 0) {
          m_logger.log(NETWORK,"Could not initialize model.\n");
          exit(EXIT_FAILURE);

        } else if ((strncmp(token, setInitialValuesOk, strlen(setInitialValuesOk)) == 0) ||
            (strncmp(token, fmiSetValueReturn, strlen(fmiSetValueReturn)) == 0) ||
            (strncmp(token, fmiDoStepOk, strlen(fmiDoStepOk)) == 0)) {
          /* if fmiDoStepOK is returned then just inform user about successful step. */
          if (strncmp(token, fmiDoStepOk, strlen(fmiDoStepOk)) == 0) {
            m_logger.log(NETWORK,"Slave %d has successfully taken a step.\n", slave->getId());
          }

          // if no connections just call fmiDoStep
          if (m_weakConnections.size() == 0) {
            slave->doStep();

          } else {
            int allSlavesInitialized = 1;
            if (strncmp(token, setInitialValuesOk, strlen(setInitialValuesOk)) == 0) {
              slave->setState(SLAVE_INITIALIZED);
              allSlavesInitialized = hasAllClientsState(SLAVE_INITIALIZED);
            }
            lw_server_client serverClient;
            switch (m_method) {

            case PARALLEL:
              if (allSlavesInitialized) {
                // check the client's connections
                int i;
                int found = 0;
                for (i = 0 ; i < m_weakConnections.size() ; i++) {
                  WeakConnection * c = m_weakConnections[i];
                  if (c->getState() == CONNECTION_INVALID) {
                    //lw_server_client fromClient = findClientByIndex(FMICSServer, c.fromFMU);
                    //if (fromClient) {     // it might be that the client we want here is already finished.
                      found = 1;
                      c->setState(CONNECTION_REQUESTED);
                      //FMICSServer->connections[i] = c;
                      //char cmd[50];
                      //sprintf(cmd, "%s%d", fmiGetValue, c->getValueRefA());
                      slave->getReal(c->getValueRefA()); //sendCommand(fromClient, c.fromFMU, cmd, strlen(cmd));
                      break;
                    //}
                  }
                }
                // if not found then we assume we have fulfilled all the connections of this client so we can now call fmiDoStep.
                if (!found) {
                  slave->doStep();
                }
              }
              break;

              /*
            case gs:      // Sequential
              if (allSlavesInitialized) {
                // loop through all the clients
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
                      if (fromClient) {     // it might be that the client we want here is already finished.
                        found = 1;
                        c.state = connectionRequested;
                        FMICSServer->connections[i] = c;
                        char cmd[50];
                        sprintf(cmd, "%s%d", fmiGetValue, c.fromOutputVR);
                        sendCommand(fromClient, c.fromFMU, cmd, strlen(cmd));
                      }
                    }
                  }
                  // if not found then this client doesn't have any connection so just call fmiDoStep for it.
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
              */

            }
          }
        } else if (strncmp(token, fmiGetValueReturn, strlen(fmiGetValueReturn)) == 0) {
          double value = unparseDoubleResult(token, fmiGetValueReturn, strlen(token));
          int i;
          for (i = 0 ; i < m_weakConnections.size() ; i++) {
            WeakConnection * c = m_weakConnections[i];
            if (slave == c->getSlaveA() && c->getState() == CONNECTION_REQUESTED) {
              //lw_server_client toClient = findClientByIndex(FMICSServer, c.toFMU);
              c->setState(CONNECTION_COMPLETE);
              //FMICSServer->connections[i] = c;
              //char cmd[100];
              //sprintf(cmd, "%s%d#%s%f", fmiSetValueVr, c.toInputVR, fmiSetValue, value);
              slave->setReal(c->getValueRefB(),value); //sendCommand(toClient, c.toFMU, cmd, strlen(cmd));
              break;
            }
          }
        } else if (strncmp(token, fmiDoStepPending, strlen(fmiDoStepPending)) == 0) {
          /*
           * We must start a timer here which should ask the slave about fmiGetStatus.
           * No need to free the timer object. It is single shot timer and will automatically call the destroy after 5 seconds.
           */
          //FMICoSimulationTimer *FMICSTimer = createFMICoSimulationTimer(client, FMICSServer);
          //Todo: need to fix
        } else if (strncmp(token, fmiDoStepError, strlen(fmiDoStepError)) == 0) {
          //logPrint(stderr,"doStep() of FMU didn't return fmiOK! Exiting...\n");fflush(NULL);
          exit(EXIT_FAILURE);
        } else if (strncmp(token, fmiDoStepFinished, strlen(fmiDoStepFinished)) == 0) {
          slave->setState(SLAVE_STEPPINGFINISHED);
          switch (m_method) {
          case PARALLEL:  // parallel
            slave->terminate(); //sendCommand(client, clientIndex, fmiTerminateSlave, strlen(fmiTerminateSlave));
            break;
            /*
          case gs:
            if (hasAllClientsState(FMICSServer, stateSteppingFinished)) {
              // loop through all the clients and call fmiTerminate
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
            */
          }
        }
        token = strtok(NULL, "\n");
      }
      free(response);
}

void Master::createStrongConnection(int slaveA, int slaveB, int connectorA, int connectorB){
    m_strongConnections.push_back(new StrongConnection(getSlave(slaveA),getSlave(slaveB),connectorA,connectorB));
}

void Master::createWeakConnection(int slaveA, int slaveB, int valueReferenceA, int valueReferenceB){
    m_weakConnections.push_back(new WeakConnection(getSlave(slaveA),getSlave(slaveB),valueReferenceA,valueReferenceB));
}

void Master::setTimeStep(double timeStep){
    m_timeStep = timeStep;
}

void Master::setEnableEndTime(bool enable){
    m_endTimeEnabled = enable;
}

void Master::setEndTime(double endTime){
    m_endTime = endTime;
}

void Master::setMethod(WeakCouplingAlgorithm algorithm){
    m_method = algorithm;
}


bool Master::hasAllClientsState(SlaveState state){
    for(int i=0; i<m_slaves.size(); i++){
        if(m_slaves[i]->getState() != state)
            return false;
    }
    return true;
}
