#include <stdio.h>

#include "main.h"
#include "help.h"

void printHeader(){
    printf("  FMU CO-SIMULATION MASTER CLI %s\n",VERSION);
}

void printHelp() {
    char command[200];
    sprintf(command,"man %s",EXECUTABLE_NAME);
    system(command);
}
