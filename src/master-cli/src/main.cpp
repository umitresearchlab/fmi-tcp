#include "Master.h"
#include <string>
#include <fmitcp/Client.h>
#include <stdlib.h>
#include <signal.h>

using namespace fmitcp;

// Define own client
class MyFMIClient : public Client {
public:
    MyFMIClient(EventPump* pump) : Client(pump) {};
    ~MyFMIClient(){};

    void onConnect(){
        printf("MyFMIClient::onConnect\n");
        //fmi2_import_do_step(0,0,0.0,0.1,true);
        m_pump->exitEventLoop();
    };

    void onDoStepResponse(){
        printf("MyFMIClient::onDoStepResponse\n");
        m_pump->exitEventLoop();
    };

    void onDisconnect(){
        printf("MyFMIClient::onDisconnect\n");
        m_pump->exitEventLoop();
    };

    void onError(string err){
        printf("MyFMIClient::onError\n");
        m_pump->exitEventLoop();
    };
};

/*
lw_pump pump;

void onConnect(lw_client c){
    printf("connect\n");
    lw_eventpump_post_eventloop_exit(pump);
}
*/

int main(int argc, char *argv[] ) {

    printf("FMI Master\n");

    EventPump pump;
    MyFMIClient client(&pump);
    client.connect("localhost",3003);
    pump.startEventLoop();

    /*
    pump = lw_eventpump_new();
    lw_client client = lw_client_new (pump);

    lw_client_on_connect(client,onConnect);
    lw_client_connect(client, "localhost", 3000);

    lw_eventpump_start_eventloop(pump);

    lw_stream_delete (client);
    lw_pump_delete (pump);
    */

    /*
    Logger logger;
    Master master(logger);
    master.setTimeStep(0.1);
    master.setEnableEndTime(false);
    master.setWeakMethod(PARALLEL);

    int j, numScanned;
    const char* connectionsArg;
    int n, skip, l, cont, i;

    for (j = 1; j < argc; j++) {
        std::string arg = argv[j];
        bool last = (j == argc-1);

        if (arg == "-h" || arg == "--help") {
            printHelp();
            return EXIT_SUCCESS;

        } else if (arg == "--version") {
            printf("%s\n",FMITCP_VERSION);
            return EXIT_SUCCESS;

        } else if (arg ==  "--debug") {
            // debugFlag = 1;
            // Todo: set flag in logger

        } else if (arg == "--timeStep" && !last) {
            std::string nextArg = argv[j+1];
            double timeStepSize = ::atof(nextArg.c_str());
            j++;

            if(timeStepSize <= 0){
                fprintf(stderr,"Invalid timeStepSize.");
                return EXIT_FAILURE;
            }

            master.setTimeStep(timeStepSize);

        } else if ((arg == "-t" || arg == "--stopAfter") && !last) {
            std::string nextArg = argv[j+1];
            double endTime = ::atof(nextArg.c_str());
            j++;

            if (endTime <= 0) {
                fprintf(stderr,"Invalid end time.");
                return EXIT_FAILURE;
            }

            master.setEnableEndTime(true);
            master.setEndTime(endTime);

        } else if ((arg == "-m" || arg == "--method") && !last) {
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

        } else {
            // Assume URI to slave
            master.connectSlave(arg);
        }
    }
    */

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

    return 0;
}
