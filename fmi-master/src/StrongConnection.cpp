#include "StrongConnection.h"
#include "Slave.h"

using namespace fmitcp;

StrongConnection::StrongConnection( Slave* slaveA,
                                    Slave* slaveB,
                                    int connectorRefA,
                                    int connectorRefB ) : Connection(slaveA,slaveB) {
    m_connectorRefA = connectorRefA;
    m_connectorRefB = connectorRefB;
}
StrongConnection::~StrongConnection(){

}
