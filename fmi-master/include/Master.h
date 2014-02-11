#ifndef MASTER_H_
#define MASTER_H_

#include <vector>
#include "Connection.h"
#include "Slave.h"
#include "Logger.h"
#include "lacewing.h"
#include <limits.h>

enum WeakCouplingAlgorithm {
    SERIAL = 1,
    PARALLEL = 2
};

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
    Logger m_logger;
    WeakCouplingAlgorithm m_method;
    double m_timeStep;
    double m_endTime;
    bool m_endTimeEnabled;

public:
    Master();
    Master(const Logger& logger);
    ~Master();

    void init();

    /// Connects to a slave and gets info about it
    int connectSlave(const char uri[PATH_MAX]);

    Slave * getSlave(lw_client client);
    void clientConnected(lw_client client);
    void clientDisconnected(lw_client client);
    void clientData(lw_client client);

    void setTimeStep(double timeStep);
    void setEnableEndTime(bool enable);
    void setEndTime(double endTime);
    void setMethod(WeakCouplingAlgorithm algorithm);

    void createStrongConnection(int slaveA, int slaveB, int connectorA, int connectorB);
    void createWeakConnection(int slaveA, int slaveB, int valueReferenceA, int valueReferenceB);

    /// Start simulation
    void simulate();
};

#endif /* MASTER_H_ */
