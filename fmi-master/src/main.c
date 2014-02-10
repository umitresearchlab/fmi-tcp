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
    printf("\n\
FMI-MASTER\n\
\n\
    Server for distributed FMI simulation.\n\
\n\
FLAGS\n\
\n\
    -h     Show help and quit.\n\
    -v     Show version and quit.\n\
    -debug Start in debug mode.\n\
    -n     Number of FMUs.\n\
\n\
OPTIONS\n\
\n\
 -c=[CONNECTIONS]\n\
    Connection specification. No connections by default. Quadruples of positive\n\
    integers, representing which FMU and value reference to connect from and\n\
    what to connect to. Syntax is\n\
        CONN1:CONN2:CONN3...\n\
    where CONNX is four comma-separated integers; FMUFROM,VRFROM,FMUTO,VRTO.\n\
    An example connection string is\n\
        0,0,1,0:0,1,1,1\n\
    which means: connect FMU0 (value reference 0) to FMU1 (vr 0) and FMU0 (vr 1)\n\
    to FMU1 (vr 1). Default is no connections.\n\
 \n\
 -d=[TIMESTEP]  Timestep size. Default is 0.1.\n\
 \n\
 -m=[METHOD]    Stepping method. Available methods are \"jacobi\" (Jacobi,\n\
                parallel) and \"gs\" (Gauss-Seidel, serial). Default is \"jacobi\".\n\
 \n\
 -t=[ENDTIME]   End simulation time in seconds. Default is 1.0.\n\
 \n\
 -host=[HOST]   Server host.\n\
 \n\
 -port=[PORT]   Port to listen to.\n\
 \n\
LICENSE\n\
\
\tMIT license\n\n");

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
