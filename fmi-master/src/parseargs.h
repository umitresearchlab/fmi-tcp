/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 25, 2013
 *
 */

#ifndef PARSEARGS_H_
#define PARSEARGS_H_

#include "main.h"

/**
 * @brief Parses the command line arguments and stores in the given variable pointer targets.
 * @param argc Given by system
 * @param argv Given by system
 * @param versionMode
 * @param port
 * @param hostName
 * @param numFMUs number of FMUs
 * @param timeStepSize
 * @param tEnd
 * @param numConnections
 * @param connections
 * @return int Returns 0 if the program should proceed, 1 if the program should end.
 */
int parseArguments(int argc, char *argv[], int* versionMode, long* port, char hostName[PATH_MAX], int* numFMUs, double* timeStepSize,
    double* tEnd, enum METHOD* method, int* numConnections, connection connections[MAX_CONNECTIONS]);

#endif /* PARSEARGS_H_ */
