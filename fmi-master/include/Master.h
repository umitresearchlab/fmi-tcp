#ifndef MASTER_H_
#define MASTER_H_

#include <vector>
#include "Connection.h"
#include "Slave.h"
#include "lacewing.h"
#include <limits.h>

class Master {

private:
    int numFMUS;
    int numClients;
    double tStart;
    double stepSize;
    double tStop;
    int method;
    std::vector<Connection*> m_connections;
    std::vector<Slave*> m_slaves;
    std::vector<int> m_slave_ids;
    lw_pump m_pump;
    int m_slaveIdCounter;

public:
    Master();
    ~Master();

    /// Connects to a slave and gets info about it
    int connectSlave(const char uri[PATH_MAX]);

    Slave * getSlave(lw_client client);
    void clientConnected(lw_client client);
    void clientDisconnected(lw_client client);
    void clientData(lw_client client);

    void createStrongCoupling(int slaveA, int slaveB, int connectorA, int connectorB);
    void createWeakCoupling(int slaveA, int slaveB, int valueReferenceA, int valueReferenceB);

    /// Start simulation
    void simulate();
};

#endif /* MASTER_H_ */
