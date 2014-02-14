/*
 * 
 * @author Adeel Asghar <adeel.asghar@liu.se>
 *
 * Created on: Nov 13, 2013
 *
 */

#include "fmiclient.h"
#include "../../commands.h"
#include "../../helper.h"

void fmi1_null_logger(  fmi1_component_t    c,
                        fmi1_string_t   instanceName,
                        fmi1_status_t   status,
                        fmi1_string_t   category,
                        fmi1_string_t   message, ... ) { }

void fmi2_null_logger(  fmi2_component_t c,
                        fmi2_string_t instanceName,
                        fmi2_status_t status,
                        fmi2_string_t category,
                        fmi2_string_t message, ...){ }

void importlogger(jm_callbacks* c, jm_string module, jm_log_level_enu_t log_level, jm_string message) {
  printf("module = %s, log level = %s: %s\n", module, jm_log_level_to_string(log_level), message);fflush(NULL);
}

FMICoSimulationClient* createFMICoSimulationClient(const char* fileName, int loggingOn, int debugLogging) {
  FMICoSimulationClient* FMICSClient = malloc(sizeof(FMICoSimulationClient));
  FMICSClient->logLevel = loggingOn ? jm_log_level_all : jm_log_level_fatal;
  /* JM callbacks */
  FMICSClient->JMCallbacks.malloc = malloc;
  FMICSClient->JMCallbacks.calloc = calloc;
  FMICSClient->JMCallbacks.realloc = realloc;
  FMICSClient->JMCallbacks.free = free;
  FMICSClient->JMCallbacks.logger = importlogger;
  FMICSClient->JMCallbacks.log_level = FMICSClient->logLevel;
  FMICSClient->JMCallbacks.context = 0;
  /* working directory */
  FMICSClient->workingDirectory = fmi_import_mk_temp_dir(&FMICSClient->JMCallbacks, NULL, EXECUTABLE_NAME);
  /* import allocate context */
  FMICSClient->importContext = fmi_import_allocate_context(&FMICSClient->JMCallbacks);
  /* get FMU version */
  FMICSClient->version = fmi_import_get_fmi_version(FMICSClient->importContext, fileName, FMICSClient->workingDirectory);
  if ((FMICSClient->version <= fmi_version_unknown_enu) || (FMICSClient->version >= fmi_version_unsupported_enu)) {
    fmi_import_free_context(FMICSClient->importContext);
    free(FMICSClient->workingDirectory);
    free(FMICSClient);
    logPrint(stderr, "The FMU version is %s. Unknown/Unsupported FMU version.\n", fmi_version_to_string(FMICSClient->version));fflush(NULL);
    return 0;
  }
  if (FMICSClient->version == fmi_version_1_enu) { // FMI 1.0
    // parse the xml file
    FMICSClient->FMI1ImportInstance = fmi1_import_parse_xml(FMICSClient->importContext, FMICSClient->workingDirectory);
    if(!FMICSClient->FMI1ImportInstance) {
      fmi_import_free_context(FMICSClient->importContext);
      free(FMICSClient->workingDirectory);
      free(FMICSClient);
      logPrint(stderr, "Error parsing the XML file contained in %s\n", FMICSClient->workingDirectory);
      return 0;
    }
    /* check FMU kind */
    fmi1_fmu_kind_enu_t fmuType = fmi1_import_get_fmu_kind(FMICSClient->FMI1ImportInstance);
    if(fmuType != fmi1_fmu_kind_enu_cs_standalone && fmuType != fmi1_fmu_kind_enu_cs_tool) {
      fmi_import_free_context(FMICSClient->importContext);
      free(FMICSClient->workingDirectory);
      free(FMICSClient);
      logPrint(stderr,"Only FMI Co-Simulation 1.0 & 2.0 are supported.\n");fflush(NULL);
      return 0;
    }
    FMICSClient->fmuType = fmuType;
    /* FMI callback functions */
    int registerGlobally = 0;
    FMICSClient->FMI1CallbackFunctions.logger = loggingOn ? fmi1_log_forwarding : fmi1_null_logger;
    FMICSClient->FMI1CallbackFunctions.allocateMemory = calloc;
    FMICSClient->FMI1CallbackFunctions.freeMemory = free;
    /* Load the binary (dll/so) */
    jm_status_enu_t status = fmi1_import_create_dllfmu(FMICSClient->FMI1ImportInstance, FMICSClient->FMI1CallbackFunctions, registerGlobally);
    if (status == jm_status_error) {
      fmi_import_free_context(FMICSClient->importContext);
      free(FMICSClient->workingDirectory);
      free(FMICSClient);
      logPrint(stderr, "There was an error loading the FMU dll. Turn on logging (-l) for more info.\n");
      return 0;
    }
    FMICSClient->instanceName = (char*)fmi1_import_get_model_name(FMICSClient->FMI1ImportInstance);
    FMICSClient->fmuLocation = fmi_import_create_URL_from_abs_path(&FMICSClient->JMCallbacks, (const char*)fileName);
    FMICSClient->mimeType = "application/x-fmu-sharedlibrary";       // denotes tool in case of tool coupling
    FMICSClient->timeOut = 1000;     // wait period in milliseconds, 0 for unlimited wait period
    FMICSClient->visible = 0;        // no simulator user interface
    FMICSClient->interactive = 0;    // simulation run without user interaction
    FMICSClient->debugLogging = debugLogging;
    /*
     * We define these fields in the clientOnData function.
     * These are the first values sent by the server to the client.
     *
    FMICSClient->tStart = 0;
    FMICSClient->currentTime = 0;
    FMICSClient->stepSize = stepSize;
    FMICSClient->stopTimeDefined = tEnd > 0;
    FMICSClient->tStop = tEnd;
    */
    FMICSClient->debugLogging = debugLogging;
    FMICSClient->FMI1Variables = fmi1_import_get_variable_list(FMICSClient->FMI1ImportInstance);
  } else if (FMICSClient->version == fmi_version_2_0_enu) { // FMI 2.0
    // parse the xml file
    FMICSClient->FMI2ImportInstance = fmi2_import_parse_xml(FMICSClient->importContext, FMICSClient->workingDirectory, NULL);
    if(!FMICSClient->FMI2ImportInstance) {
      fmi_import_free_context(FMICSClient->importContext);
      free(FMICSClient->workingDirectory);
      free(FMICSClient);
      logPrint(stderr, "Error parsing the XML file contained in %s\n", FMICSClient->workingDirectory);
      return 0;
    }
    /* check FMU kind */
    fmi2_fmu_kind_enu_t fmuType = fmi2_import_get_fmu_kind(FMICSClient->FMI2ImportInstance);
    if(fmuType != fmi2_fmu_kind_cs) {
      fmi_import_free_context(FMICSClient->importContext);
      free(FMICSClient->workingDirectory);
      free(FMICSClient);
      logPrint(stderr,"Only FMI Co-Simulation 1.0 & 2.0 are supported.\n");fflush(NULL);
      return 0;
    }
    FMICSClient->fmuType = fmuType;
    /* FMI callback functions */
    FMICSClient->FMI2CallbackFunctions.logger = loggingOn ? fmi2_log_forwarding : fmi2_null_logger;
    FMICSClient->FMI2CallbackFunctions.allocateMemory = calloc;
    FMICSClient->FMI2CallbackFunctions.freeMemory = free;
    /* Load the binary (dll/so) */
    jm_status_enu_t status = fmi2_import_create_dllfmu(FMICSClient->FMI2ImportInstance, fmuType, &FMICSClient->FMI2CallbackFunctions);
    if (status == jm_status_error) {
      fmi_import_free_context(FMICSClient->importContext);
      free(FMICSClient->workingDirectory);
      free(FMICSClient);
      logPrint(stderr, "There was an error loading the FMU dll. Turn on logging (-l) for more info.\n");fflush(NULL);
      return 0;
    }
    FMICSClient->instanceName = (char*)fmi2_import_get_model_name(FMICSClient->FMI2ImportInstance);
    FMICSClient->fmuLocation = fmi_import_create_URL_from_abs_path(&FMICSClient->JMCallbacks, (const char*)fileName);
    FMICSClient->mimeType = "application/x-fmu-sharedlibrary";       // denotes tool in case of tool coupling
    FMICSClient->timeOut = 1000;     // wait period in milliseconds, 0 for unlimited wait period
    FMICSClient->visible = 0;        // no simulator user interface
    FMICSClient->interactive = 0;    // simulation run without user interaction
    FMICSClient->debugLogging = debugLogging;
    /*
     * We define these fields in the clientOnData function.
     * These are the first values sent by the server to the client.
     *
        FMICSClient->tStart = 0;
        FMICSClient->currentTime = 0;
        FMICSClient->stepSize = stepSize;
        FMICSClient->stopTimeDefined = tEnd > 0;
        FMICSClient->tStop = tEnd;
     */
    FMICSClient->debugLogging = debugLogging;
    /* 0 - original order as found in the XML file;
     * 1 - sorted alphabetically by variable name;
     * 2 sorted by types/value references.
     */
    int sortOrder = 0;
    FMICSClient->FMI2Variables = fmi2_import_get_variable_list(FMICSClient->FMI2ImportInstance, sortOrder);
  }
  return FMICSClient;
}

