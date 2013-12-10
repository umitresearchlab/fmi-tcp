#include <fmilib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>

#include "main.h"
#include "utils.h"
#include "parseargs.h"
#include "fmiclient.h"

void printHeader(){
  printf("  FMU CO-SIMULATION SLAVE CLI %s\n",VERSION);
}

void printHelp() {
  char command[200];
  sprintf(command,"man %s",EXECUTABLE_NAME);
  system(command);
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
  enum METHOD method = jacobi;
  int port = -1;
  char hostName[PATH_MAX] = "localhost";
  int status =parseArguments(argc,
      argv,
      fmuPath,
      &loggingOn,
      &debugLogging,
      &version,
      &method,
      &port,
      hostName);

  if (version) {
    // version flag given
    printf(VERSION);
    printf("\n");
    exit(EXIT_SUCCESS);
  }

  if (strcmp(fmuPath, "") == 0) {
    fprintf(stderr, "No FMU given. Aborting... (see -h for help)\n");
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
    FMICSClient = createFMICoSimulationClient(fmuPath, loggingOn, debugLogging, method);
  } else {
    // Error opening...
    exit(EXIT_FAILURE);
  }

  connectClient(FMICSClient, hostName, port);

  return EXIT_SUCCESS;
}
