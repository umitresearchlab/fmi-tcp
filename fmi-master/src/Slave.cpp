#include "Slave.h"
#include "lacewing.h"
#include "string.h"

using namespace fmitcp;

Slave::Slave(lw_client client){
    m_client = client;
    m_id = 0;
}

Slave::~Slave(){

}

void Slave::sendCommand(char* data, size_t size){
  //debugPrint(debugFlag, stderr, "--> %d: %.*s\n", index, size, data);fflush(NULL);
  char cmd[size+1];
  strcpy(cmd, data);
  strcat(cmd, "\n");
  lw_stream_write(m_client, cmd, size+1);
}

void Slave::doStep(){

}

lw_client Slave::getClient(){
    return m_client;
}

int Slave::getId(){
    return m_id;
}

void Slave::setId(int id){
    m_id = id;
}

void Slave::initialize(){

}

void Slave::instantiate(){

}

void Slave::setInitialValues(){

}

void Slave::terminate(){

}

void Slave::setState(SlaveState s){
    m_state = s;
}

SlaveState Slave::getState(){
    return m_state;
}

void Slave::getReal(int valueRef){

}

void Slave::setReal(int valueRef, double value){

}