void destroyFMICoSimulationClient(FMICoSimulationClient *FMICSClient) {
  fmi1_import_terminate_slave(FMICSClient->FMI1ImportInstance);
  fmi1_import_free_slave_instance(FMICSClient->FMI1ImportInstance);
  fmi1_import_destroy_dllfmu(FMICSClient->FMI1ImportInstance);
  fmi1_import_free(FMICSClient->FMI1ImportInstance);
  fmi_import_free_context(FMICSClient->importContext);
  fmi_import_rmdir(&FMICSClient->JMCallbacks, FMICSClient->workingDirectory);
  free(FMICSClient->workingDirectory);
}

void sendCommand(lw_client client, char* data, size_t size) {
  debugPrint(debugFlag, stderr, "sending data=%.*s\n", size, data);fflush(NULL);
  char cmd[size+1];
  strcpy(cmd, data);
  strcat(cmd, "\n");
  lw_stream_write(client, cmd, size+1);
}

jm_status_enu_t fmi1InstantiateSlaveWrapper(FMICoSimulationClient *FMICSClient) {
  jm_status_enu_t status = fmi1_import_instantiate_slave(FMICSClient->FMI1ImportInstance, FMICSClient->instanceName, FMICSClient->fmuLocation,
      FMICSClient->mimeType, FMICSClient->timeOut, FMICSClient->visible, FMICSClient->interactive);
  if (status != jm_status_error) {
    fmi1_import_set_debug_logging(FMICSClient->FMI1ImportInstance, FMICSClient->debugLogging);
  }
  return status;
}

