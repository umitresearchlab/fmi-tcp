#include "main.h"
#include "utils.h"
#include "parseargs.h"
#include "fmiclient.h"

int debugFlag = 0;

void printHeader(){
  printf("  FMU CO-SIMULATION SLAVE CLI %s\n",VERSION);
}

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
    exit(EXIT_SUCCESS);
  }

  char fmuPath[PATH_MAX] = "";

  int loggingOn=0,
  debugLogging=0,
  version=0;
  int port = -1;
  char hostName[PATH_MAX] = "localhost";
  int status =parseArguments(argc,
      argv,
      fmuPath,
      &loggingOn,
      &debugLogging,
      &version,
      &port,
      hostName);

  if (version) {
    // version flag given
    printf(VERSION);
    printf("\n");
    exit(EXIT_SUCCESS);
  }

  if (strcmp(fmuPath, "") == 0) {
    logPrint(stderr, "No FMU given. Aborting... (see -h for help)\n");
    return EXIT_FAILURE;
  }

  if (status == 1) {
    // Exit
    exit(EXIT_FAILURE);
  }

  printf("\n");
  printHeader();
  printf("\n");
  fflush(NULL);

  // Load FMU
  FMICoSimulationClient *FMICSClient = 0;
  char buf[PATH_MAX+1];  // not sure about the "+ 1"
  char *res = realpath(fmuPath, buf);
  if (res) {
    // OK
    strcpy(fmuPath,buf);
    FMICSClient = createFMICoSimulationClient(fmuPath, loggingOn, debugLogging);
    if (!FMICSClient) {
      exit(EXIT_FAILURE);
    }
  } else {
    // Error opening...
    exit(EXIT_FAILURE);
  }

  connectClient(FMICSClient, hostName, port);

  return EXIT_SUCCESS;
}
