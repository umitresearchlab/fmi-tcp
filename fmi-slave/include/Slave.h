#ifndef SLAVE_H_
#define SLAVE_H_

#include <string>
#include "fmilib.h"
#include "lacewing.h"

namespace fmitcp {

    class Slave {

    private:
        jm_log_level_enu_t m_logLevel;
        jm_callbacks m_jmCallbacks;
        fmi_import_context_t* m_context;
        fmi1_callback_functions_t m_fmi1CallbackFunctions;
        std::string m_workingDir;
        fmi_version_enu_t m_version;
        fmi1_import_t* m_fmi1Instance;
        std::string m_instanceName;
        bool m_debugLogging;
        std::string m_fmuLocation;
        std::string m_mimeType;
        bool m_visible;
        bool m_interactive;
        double m_tStart;
        double m_currentTime;
        double m_stepSize;
        bool m_stopTimeDefined;
        double m_tStop;
        fmi1_import_variable_list_t* m_fmi1Variables;
        std::string m_fmuPath;

        double m_timeOut;
        lw_pump m_pump;
        lw_server m_server;

    public:
        Slave(std::string fmuPath);
        virtual ~Slave();

        void host(std::string host, long port);
    };
};

#endif
