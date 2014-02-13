#include <string>

namespace fmitcp {

    /// Packs and unpacks messages.
    class Message {


    public:

        Message();
        virtual ~Message();

        enum Type {
            message_none,
            message_tStart,
            message_tStartOk,
            message_stepSize,
            message_stepSizeOk,
            message_tEnd,
            message_tEndOk,
            message_instantiateSlave,
            message_instantiateSlaveError,
            message_instantiateSlaveSuccess,
            message_instantiateSlaveWarning,
            message_initializeSlave,
            message_initializeSlaveOk,
            message_initializeSlaveError,
            message_setInitialValues,
            message_setInitialValuesOk,
            message_doStep,
            message_doStepOk,
            message_doStepPending,
            message_doStepFinished,
            message_doStepError,
            message_doStepStatus,
            message_doStepStatusOk,
            message_doStepStatusPending,
            message_getValue,
            message_getValueReturn,
            message_setValueVr,
            message_setValue,
            message_setValueReturn,
            message_terminateSlave
        };

        /// Parse a message buffer. Returns true on success. Will set all data fields.
        bool parse(const char* data, long size);

        /// Serialize the message into a buffer. See getSize().
        void serialize(char* data);

        /// Get the size of the message, if it were to be serialized. See serialize()
        long getSize();

        /// Get the type of the message
        Type getType();

        /// Set the type of the message
        void setType(Type t);

        // Enable/disable binary transfer
        // void setEnableBinary(bool); // todo

        // Empty the message.
        void reset();

        static const std::string fmiTStart;
        static const std::string fmiTStartOk;
        static const std::string fmiStepSize;
        static const std::string fmiStepSizeOk;
        static const std::string fmiTEnd;
        static const std::string fmiTEndOk;
        static const std::string fmiInstantiateSlave;
        static const std::string fmiInstantiateSlaveError;
        static const std::string fmiInstantiateSlaveSuccess;
        static const std::string fmiInstantiateSlaveWarning;
        static const std::string fmiInitializeSlave;
        static const std::string fmiInitializeSlaveOk;
        static const std::string fmiInitializeSlaveError;
        static const std::string setInitialValues;
        static const std::string setInitialValuesOk;
        static const std::string fmiDoStep;
        static const std::string fmiDoStepOk;
        static const std::string fmiDoStepPending;
        static const std::string fmiDoStepFinished;
        static const std::string fmiDoStepError;
        static const std::string fmiDoStepStatus;
        static const std::string fmiDoStepStatusOk;
        static const std::string fmiDoStepStatusPending;
        static const std::string fmiGetValue;
        static const std::string fmiGetValueReturn;
        static const std::string fmiSetValueVr;
        static const std::string fmiSetValue;
        static const std::string fmiSetValueReturn;
        static const std::string fmiTerminateSlave;
        static const std::string waitingForFMUs;

    private:
        Type m_type;
    };

};


