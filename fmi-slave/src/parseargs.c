#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#include "parseargs.h"

void printInvalidArg(char* option) {
  fprintf(stderr, "Invalid argument of %s. Use -h for help.\n", option);
}

int parseArguments( int argc,
                    char *argv[],
                    char fmuFilePath[PATH_MAX],
                    int* loggingOn,
                    int* debugLogging,
                    int* version,
                    enum METHOD * method,
                    int* port,
                    char hostName[PATH_MAX]){
    int j;
    int numScanned;

    for (j = 1; j < argc; j++) {
      if (strcmp(argv[j], "-h") == 0) {
        printHelp();
        return 1;
      } else if (strncmp(argv[j], "-l", 2) == 0) {
        *loggingOn = 1;
      } else if (strncmp(argv[j], "-dl", 3) == 0) {
        *debugLogging = 1;
      } else if (strncmp(argv[j], "-m=", 3) == 0) {
        if (strcmp(argv[j]+3,"jacobi") == 0) {
          *method = jacobi;
        } else if(strcmp(argv[j]+3,"gs") == 0){
          *method = gs;
        } else {
          fprintf(stderr,"Method \"%s\" not recognized. Use \"jacobi\" or \"gs\".\n",argv[j]+3);
          return 1;
        }
      } else if (strncmp(argv[j], "-v", 2) == 0) {
        *version = 1;
      } else if (strncmp(argv[j], "-port=", 6) == 0) {
        numScanned = sscanf(argv[j]+6, "%d", port);
        if (numScanned <= 0) {
          printInvalidArg(argv[j]);
          return 1;
        }
      } else if (strncmp(argv[j], "-host=", 6) == 0) {
        strcpy(hostName, argv[j]+6);
      } else {
        strcpy(fmuFilePath, argv[j]);
      }
    }

    return 0; // OK
}
