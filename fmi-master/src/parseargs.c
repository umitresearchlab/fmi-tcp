/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Nov 7, 2013
 *
 */

#include "parseargs.h"

void printInvalidArg(char* option) {
  fprintf(stderr, "Invalid argument of %s. Use -h for help.\n", option);
}

int parseArguments(int argc, char *argv[], int* version, int* numFMUs, double* timeStepSize, double* tEnd, int* numConnections,
    connection connections[MAX_CONNECTIONS]) {
  int j, numScanned;
  const char* connectionsArg;
  int n, skip, l, cont, i;
  connection *conn;

  for (j = 1; j < argc; j++) {
    if (strcmp(argv[j], "-h") == 0) {
      printHelp();
      return 1;
    } else if (strncmp(argv[j], "-v", 2) == 0) {
      *version = 1;
    } else if (strncmp(argv[j], "-n=", 3) == 0) {
      numScanned = sscanf(argv[j]+3,"%d", numFMUs);
      if (numScanned <= 0) {
        printInvalidArg(argv[j]);
        return 1;
      }
    } else if (strncmp(argv[j], "-d=", 3) == 0) {
      numScanned = sscanf(argv[j]+3,"%lf", timeStepSize);
      if (numScanned <= 0) {
        printInvalidArg(argv[j]);
        return 1;
      }
    } else if (strncmp(argv[j], "-t=", 3) == 0) {
      numScanned = sscanf(argv[j]+3,"%lf", tEnd);
      if (numScanned <= 0) {
        printInvalidArg(argv[j]);
        return 1;
      }
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
    }
  }

  return 0; // OK
}
