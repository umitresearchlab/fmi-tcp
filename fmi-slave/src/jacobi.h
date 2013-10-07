#ifndef JACOBI_H
#define JACOBI_H

// Takes a Jacobi co-simulation step
int fmi1JacobiStep( double time,
                    double communicationTimeStep,
                    int numFMUs,
                    fmi1_import_t ** fmus,
                    fmi1_import_variable_list_t** variables,
                    int numConnections,
                    connection connections[MAX_CONNECTIONS],
                    int numStepOrder,
                    int stepOrder[MAX_STEP_ORDER]);

int fmi2JacobiStep( double time,
                    double communicationTimeStep,
                    int numFMUs,
                    fmi2_import_t ** fmus,
                    fmi2_import_variable_list_t** variables,
                    int numConnections,
                    connection connections[MAX_CONNECTIONS],
                    int numStepOrder,
                    int stepOrder[MAX_STEP_ORDER]);

#endif /* JACOBI_H */
