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
 -h Show help and quit.\n\
 -l Show FMILibrary logs.\n\
 -q Quiet mode, do not print simulation info to STDOUT. This flag should be used together with -r.\n\
 -r Real time mode. Will usleep() in between communication steps if the execution was faster than real time.\n\
 -v Show version and quit.\n\
 -x Print XML and quit.\n\
\n\
OPTIONS\n\
\n\
 -c [CONNECTIONS]\n\
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
 -d [TIMESTEP]  Timestep size. Default is 0.1.\n\
 \n\
 -f [OUTFORMAT] Output file format. Currently only \"csv\" is supported, and it\n\
                is also the default.\n\
 \n\
 -g [STEPORDER] FMU stepping order. Used in serial algorithms. Should be a comma\n\
                separated list of FMU indices. If the list starts with 0, then\n\
                the first FMU will be stepped, and its utput values will be\n\
                transferred through their connections (if any).\n\
 \n\
 -m [METHOD]    Stepping method. Available methods are \"jacobi\" (Jacobi,\n\
                parallel) and \"gs\" (Gauss-Seidel, serial). Default is \"jacobi\".\n\
 \n\
 -o [OUTFILE]   Result output file. If set to \"stdout\", then output will be to\n\
                STDOUT. Default value is \"result.csv\".\n\
 \n\
 -p [PARAMS]    Parameter specification. Triplets separated by :, specifying FMU\n\
                index, value reference index and parameter value. Example\n\
                setting the three first value references of FMU 0:\n\
                \"0,0,12.3:0,1,true:0,2,9\". Default is no parameters.\n\
 \n\
 -s [SEPARATOR] CSV separator character. Default is comma (,).\n\
 \n\
 -t [ENDTIME]   End simulation time in seconds. Default is 1.0.\n\
 \n\
EXAMPLES\n\
\n\
    To run an FMU simulation from time 0 to 5 with timestep 0.01:\n\
        fmi-master -t 5 -d 0.01 ../myFMU.fmu\n\n\
\n\
    To simulate two FMUs connected from the first output of the first FMU to the\n\
    first input of the second:\n\n\
        fmi-master -c 0,0,1,0 a.fmu b.fmu\n\
\n\
    To simulate quietly (without output to STDOUT) and save the results to file:\n\
\n\
        fmi-master -q -o out.csv a.fmu\n\
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
