/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 25, 2013
 *
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define lw_import
#include <lacewing.h>

#define EXECUTABLE_NAME "fmi-master"
#define VERSION "0.1.3"
#define MAX_CONNECTIONS 1000

extern int debugFlag;

typedef enum METHOD {
    jacobi,
    gs
} METHOD;

/** \brief Base types used in type definitions */
typedef enum connectionState
{
  connectionInvalid,
  connectionRequested,
  connectionComplete
} connectionState;

/**
 * @struct __connection
 * @brief Storage struct for a parsed connection
 * @var connection::fromFMU
 * @brief Index of the FMU that the connection goes from.
 * @var connection::toFMU
 * @brief Index of the FMU that the connection goes to.
 * @var connection::fromOutputVR
 * @brief Value reference of the variable to connect from.
 * @var connection::toInputVR
 * @brief Value reference of the variable to connect to.
 */
typedef struct __connection{
    int fromFMU;                // Index of FMU
    int fromOutputVR;           // Value reference
    int toFMU;                  // FMU index
    int toInputVR;              // Value reference
    connectionState state;
} connection;

#endif /* MAIN_H_ */
