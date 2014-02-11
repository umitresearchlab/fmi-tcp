#include "Master.h"
#include "Slave.h"
#include "string.h"
#include "stdlib.h"
#include "url_parser.h"

void clientOnConnect(lw_client client) {
  printf("Client connected \n");
  Master * master = (Master*)lw_stream_tag(client);
  master->clientConnected(client);
}

void clientOnData(lw_client client, const char* data, long size) {
  printf("Client data!\n");
  Master * master = (Master*)lw_stream_tag(client);
  master->clientData(client);
}

void clientOnDisconnect(lw_client client) {
  printf("Client disconnected!\n");
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

Master::Master(){
  // One pump for all clients
  m_pump = lw_eventpump_new();
}

Master::~Master(){
  lw_eventpump_post_eventloop_exit(m_pump);
  lw_pump_delete(m_pump);
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
  m_slaves.push_back(slave);
  m_slave_ids.push_back(slaveId);

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

void Master::clientConnected(lw_client client){
  Slave * slave = getSlave(client);
}

void Master::clientDisconnected(lw_client client){
  // Remove from slave vector
  for(int i=0; i<m_slaves.size(); i++){
    if(m_slaves[i]->getClient() == client){
      delete m_slaves[i];
      m_slaves.erase(m_slaves.begin()+i);
      m_slave_ids.erase(m_slave_ids.begin()+i);
      lw_stream_delete(client);
      break;
    }
  }

  // No slaves left - try exit
  if(m_slaves.size() == 0)
    lw_eventpump_post_eventloop_exit(m_pump);
}

void Master::clientData(lw_client client){
  Slave * slave = getSlave(client);
}

void Master::createStrongCoupling(int slaveA, int slaveB, int connectorA, int connectorB){

}

void Master::createWeakCoupling(int slaveA, int slaveB, int valueReferenceA, int valueReferenceB){

}
