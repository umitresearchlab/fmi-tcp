#include "WeakConnection.h"

using namespace fmitcp;

WeakConnection::WeakConnection( Slave* slaveA,
                                Slave* slaveB,
                                int valueRefA,
                                int valueRefB ) : Connection(slaveA, slaveB){
    m_valueRefA = valueRefA;
    m_valueRefB = valueRefB;
}
WeakConnection::~WeakConnection(){

}

int WeakConnection::getValueRefA(){
    return m_valueRefA;
}

int WeakConnection::getValueRefB(){
    return m_valueRefB;
}
