#include "Connection.h"

using namespace fmitcp;

Connection::Connection( Slave * slaveA,
                        Slave * slaveB ){
    m_slaveA = slaveA;
    m_slaveB = slaveB;
}
Connection::~Connection(){

}

ConnectionState Connection::getState(){
    return m_state;
}

void Connection::setState(ConnectionState s){
    s = m_state;
}

Slave * Connection::getSlaveA(){
    return m_slaveA;
}

Slave * Connection::getSlaveB(){
    return m_slaveB;
}

