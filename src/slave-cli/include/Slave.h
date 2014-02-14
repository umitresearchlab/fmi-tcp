#ifndef SLAVE_H_
#define SLAVE_H_

#include <string>
//#include "Logger.h"
#include "fmilib.h"
#include "lacewing.h"

namespace fmitcp {

    /// FMI Slave.
    class Slave {

    private:
        // --- FMI Common ---

        /// FMU logging level
        jm_log_level_enu_t m_logLevel;
        jm_callbacks m_jmCallbacks;
        fmi_import_context_t* m_context;

        /// Directory for the unpacked FMU
        std::string m_workingDir;
        fmi_version_enu_t m_version;
        std::string m_instanceName;
        bool m_debugLogging;

        /// Location (uri) of the FMU
        std::string m_fmuLocation;
        std::string m_mimeType;
        bool m_visible;
        bool m_interactive;
        double m_tStart;
        double m_currentTime;
        double m_stepSize;
        bool m_stopTimeDefined;

        /// Simulation time to stop at.
        double m_tStop;

        /// Where the .fmu zip file is stored.
        std::string m_fmuPath;

        // --- FMI1 ---
        fmi1_callback_functions_t m_fmi1CallbackFunctions;
        fmi1_import_t* m_fmi1Instance;
        fmi1_import_variable_list_t* m_fmi1Variables;

        // --- TCP Server ---
        /// Event pump
        lw_pump m_pump;
        /// Slave server object
        lw_server m_server;
        /// The connected master client
        lw_client m_master;
        bool m_connected;
        double m_timeOut;

        //Logger m_logger;

    public:
        Slave(std::string fmuPath);
        virtual ~Slave();

        //void setLogger(const Logger& logger);

        /// Start the event pump and host at a given port
        void host(std::string host, long port);

        void onMasterConnect(lw_client client);
        void onMasterData(lw_client client, const char* data, size_t size);
        void onMasterDisconnect(lw_client client);
        void onError(lw_error error);
    };
};

#endif
