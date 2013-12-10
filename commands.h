/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Oct 30, 2013
 *
 */

#ifndef COMMANDS_H_
#define COMMANDS_H_

/* Server Commands */
static char* fmiTStart = "fmiTStart=";
static char* fmiTStartOk = "fmiTStart#ok";
static char* fmiStepSize = "fmiStepSize=";
static char* fmiStepSizeOk = "fmiStepSize#ok";
static char* fmiTEnd = "fmiTEnd=";
static char* fmiTEndOk = "fmiTEnd#ok";

static char* fmiInstantiateSlave = "fmiInstantiateSlave";
static char* fmiInstantiateSlaveError = "fmiInstantiateSlave#-1";
static char* fmiInstantiateSlaveSuccess = "fmiInstantiateSlave#0";
static char* fmiInstantiateSlaveWarning = "fmiInstantiateSlave#1";
static char* fmiInitializeSlave = "fmiInitializeSlave";
static char* fmiInitializeSlaveOk = "fmiInitializeSlave#ok";
static char* fmiInitializeSlaveError = "fmiInitializeSlave#error";
static char* setInitialValues = "setInitialValues";
static char* setInitialValuesOk = "setInitialValues#ok";
static char* fmiDoStep = "fmiDoStep";
static char* fmiDoStepOk = "fmiDoStep#ok";
static char* fmiDoStepFinished = "fmiDoStep#finished";
static char* fmiDoStepError = "fmiDoStep#error";
static char* fmiGetValue = "fmiGetValue=";
static char* fmiGetValueReturn = "fmiGetValueReturn=";
static char* fmiSetValueVr = "fmiSetValueVr=";
static char* fmiSetValue = "fmiSetValue=";
static char* fmiSetValueReturn = "fmiSetValueReturn";
static char* fmiTerminateSlave = "fmiTerminateSlave";

/* Information Commands */
static char* waitingForFMUs = "INFO#Master is waiting for %d more FMU(s) to connect.";

#endif /* COMMANDS_H_ */