fmi1_status_t fmi1InitializeSlaveWrapper(FMICoSimulationClient *FMICSClient) {
  fmi1_status_t status = fmi1_import_initialize_slave(FMICSClient->FMI1ImportInstance, FMICSClient->tStart, FMICSClient->stopTimeDefined,
      FMICSClient->tStop);
  return status;
}

void fmi1SetInitialValues(FMICoSimulationClient *FMICSClient) {
  int k;
  int num = fmi1_import_get_variable_list_size(FMICSClient->FMI1Variables);
  for (k=0; num; k++) {
    fmi1_import_variable_t * v = fmi1_import_get_variable(FMICSClient->FMI1Variables, k);
    if(!v) break;

    fmi1_value_reference_t vr[1];
    vr[0] = fmi1_import_get_variable_vr(v);

    fmi1_import_variable_typedef_t * vt = fmi1_import_get_variable_declared_type(v);

    fmi1_base_type_enu_t bt;
    bt = fmi1_import_get_variable_base_type(v);

    fmi1_real_t lol[1];
    fmi1_integer_t innt[1];
    fmi1_boolean_t boool[1];
    fmi1_string_t striing[1];

    // Set initial values from the XML file
    if(fmi1_import_get_variable_has_start(v)){
      switch (bt){
      case fmi1_base_type_real:
        lol[0] = fmi1_import_get_real_variable_start((fmi1_import_real_variable_t*) v);
        fmi1_import_set_real(FMICSClient->FMI1ImportInstance,   vr,   1, lol);
        break;
      case fmi1_base_type_int:
      case fmi1_base_type_enum:
        innt[0] = fmi1_import_get_integer_variable_start((fmi1_import_integer_variable_t*) v);
        fmi1_import_set_integer(FMICSClient->FMI1ImportInstance,   vr,   1, innt);
        break;
      case fmi1_base_type_bool:
        boool[0] = fmi1_import_get_boolean_variable_start((fmi1_import_bool_variable_t*) v);
        fmi1_import_set_boolean(FMICSClient->FMI1ImportInstance,   vr,   1, boool);
        break;
      case fmi1_base_type_str:
        striing[0] = fmi1_import_get_string_variable_start((fmi1_import_string_variable_t*) v);
        fmi1_import_set_string(FMICSClient->FMI1ImportInstance,   vr,   1, striing);
        break;
      default:
        logPrint(stderr,"Could not determine type of value reference %d in FMU. Continuing without setting initial value...\n", vr[0]);
        break;
      }
    }
  }
}

