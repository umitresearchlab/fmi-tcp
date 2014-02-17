#include "Master.h"
#include <string>
#include <fmitcp/Client.h>
#include <fmitcp/common.h>
#include <stdlib.h>
#include <signal.h>
#include <sstream>
#include <fmilib.h>

using namespace fmitcp;

// Define own client
class MyFMIClient : public Client {
public:
    MyFMIClient(EventPump* pump) : Client(pump) {};
    ~MyFMIClient(){};

    void onConnect(){
        fmi2_import_do_step(0,0,0.0,0.1,true);
    };

    void on_fmi2_import_do_step_res(int message_id, fmitcp_proto::fmi2_status_t status){
        fmi2_import_instantiate_slave(0,0,"","",true);
    };

    void on_fmi2_import_instantiate_slave_res(int message_id, fmitcp_proto::jm_status_enu_t status){
        m_pump->exitEventLoop();
    }

    void onDisconnect(){
        //m_logger.log(Logger::DEBUG,"MyFMIClient::onDisconnect\n");
        m_pump->exitEventLoop();
    };

    void onError(string err){
        m_logger.log(Logger::DEBUG,"MyFMIClient::onError\n");
        m_pump->exitEventLoop();
    };
};

void printHelp(){
    printf("HELP PAGE: TODO\n");fflush(NULL);
}

int main(int argc, char *argv[] ) {

    printf("FMI Master %s\n",FMITCP_VERSION);fflush(NULL);

    /*
    Logger logger;
    Master master(logger);
    master.setTimeStep(0.1);
    master.setEnableEndTime(false);
    master.setWeakMethod(PARALLEL);
    */

    int j, numScanned;
    const char* connectionsArg;
    int n, skip, l, cont, i;
    string hostName = "localhost";
    long port = 3000;

    for (j = 1; j < argc; j++) {
        std::string arg = argv[j];
        bool last = (j == argc-1);

        if (arg == "-h" || arg == "--help") {
            printHelp();
            return EXIT_SUCCESS;

        } else if (arg == "--version") {
            printf("%s\n",FMITCP_VERSION);
            return EXIT_SUCCESS;

        } else if((arg == "--port" || arg == "-p") && !last) {
            std::string nextArg = argv[j+1];

            std::istringstream ss(nextArg);
            ss >> port;

            if (port <= 0) {
                printf("Invalid port.\n");
                return EXIT_FAILURE;
            }

        } else if (arg == "--host" && !last) {
            hostName = argv[j+1];

        } else if (arg ==  "--debug") {
            // debugFlag = 1;
            // Todo: set flag in logger

        } else if (arg == "--timeStep" && !last) {
            /*
            std::string nextArg = argv[j+1];
            double timeStepSize = ::atof(nextArg.c_str());
            j++;

            if(timeStepSize <= 0){
                fprintf(stderr,"Invalid timeStepSize.");
                return EXIT_FAILURE;
            }

            master.setTimeStep(timeStepSize);
            */

        } else if ((arg == "-t" || arg == "--stopAfter") && !last) {
            /*
            std::string nextArg = argv[j+1];
            double endTime = ::atof(nextArg.c_str());
            j++;

            if (endTime <= 0) {
                fprintf(stderr,"Invalid end time.");
                return EXIT_FAILURE;
            }

            master.setEnableEndTime(true);
            master.setEndTime(endTime);
            */

        } else if ((arg == "-m" || arg == "--method") && !last) {
            /*
            std::string nextArg = argv[j+1];
            j++;

            if (nextArg == "jacobi") {
                //*method = jacobi;

            } else if (nextArg == "jacobi") {
                //*method = jacobi;

            } else {
                fprintf(stderr,"Method not recognized. Use \"jacobi\" or \"gs\".\n");
                return EXIT_FAILURE;

            }
            */

        } else {
            // Assume URI to slave
            //master.connectSlave(arg);
        }
    }

    //int slaveA = master.connectSlave("tcp://granular.cs.umu.se:3001");
    //int slaveB = master.connectSlave("tcp://granular.cs.umu.se:3002");
    /*int valueReferenceA = 0,
        valueReferenceB = 0,
        connectorReferenceA = 0,
        connectorReferenceB = 0;
    master.createWeakConnection(slaveA, slaveB, valueReferenceA, valueReferenceB);
    master.createStrongConnection(slaveA, slaveB, connectorReferenceA, connectorReferenceB);
    master.simulate();
    */

    EventPump pump;
    MyFMIClient client(&pump);
    client.connect(hostName,port);
    pump.startEventLoop();

    return 0;
}
