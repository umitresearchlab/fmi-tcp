#include <fmilib.h>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <fmitcp/Server.h>
#include <fmitcp/common.h>

using namespace fmitcp;

// Define own server
class MyFMIServer : public Server {
public:
    MyFMIServer(EventPump* pump) : Server(pump) {};
    ~MyFMIServer(){};

    void onClientConnect(){
        //printf("MyFMIServer::onConnect\n");
        //m_pump->exitEventLoop();
    };

    void onClientDisconnect(){
        //printf("MyFMIServer::onDisconnect\n");
        //m_pump->exitEventLoop();
    };

    void onError(string message){
        printf("MyFMIServer::onError\n");fflush(NULL);
        m_pump->exitEventLoop();
    };
};

void printHelp(){
    printf("HELP PAGE: TODO\n");fflush(NULL);
}

int main( int argc, char *argv[] ) {

    printf("FMI Server %s\n",FMITCP_VERSION);

    if (argc == 1) {
        // No args given, print help
        printHelp();
        return EXIT_SUCCESS;
    }

    int j;
    long port = 3000;
    bool loggingOn = false,
         debugLogging = false,
         debugFlag = false;
    std::string hostName = "localhost",
        fmuPath = "";

    for (j = 1; j < argc; j++) {
        std::string arg = argv[j];
        bool last = (j==argc-1);

        if (arg == "-h" || arg == "--help") {
            printHelp();
            return EXIT_SUCCESS;

        } else if (arg == "-l" || arg == "--logging") {
            loggingOn = true;

        } else if (arg == "-d" || arg == "--debugLogging") {
            debugLogging = true;

        } else if (arg == "-v" || arg == "--version") {
            printf("%s\n",FMITCP_VERSION); // todo
            return EXIT_SUCCESS;

        } else if (arg == "--debugFlag") {
            debugFlag = true;

        } else if((arg == "--port" || arg == "-p") && !last) {
            std::string nextArg = argv[j+1];

            std::istringstream ss(nextArg);
            ss >> port;

            if (port <= 0) {
                printf("Invalid port.\n");fflush(NULL);
                return EXIT_FAILURE;
            }

        } else if (arg == "--host" && !last) {
            hostName = argv[j+1];

        } else {
            fmuPath = argv[j];
        }
    }

    if(fmuPath == ""){
        printHelp();
        return EXIT_FAILURE;
    }

    EventPump pump;
    MyFMIServer server(&pump);
    server.host(hostName,port);
    server.getLogger()->setFilter(Logger::LOG_NETWORK|Logger::LOG_DEBUG|Logger::LOG_ERROR);
    pump.startEventLoop();

    return EXIT_SUCCESS;
}