fmi1_import_variable_t* fmi1GetVariableByVr(FMICoSimulationClient *FMICSClient, int valueReference) {
  int i;
  size_t n = fmi1_import_get_variable_list_size(FMICSClient->FMI1Variables);
  fmi1_import_variable_t* v = 0;
  for (i = 0 ; i < n ; i++) {
    v = fmi1_import_get_variable(FMICSClient->FMI1Variables, i);
    if (fmi1_import_get_variable_vr(v) == valueReference) {
      return v;
    }
  }
  return v;
}

void fmi1GetValue(FMICoSimulationClient *FMICSClient, int valueReference, char* retVal) {
  fmi1_import_variable_t* v = fmi1GetVariableByVr(FMICSClient, valueReference);
  if (!v)
    return;
  fmi1_value_reference_t vr[1];
  vr[0] = valueReference;
  fmi1_base_type_enu_t baseType = fmi1_import_get_variable_base_type(v);
  fmi1_real_t rr[1];
  fmi1_boolean_t bb[1];
  fmi1_integer_t ii[1];
  fmi1_string_t ss[1];
  switch (baseType) {
  case fmi1_base_type_real:
    fmi1_import_get_real(FMICSClient->FMI1ImportInstance, vr, 1, rr);
    debugPrint(debugFlag, stdout, "fmi1_import_get_real = %f\n", rr[0]);fflush(NULL);
    sprintf(retVal, "%f", rr[0]);
    break;
  case fmi1_base_type_int:
  case fmi1_base_type_enum:
    fmi1_import_get_integer(FMICSClient->FMI1ImportInstance, vr, 1, ii);
    debugPrint(debugFlag, stdout, "fmi1_import_get_integer = %d\n", ii[0]);fflush(NULL);
    sprintf(retVal, "%d", ii[0]);
    break;
  case fmi1_base_type_bool:
    fmi1_import_get_boolean(FMICSClient->FMI1ImportInstance, vr, 1, bb);
    debugPrint(debugFlag, stdout, "fmi1_import_get_boolean = %d\n", bb[0]);fflush(NULL);
    sprintf(retVal, "%d", bb[0]);
    break;
  case fmi1_base_type_str:
    fmi1_import_get_string(FMICSClient->FMI1ImportInstance, vr, 1, ss);
    debugPrint(debugFlag, stdout, "fmi1_import_get_string = %s\n", ss[0]);fflush(NULL);
    sprintf(retVal, "%s", ss[0]);
    break;
  default:
    logPrint(stderr,"Could not determine type of value reference %d in FMU. Continuing without connection value transfer...\n", vr[0]);
    break;
  }
}

void fmi1SetValue(FMICoSimulationClient *FMICSClient, int valueReference, const char* data, const char* name) {
  fmi1_import_variable_t* v = fmi1GetVariableByVr(FMICSClient, valueReference);
  if (!v)
    return;
  fmi1_value_reference_t vr[1];
  vr[0] = valueReference;
  fmi1_base_type_enu_t baseType = fmi1_import_get_variable_base_type(v);
  fmi1_real_t rr[1];
  double rr_value;
  fmi1_integer_t ii[1];
  int ii_value;
  fmi1_boolean_t bb[1];
  int bb_value;
  fmi1_string_t ss[1];
  switch (baseType) {
  case fmi1_base_type_real:
    rr_value = unparseDoubleResult(data, name, strlen(data));
    rr[0] = rr_value;
    fmi1_import_set_real(FMICSClient->FMI1ImportInstance, vr, 1, rr);
    break;
  case fmi1_base_type_int:
  case fmi1_base_type_enum:
    ii_value = unparseIntResult(data, name, strlen(data));
    ii[0] = ii_value;
    fmi1_import_set_integer(FMICSClient->FMI1ImportInstance, vr, 1, ii);
    break;
  case fmi1_base_type_bool:
    bb_value = unparseIntResult(data, name, strlen(data));
    bb[0] = bb_value;
    fmi1_import_set_boolean(FMICSClient->FMI1ImportInstance, vr, 1, bb);
    break;
  case fmi1_base_type_str:
    logPrint(stderr, "fmi1_import_set_string is not handled yet.\n");fflush(NULL);
    break;
  default:
    logPrint(stdout, "Could not determine type of value reference %d. Continuing without fmi1SetValue.\n", valueReference);
    break;
  }
}

