#ifndef STRONGCONNECTION_H_
#define STRONGCONNECTION_H_

#include "Connection.h"
#include "Slave.h"

class StrongConnection : public Connection {
private:
    int m_connectorRefA;
    int m_connectorRefB;

public:
    StrongConnection(Slave* slaveA,
                     Slave* slaveB,
                     int connectorRefA,
                     int connectorRefB );
    ~StrongConnection();
};

#endif
