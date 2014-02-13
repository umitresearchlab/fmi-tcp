#include "main.h"
#include "Master.h"
#include <string>

using namespace fmitcp;

void printHelp(){

}

int main(int argc, char *argv[] ) {

    printf("FMI Master\n");

    Logger logger;
    Master master(logger);
    master.setTimeStep(0.1);
    master.setEnableEndTime(false);
    master.setWeakMethod(PARALLEL);

    int j, numScanned;
    const char* connectionsArg;
    int n, skip, l, cont, i;
    connection *conn;

    for (j = 1; j < argc; j++) {
        std::string arg = argv[j];
        bool last = (j == argc-1);

        if (arg == "-h" || arg == "--help") {
            printHelp();
            return EXIT_SUCCESS;

        } else if (arg == "--version") {
            printf("%s\n",VERSION);
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

            /*
        } else if (strncmp(argv[j], "-c=", 3) == 0) {
            connectionsArg = argv[j]+3;
            n=0;
            skip=0;
            l=strlen(connectionsArg);
            cont=1;
            i=0;
            conn = &connections[0];
            while((n=sscanf(&connectionsArg[skip],"%d,%d,%d,%d",&conn->fromFMU,&conn->fromOutputVR,&conn->toFMU,&conn->toInputVR))!=-1 && skip<l && cont){
                // Now skip everything before the n'th colon
                char* pos = strchr(&connectionsArg[skip],':');
                if(pos==NULL){
                    cont=0;
                } else {
                    skip += pos-&connectionsArg[skip]+1; // Dunno why this works... See http://www.cplusplus.com/reference/cstring/strchr/
                    conn = &connections[i+1];
                }
                i++;
            }
            *numConnections = i;
*/

        } else {
            // Assume URI to slave
            master.connectSlave(arg);
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
    */
    master.simulate();

    return 0;
}
