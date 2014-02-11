#include "Slave.h"
#include "lacewing.h"

Slave::Slave(lw_client client){
    m_client = client;
}

Slave::~Slave(){

}

void Slave::sendCommand(char* data, size_t size){

}

void Slave::doStep(){

}

lw_client Slave::getClient(){
    return m_client;
}