fmi1_status_t fmi1DoStep(FMICoSimulationClient *FMICSClient, int *finished) {
  fmi1_status_t status = fmi1_status_error;
  debugPrint(debugFlag, stdout, "FMICSClient->tStart=%f\n", FMICSClient->tStart);fflush(NULL);
  debugPrint(debugFlag, stdout, "FMICSClient->currentTime=%f\n", FMICSClient->currentTime);fflush(NULL);
  debugPrint(debugFlag, stdout, "FMICSClient->tStop=%f\n", FMICSClient->tStop);fflush(NULL);
  if (FMICSClient->currentTime < FMICSClient->tStop) {
    status = fmi1_import_do_step(FMICSClient->FMI1ImportInstance, FMICSClient->currentTime, FMICSClient->stepSize, 1);
    FMICSClient->currentTime += FMICSClient->stepSize;
    *finished = 0;
  } else {
    *finished = 1;
  }
  return status;
}

fmi1_status_t fmi1PendingStatusString(FMICoSimulationClient *FMICSClient, fmi1_string_t *str) {
  return fmi1_import_get_string_status(FMICSClient->FMI1ImportInstance, fmi1_pending_status, str);
}

fmi1_status_t fmi1DoStepStatus(FMICoSimulationClient *FMICSClient, fmi1_status_t *status) {
  return fmi1_import_get_status(FMICSClient->FMI1ImportInstance, fmi1_do_step_status, status);
}

jm_status_enu_t fmi2InstantiateSlaveWrapper(FMICoSimulationClient *FMICSClient) {
  jm_status_enu_t status = fmi2_import_instantiate(FMICSClient->FMI2ImportInstance, FMICSClient->instanceName, FMICSClient->fmuType,
      FMICSClient->fmuLocation, FMICSClient->visible);
  if (status != jm_status_error) {
    /* fetch the logging categories from the FMU */
    size_t nCategories = fmi2_import_get_log_categories_num(FMICSClient->FMI2ImportInstance);
    fmi2_string_t categories[nCategories];
    int i;
    for (i = 0 ; i < nCategories ; i++) {
      categories[i] = fmi2_import_get_log_category(FMICSClient->FMI2ImportInstance, i);
    }
    /* set debug logging */
    fmi2_import_set_debug_logging(FMICSClient->FMI2ImportInstance, FMICSClient->debugLogging, nCategories, categories);
  }
  return status;
}

void connectClient(FMICoSimulationClient *FMICSClient, const char* hostName, int port) {
  FMICSClient->pump = lw_eventpump_new();
  FMICSClient->client = lw_client_new(FMICSClient->pump);
  /* save this object in the client tag so we can use it later on. */
  lw_stream_set_tag(FMICSClient->client, (void*)FMICSClient);
  /* connect the hooks */
  lw_client_on_connect(FMICSClient->client, clientOnConnect);
  lw_client_on_data(FMICSClient->client, clientOnData);
  lw_client_on_disconnect(FMICSClient->client, clientOnDisconnect);
  lw_client_on_error(FMICSClient->client, clientOnError);
  /* connect the client to the server */
  lw_client_connect(FMICSClient->client, hostName, port);
  /* start the eventloop */
  lw_eventpump_start_eventloop(FMICSClient->pump);
}

void clientOnConnect(lw_client client) {
  if (!lw_client_connected(client))
  {
    /*fprintf(stdout, "Client connected \n");fflush(NULL);
    lw_stream_write(client, "Hello Server!", 20);*/
  }
}

