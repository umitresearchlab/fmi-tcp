/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 25, 2013
 *
 */

#include "main.h"
#include "fmiserver.h"

int debugFlag = 0;

void printHeader() {
  printf("  FMU CO-SIMULATION MASTER CLI %s\n",VERSION);
}

void printHelp() {
  char command[200];
  sprintf(command,"man %s",EXECUTABLE_NAME);
  system(command);
}

int main( int argc, char *argv[] ) {

  if(argc == 1) {
    // No args given, print help
    printHelp();
    exit(EXIT_SUCCESS);
  }

  int version=0,
      numFMUs=-1,
      numConnections=0;
  long port = 10200;
  char hostName[PATH_MAX] = "localhost";
  double tEnd=1.0,timeStep=0.1;
  enum METHOD method = jacobi;
  connection connections[MAX_CONNECTIONS];

  int status = parseArguments(argc, argv, &version, &port, hostName, &numFMUs, &timeStep, &tEnd, &method, &numConnections, connections);
  if (version) {
    // version flag given
    printf(VERSION);
    printf("\n");
    exit(EXIT_SUCCESS);
  }

  if (numFMUs == -1) {
    logPrint(stderr, "Number of FMU(s) is not defined. Use -n option e.g -n=2.\n");fflush(NULL);
    exit(EXIT_FAILURE);
  }

  if (status == 1) {
    // Exit
    exit(EXIT_FAILURE);
  }

  printf("\n");
  printHeader();
  printf("\n");
  fflush(NULL);

  FMICoSimulationServer *FMICServer = 0;
  FMICServer = createFMICoSimulationServer(hostName, port, numFMUs, 0, timeStep, tEnd, method, numConnections, connections);
  return 0;
}
