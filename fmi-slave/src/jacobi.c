#include <fmilib.h>
#include <stdio.h>

#include "fmiclient.h"

/**
 * Jacobi is a stepping method where we step the subsystems in parallel.
 * The order of connections will therefore not matter.
 */
fmi1_status_t fmi1JacobiStep(FMICoSimulationClient *FMICSClient) {
  return fmi1_import_do_step(FMICSClient->importInstance, FMICSClient->currentTime, FMICSClient->stepSize, 1);
}