void clientOnData(lw_client client, const char* data, long size) {
  char* request = (char*)malloc(size+1);
  strncpy(request, data, size);
  request[size] = '\0';
  debugPrint(debugFlag, stdout, "clientOnData = %s\n", request);fflush(NULL);
  FMICoSimulationClient *FMICSClient = (FMICoSimulationClient*)lw_stream_tag(client);

  char* token;
  token = strtok(request, "\n");
  while (token != NULL)
  {
    debugPrint(debugFlag, stdout, "token = %s\n", token);fflush(NULL);
    if (strncmp(token, fmiTStart, strlen(fmiTStart)) == 0) {  /* Handle fmiTStart */
      FMICSClient->tStart = unparseDoubleResult(token, fmiTStart, size);
      FMICSClient->currentTime = FMICSClient->tStart;
    } else if (strncmp(token, fmiStepSize, strlen(fmiStepSize)) == 0) {  /* Handle fmiTStart */
      FMICSClient->stepSize = unparseDoubleResult(token, fmiStepSize, size);
    } else if (strncmp(token, fmiTEnd, strlen(fmiTEnd)) == 0) {  /* Handle fmiTStart */
      FMICSClient->tStop = unparseDoubleResult(token, fmiTEnd, size);
      FMICSClient->stopTimeDefined = FMICSClient->tStop > 0;
      sendCommand(client, fmiTEndOk, strlen(fmiTEndOk));
    }
    /* handle commands according to the FMI version */
    if (FMICSClient->version == fmi_version_1_enu) { // FMI 1.0
      fmi1ClientOnData(FMICSClient, client, token);
    } else if (FMICSClient->version == fmi_version_2_0_enu) { // FMI 2.0
      fmi2ClientOnData(FMICSClient, client, token);
    }
    token = strtok(NULL, "\n");
  }
  free(request);
}

void fmi1ClientOnData(FMICoSimulationClient *FMICSClient, lw_client client, const char* token) {
  if (strncmp(token, fmiInstantiateSlave, strlen(fmiInitializeSlave)) == 0) {  /* Handle fmiInstantiateSlave */
    jm_status_enu_t status = fmi1InstantiateSlaveWrapper(FMICSClient);
    if (status == jm_status_success) {
      sendCommand(client, fmiInstantiateSlaveSuccess, strlen(fmiInstantiateSlaveSuccess));
    } else if (status == jm_status_warning) {
      sendCommand(client, fmiInstantiateSlaveWarning, strlen(fmiInstantiateSlaveWarning));
    } else {
      sendCommand(client, fmiInstantiateSlaveError, strlen(fmiInstantiateSlaveError));
    }
  } else if (strncmp(token, fmiInitializeSlave, strlen(fmiInitializeSlave)) == 0) {  /* Handle fmiInitializeSlave */
    fmi1_status_t status = fmi1InitializeSlaveWrapper(FMICSClient);
    if (status == fmi1_status_ok) {
      sendCommand(client, fmiInitializeSlaveOk, strlen(fmiInitializeSlaveOk));
    } else {
      sendCommand(client, fmiInitializeSlaveError, strlen(fmiInitializeSlaveError));
    }
  } else if (strncmp(token, setInitialValues, strlen(setInitialValues)) == 0) {  /* Handle fmiInitializeSlave */
    fmi1SetInitialValues(FMICSClient);
    sendCommand(client, setInitialValuesOk, strlen(setInitialValuesOk));
  } else if (strncmp(token, fmiGetValue, strlen(fmiGetValue)) == 0) {  /* Handle fmiGetValue */
    char value[50];
    fmi1GetValue(FMICSClient, unparseIntResult(token, fmiGetValue, strlen(token)), value);
    char cmd[50];
    sprintf(cmd, "%s%s", fmiGetValueReturn, value);
    sendCommand(client, cmd, strlen(cmd));
  } else if (strncmp(token, fmiSetValueVr, strlen(fmiSetValueVr)) == 0) {  /* Handle fmiSetValueVr */
    /* split the fmisetvalue on #. the data is of form fmiSetValueVr=1#fmiSetValue=2.1 */
    char* fmiSetValueVrStr = strtok((char*)token, "#");
    char* fmiSetValueStr = strtok(NULL, " ");
    /* fetch the results */
    int vr = unparseIntResult(fmiSetValueVrStr, fmiSetValueVr, strlen(fmiSetValueVrStr));
    /* do fmi1_import_set_real, fmi1_import_set_integer etc. */
    fmi1SetValue(FMICSClient, vr, fmiSetValueStr, fmiSetValue);
    /* tell server we are done setting value */
    sendCommand(client, fmiSetValueReturn, strlen(fmiSetValueReturn));
  } else if (strncmp(token, fmiDoStep, strlen(fmiDoStep)) == 0) {  /* Handle fmiDoStep */
    int finished = 0;
    fmi1_status_t status = fmi1DoStep(FMICSClient, &finished);
    if (finished) {
      sendCommand(client, fmiDoStepFinished, strlen(fmiDoStepFinished));
    } else {
      if (status == fmi1_status_ok) {
        sendCommand(client, fmiDoStepOk, strlen(fmiDoStepOk));
      } else if (status == fmi1_status_pending) {
        /* ask the pending status from the FMU and print it as info */
        fmi1_string_t str;
        if (fmi1PendingStatusString(FMICSClient, &str) == fmi1_status_ok) {
          logPrint(stdout, "INFO#%s\n", str);fflush(NULL);
        }
        /* tell the server that fmiDoStep is in pending state. */
        sendCommand(client, fmiDoStepPending, strlen(fmiDoStepPending));
      } else {
        sendCommand(client, fmiDoStepError, strlen(fmiDoStepError));
      }
    }
  } else if (strncmp(token, fmiDoStepStatus, strlen(fmiDoStepStatus)) == 0) {  // handle fmiDoStepStatus
    fmi1_status_t status = fmi1_status_error;
    fmi1DoStepStatus(FMICSClient, &status);
    if (status == fmi1_status_ok) {
      sendCommand(client, fmiDoStepOk, strlen(fmiDoStepOk));
    } else if (status == fmi1_status_pending) {
      /* ask the pending status from the FMU and print it as info */
      fmi1_string_t str;
      if (fmi1PendingStatusString(FMICSClient, &str) == fmi1_status_ok) {
        logPrint(stdout, "INFO#%s\n", str);fflush(NULL);
      }
      /* tell the server that fmiDoStep is in pending state. */
      sendCommand(client, fmiDoStepPending, strlen(fmiDoStepPending));
    } else {
      sendCommand(client, fmiDoStepError, strlen(fmiDoStepError));
    }
  } else if (strncmp(token, fmiTerminateSlave, strlen(fmiTerminateSlave)) == 0) {  // handle fmiTerminateSlave
    destroyFMICoSimulationClient(FMICSClient);
    lw_stream_close(client, lw_true);
  } else {
    debugPrint(debugFlag, stdout, "%s\n", token);fflush(NULL);
  }
}

