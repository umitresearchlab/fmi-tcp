#include <string>
#include "Message.h"
#include "stdlib.h"
#include "string.h"

using namespace fmitcp;

Message::Message(){
    reset();
}

Message::~Message(){

}

void Message::reset(){
    m_type = message_none;
}

void Message::serialize(char* data){
    switch(m_type){

    case message_none:
        throw new std::exception;
        break;

    case message_tStart:
        strcpy(data,fmiTStart.c_str());
        break;

    case message_tStartOk: break;
    case message_stepSize: break;
    case message_stepSizeOk: break;
    case message_tEnd: break;
    case message_tEndOk: break;
    case message_instantiateSlave: break;
    case message_instantiateSlaveError: break;
    case message_instantiateSlaveSuccess: break;
    case message_instantiateSlaveWarning: break;
    case message_initializeSlave: break;
    case message_initializeSlaveOk: break;
    case message_initializeSlaveError: break;
    case message_setInitialValues: break;
    case message_setInitialValuesOk: break;
    case message_doStep: break;
    case message_doStepOk: break;
    case message_doStepPending: break;
    case message_doStepFinished: break;
    case message_doStepError: break;
    case message_doStepStatus: break;
    case message_doStepStatusOk: break;
    case message_doStepStatusPending: break;
    case message_getValue: break;
    case message_getValueReturn: break;
    case message_setValueVr: break;
    case message_setValue: break;
    case message_setValueReturn: break;
    case message_terminateSlave: break;
    }
}

bool Message::parse(const char* data, long size){

}

long Message::getSize(){
    return 1000; // Todo
}

Message::Type Message::getType(){
    return m_type;
}

void Message::setType(Type t){
    m_type = t;
}

const std::string Message::fmiTStart = "fmiTStart=";
const std::string Message::fmiTStartOk = "fmiTStart#ok";
const std::string Message::fmiStepSize = "fmiStepSize=";
const std::string Message::fmiStepSizeOk = "fmiStepSize#ok";
const std::string Message::fmiTEnd = "fmiTEnd=";
const std::string Message::fmiTEndOk = "fmiTEnd#ok";
const std::string Message::fmiInstantiateSlave = "fmiInstantiateSlave";
const std::string Message::fmiInstantiateSlaveError = "fmiInstantiateSlave#-1";
const std::string Message::fmiInstantiateSlaveSuccess = "fmiInstantiateSlave#0";
const std::string Message::fmiInstantiateSlaveWarning = "fmiInstantiateSlave#1";
const std::string Message::fmiInitializeSlave = "fmiInitializeSlave";
const std::string Message::fmiInitializeSlaveOk = "fmiInitializeSlave#ok";
const std::string Message::fmiInitializeSlaveError = "fmiInitializeSlave#error";
const std::string Message::setInitialValues = "setInitialValues";
const std::string Message::setInitialValuesOk = "setInitialValues#ok";
const std::string Message::fmiDoStep = "fmiDoStep";
const std::string Message::fmiDoStepOk = "fmiDoStep#ok";
const std::string Message::fmiDoStepPending = "fmiDoStep#pending";
const std::string Message::fmiDoStepFinished = "fmiDoStep#finished";
const std::string Message::fmiDoStepError = "fmiDoStep#error";
const std::string Message::fmiDoStepStatus = "fmiDoStepStatus";
const std::string Message::fmiDoStepStatusOk = "fmiDoStepStatus#ok";
const std::string Message::fmiDoStepStatusPending = "fmiDoStepStatus#pending";
const std::string Message::fmiGetValue = "fmiGetValue=";
const std::string Message::fmiGetValueReturn = "fmiGetValueReturn=";
const std::string Message::fmiSetValueVr = "fmiSetValueVr=";
const std::string Message::fmiSetValue = "fmiSetValue=";
const std::string Message::fmiSetValueReturn = "fmiSetValueReturn";
const std::string Message::fmiTerminateSlave = "fmiTerminateSlave";
const std::string Message::waitingForFMUs = "INFO#Master is waiting for %d more FMU(s) to connect.";
