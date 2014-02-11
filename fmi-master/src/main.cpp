#include "main.h"
#include "Master.h"

using namespace fmitcp;

int main(int argc, char *argv[] ) {

    printf("FMI Master\n");

    Logger logger;
    Master master(logger);
    master.setTimeStep(0.1);
    master.setEnableEndTime(true);
    master.setEndTime(10);
    master.setMethod(PARALLEL);
    int slaveA = master.connectSlave("tcp://granular.cs.umu.se:3001");
    int slaveB = master.connectSlave("tcp://granular.cs.umu.se:3002");
    int valueReferenceA = 0,
        valueReferenceB = 0,
        connectorReferenceA = 0,
        connectorReferenceB = 0;
    master.createWeakConnection(slaveA, slaveB, valueReferenceA, valueReferenceB);
    master.createStrongConnection(slaveA, slaveB, connectorReferenceA, connectorReferenceB);
    master.simulate();

    return 0;
}