void fmi2ClientOnData(FMICoSimulationClient *FMICSClient, lw_client client, const char* token) {
  if (strncmp(token, fmiInstantiateSlave, strlen(fmiInitializeSlave)) == 0) {  /* Handle fmiInstantiateSlave */
    jm_status_enu_t status = fmi2InstantiateSlaveWrapper(FMICSClient);
    if (status == jm_status_success) {
      sendCommand(client, fmiInstantiateSlaveSuccess, strlen(fmiInstantiateSlaveSuccess));
    } else if (status == jm_status_warning) {
      sendCommand(client, fmiInstantiateSlaveWarning, strlen(fmiInstantiateSlaveWarning));
    } else {
      sendCommand(client, fmiInstantiateSlaveError, strlen(fmiInstantiateSlaveError));
    }
//  } else if (strncmp(token, fmiInitializeSlave, strlen(fmiInitializeSlave)) == 0) {  /* Handle fmiInitializeSlave */
//    fmi1_status_t status = fmi1InitializeSlaveWrapper(FMICSClient);
//    if (status == fmi1_status_ok) {
//      sendCommand(client, fmiInitializeSlaveOk, strlen(fmiInitializeSlaveOk));
//    } else {
//      sendCommand(client, fmiInitializeSlaveError, strlen(fmiInitializeSlaveError));
//    }
//  } else if (strncmp(token, setInitialValues, strlen(setInitialValues)) == 0) {  /* Handle fmiInitializeSlave */
//    fmi1SetInitialValues(FMICSClient);
//    sendCommand(client, setInitialValuesOk, strlen(setInitialValuesOk));
//  } else if (strncmp(token, fmiGetValue, strlen(fmiGetValue)) == 0) {  /* Handle fmiGetValue */
//    char value[50];
//    fmi1GetValue(FMICSClient, unparseIntResult(token, fmiGetValue, strlen(token)), value);
//    char cmd[50];
//    sprintf(cmd, "%s%s", fmiGetValueReturn, value);
//    sendCommand(client, cmd, strlen(cmd));
//  } else if (strncmp(token, fmiSetValueVr, strlen(fmiSetValueVr)) == 0) {  /* Handle fmiSetValueVr */
//    /* split the fmisetvalue on #. the data is of form fmiSetValueVr=1#fmiSetValue=2.1 */
//    char* fmiSetValueVrStr = strtok((char*)token, "#");
//    char* fmiSetValueStr = strtok(NULL, " ");
//    /* fetch the results */
//    int vr = unparseIntResult(fmiSetValueVrStr, fmiSetValueVr, strlen(fmiSetValueVrStr));
//    /* do fmi1_import_set_real, fmi1_import_set_integer etc. */
//    fmi1SetValue(FMICSClient, vr, fmiSetValueStr, fmiSetValue);
//    /* tell server we are done setting value */
//    sendCommand(client, fmiSetValueReturn, strlen(fmiSetValueReturn));
//  } else if (strncmp(token, fmiDoStep, strlen(fmiDoStep)) == 0) {  /* Handle fmiDoStep */
//    int finished = 0;
//    fmi1_status_t status = fmi1DoStep(FMICSClient, &finished);
//    if (finished) {
//      sendCommand(client, fmiDoStepFinished, strlen(fmiDoStepFinished));
//    } else {
//      if (status == fmi1_status_ok) {
//        sendCommand(client, fmiDoStepOk, strlen(fmiDoStepOk));
//      } else if (status == fmi1_status_pending) {
//        /* ask the pending status from the FMU and print it as info */
//        fmi1_string_t str;
//        if (fmi1PendingStatusString(FMICSClient, &str) == fmi1_status_ok) {
//          logPrint(stdout, "INFO#%s\n", str);fflush(NULL);
//        }
//        /* tell the server that fmiDoStep is in pending state. */
//        sendCommand(client, fmiDoStepPending, strlen(fmiDoStepPending));
//      } else {
//        sendCommand(client, fmiDoStepError, strlen(fmiDoStepError));
//      }
//    }
//  } else if (strncmp(token, fmiDoStepStatus, strlen(fmiDoStepStatus)) == 0) {  // handle fmiDoStepStatus
//    fmi1_status_t status = fmi1_status_error;
//    fmi1DoStepStatus(FMICSClient, &status);
//    if (status == fmi1_status_ok) {
//      sendCommand(client, fmiDoStepOk, strlen(fmiDoStepOk));
//    } else if (status == fmi1_status_pending) {
//      /* ask the pending status from the FMU and print it as info */
//      fmi1_string_t str;
//      if (fmi1PendingStatusString(FMICSClient, &str) == fmi1_status_ok) {
//        logPrint(stdout, "INFO#%s\n", str);fflush(NULL);
//      }
//      /* tell the server that fmiDoStep is in pending state. */
//      sendCommand(client, fmiDoStepPending, strlen(fmiDoStepPending));
//    } else {
//      sendCommand(client, fmiDoStepError, strlen(fmiDoStepError));
//    }
//  } else if (strncmp(token, fmiTerminateSlave, strlen(fmiTerminateSlave)) == 0) {  // handle fmiTerminateSlave
//    destroyFMICoSimulationClient(FMICSClient);
//    lw_stream_close(client, lw_true);
  } else {
    debugPrint(debugFlag, stdout, "%s\n", token);fflush(NULL);
  }
}

void clientOnDisconnect(lw_client client) {
  logPrint(stdout, "Client disconnected\n");fflush(NULL);
  lw_pump pump = lw_stream_pump(client);
  lw_eventpump_post_eventloop_exit(pump);
  lw_stream_delete(client);
  lw_pump_delete(pump);
}

void clientOnError(lw_client client, lw_error error) {
  const char* errorString = lw_error_tostring(error);
  lw_pump pump = lw_stream_pump(client);
  FMICoSimulationClient *FMICSClient = (FMICoSimulationClient*)lw_stream_tag(client);

  if (strcmp(errorString, "Error connecting") == 0) {
    lw_addr address = lw_client_server_addr(client);
    logPrint(stderr, "Unable to connect to the server running at %s\n", lw_addr_tostring(address));fflush(NULL);
    destroyFMICoSimulationClient(FMICSClient);
    lw_eventpump_post_eventloop_exit(pump);
    lw_stream_delete(client);
    return;
  }
  logPrint(stderr, "Client received an error \"%s\"\n", errorString);fflush(NULL);
  destroyFMICoSimulationClient(FMICSClient);
  lw_eventpump_post_eventloop_exit(pump);
  lw_stream_delete(client);
}
