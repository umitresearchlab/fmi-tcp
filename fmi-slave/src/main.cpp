#include <fmilib.h>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include "Slave.h"

#define VERSION "0.0.1"

using namespace fmitcp;

void printHelp() {
    printf("\n\
FMI-SLAVE\n\
    Client for distributed FMI simulation.\n\
\n\
USAGE\n\
    fmi-slave [FLAGS] [OPTIONS] fmupath\n\
\n\
FLAGS\n\
    -h      Show help and quit.\n\
    -l      Turn on logging\n\
    -dl     Turn on debug logging\n\
    -v      Print version.\n\
    -debug  Start in debug mode.\n\
\n\
OPTIONS\n\
    -port=<integer>     Server port to connect to.\n\
    -host=<string>      Server host.\n\
\n\
EXAMPLES\n\
\n\
LICENSE\n\
\
\tMIT license\n\n");

}

int main( int argc, char *argv[] ) {
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
    std::string hostName="localhost",
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
            printf("%s\n",VERSION); // todo
            return EXIT_SUCCESS;

        } else if (arg == "--debugFlag") {
            debugFlag = true;

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

        } else {
            fmuPath = argv[j];
        }
    }

    if(fmuPath == ""){
        printHelp();
        return EXIT_FAILURE;
    }

    Slave slave(fmuPath);
    slave.setLogger(Logger());
    slave.host(hostName,port);

    return EXIT_SUCCESS;
}
