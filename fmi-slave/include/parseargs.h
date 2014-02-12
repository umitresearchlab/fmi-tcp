#ifndef PARSEARGS_H
#define PARSEARGS_H

#include <fmilib.h>
#include <stdio.h>

#include "main.h"

/**
 * @brief Parses the command line arguments and stores in the given variable pointer targets.
 * @param argc Given by system
 * @param argv Given by system
 * @param fmuFilePath
 * @param loggingOn
 * @param debugLogging
 * @param versionMode
 * @param port
 * @param hostName
 * @return int Returns 0 if the program should proceed, 1 if the program should end.
 */
int parseArguments( int argc,
                    char *argv[],
                    char fmuFilePath[PATH_MAX],
                    int* loggingOn,
                    int* debugLogging,
                    int* versionMode,
                    int* port,
                    char hostName[PATH_MAX]);

#endif /* PARSEARGS_H */
