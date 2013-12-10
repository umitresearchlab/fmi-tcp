#include <fmilib.h>
#include <stdio.h>

#include "fmiclient.h"

fmi1_status_t fmi1GaussSeidelStep(FMICoSimulationClient *FMICSClient) {
  return fmi1_import_do_step(FMICSClient->importInstance, FMICSClient->currentTime, FMICSClient->stepSize, 1);
}
