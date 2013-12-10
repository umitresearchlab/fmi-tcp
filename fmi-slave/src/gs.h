#ifndef GS_H
#define GS_H

/**
 * @brief Gauss-Seidel is a stepping method where we step the subsystems in serial. The order of connections will therefore matter.
 */
fmi1_status_t fmi1GaussSeidelStep(FMICoSimulationClient *FMICSClient);

#endif /* GS_H */
