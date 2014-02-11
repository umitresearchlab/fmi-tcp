namespace fmitcp {

    class Message {

    public:

        /// Parse a message buffer. Returns true on success.
        bool parse(const char* data, long size);

        /// Serialize the message into a buffer. See getSize().
        void serialize(const char* data);

        /// Get the size of the message, if it were to be serialized. See serialize()
        long getSize();

        /// Get the type of the message
        int getType();

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
        static char* fmiDoStepPending = "fmiDoStep#pending";
        static char* fmiDoStepFinished = "fmiDoStep#finished";
        static char* fmiDoStepError = "fmiDoStep#error";
        static char* fmiDoStepStatus = "fmiDoStepStatus";
        static char* fmiDoStepStatusOk = "fmiDoStepStatus#ok";
        static char* fmiDoStepStatusPending = "fmiDoStepStatus#pending";
        static char* fmiGetValue = "fmiGetValue=";
        static char* fmiGetValueReturn = "fmiGetValueReturn=";
        static char* fmiSetValueVr = "fmiSetValueVr=";
        static char* fmiSetValue = "fmiSetValue=";
        static char* fmiSetValueReturn = "fmiSetValueReturn";
        static char* fmiTerminateSlave = "fmiTerminateSlave";

        // Information Commands
        static char* waitingForFMUs = "INFO#Master is waiting for %d more FMU(s) to connect.";
    };

};

