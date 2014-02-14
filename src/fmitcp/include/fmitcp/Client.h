#ifndef CLIENT_H_
#define CLIENT_H_

#include "EventPump.h"
#include <string>
#include <fmilib.h>
#include <lacewing.h>

using namespace std;

namespace fmitcp {

    class Client {

    protected:
        EventPump * m_pump;

    private:
        lw_client m_client;

    public:
        Client(EventPump * pump);
        ~Client();

        void connect(string host, long port);
        void disconnect();

        /// To be implemented in subclass
        virtual void onConnect();

        /// To be implemented in subclass
        virtual void onDisconnect();

        /// To be implemented in subclass
        virtual void onError(string message);

        /// To be implemented in subclass
        virtual void onDoStepResponse();

        void clientConnected(lw_client c);
        void clientData(lw_client c, const char* data, long size);
        void clientDisconnected(lw_client c);
        void clientError(lw_client c, lw_error error);

        // =========== FMI 2.0 (CS) Co-Simulation functions ===========
        void fmi2_import_instantiate_slave();
        void fmi2_import_initialize_slave ();
        void fmi2_import_terminate_slave();
        void fmi2_import_free_slave_instance();
        void fmi2_import_set_real_input_derivatives();
        void fmi2_import_get_real_output_derivatives();
        void fmi2_import_cancel_step();
        void fmi2_import_do_step(int message_id,
                                 int fmuId,
                                 double currentCommunicationPoint,
                                 double communicationStepSize,
                                 bool newStep);
        void fmi2_import_get_status();
        void fmi2_import_get_real_status();
        void fmi2_import_get_integer_status();
        void fmi2_import_get_boolean_status();
        void fmi2_import_get_string_status();

        // =========== FMI 2.0 (ME) Model Exchange functions ===========
        void fmi2_import_instantiate_model();
        void fmi2_import_free_model_instance();
        void fmi2_import_set_time();
        void fmi2_import_set_continuous_states();
        void fmi2_import_completed_integrator_step();
        void fmi2_import_initialize_model();
        void fmi2_import_get_derivatives();
        void fmi2_import_get_event_indicators();
        void fmi2_import_eventUpdate();
        void fmi2_import_completed_event_iteration();
        void fmi2_import_get_continuous_states();
        void fmi2_import_get_nominal_continuous_states();
        void fmi2_import_terminate();

        // ========= FMI 2.0 CS & ME COMMON FUNCTIONS ============
        void fmi2_import_get_version();
        void fmi2_import_set_debug_logging();
        void fmi2_import_set_real();
        void fmi2_import_set_integer();
        void fmi2_import_set_boolean();
        void fmi2_import_set_string();
        void fmi2_import_get_real();
        void fmi2_import_get_integer();
        void fmi2_import_get_boolean();
        void fmi2_import_get_string();
        void fmi2_import_get_fmu_state();
        void fmi2_import_set_fmu_state();
        void fmi2_import_free_fmu_state();
        void fmi2_import_serialized_fmu_state_size();
        void fmi2_import_serialize_fmu_state();
        void fmi2_import_de_serialize_fmu_state();
        void fmi2_import_get_directional_derivative();

        // ========= NETWORK SPECIFIC FUNCTIONS ============
        void get_xml();
    };

};

#endif
