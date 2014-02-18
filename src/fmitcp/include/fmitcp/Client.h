#ifndef CLIENT_H_
#define CLIENT_H_

#include "EventPump.h"
#include "Logger.h"
#include "fmitcp.pb.h"
#include <string>
#include <vector>
#include <lacewing.h>

using namespace std;

namespace fmitcp {

    class Client {

    protected:
        EventPump * m_pump;
        Logger m_logger;

    private:
        lw_client m_client;

    public:
        Client(EventPump * pump);
        ~Client();

        void connect(string host, long port);
        void disconnect();
        Logger * getLogger();
        void sendMessage(fmitcp_proto::fmitcp_message message);

        /// To be implemented in subclass
        virtual void onConnect(){}

        /// To be implemented in subclass
        virtual void onDisconnect(){}

        /// To be implemented in subclass
        virtual void onError(string message){}

        void clientConnected(lw_client c);
        void clientData(lw_client c, const char* data, long size);
        void clientDisconnected(lw_client c);
        void clientError(lw_client c, lw_error error);

        // Response functions - to be implemented by the user
        virtual void onGetXmlRes(string xml){}
        virtual void on_fmi2_import_instantiate_slave_res               (int mid, fmitcp_proto::jm_status_enu_t status){}
        virtual void on_fmi2_import_initialize_slave_res                (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_terminate_slave_res                 (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_reset_slave_res                     (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_free_slave_instance_res             (int mid){}
        virtual void on_fmi2_import_set_real_input_derivatives_res      (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_real_output_derivatives_res     (int mid, fmitcp_proto::fmi2_status_t status, const vector<double>& values){}
        virtual void on_fmi2_import_cancel_step_res                     (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_do_step_res                         (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_status_res                      (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_real_status_res                 (int mid, double value){}
        virtual void on_fmi2_import_get_integer_status_res              (int mid, int value){}
        virtual void on_fmi2_import_get_boolean_status_res              (int mid, bool value){}
        virtual void on_fmi2_import_get_string_status_res               (int mid, string value){}
        virtual void on_fmi2_import_instantiate_model_res               (int mid, fmitcp_proto::jm_status_enu_t status){}
        virtual void on_fmi2_import_free_model_instance_res             (int mid){}
        virtual void on_fmi2_import_set_time_res                        (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_set_continuous_states_res           (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_completed_integrator_step_res       (int mid, bool callEventUpdate, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_initialize_model_res                (int mid, bool iterationConverged, bool stateValueReferencesChanged, bool stateValuesChanged, bool terminateSimulation, bool upcomingTimeEvent, double nextEventTime, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_derivatives_res                 (int mid, const vector<double>& derivatives, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_event_indicators_res            (int mid, const vector<double>& eventIndicators, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_eventUpdate_res                     (int mid, bool iterationConverged, bool stateValueReferencesChanged, bool stateValuesChanged, bool terminateSimulation, bool upcomingTimeEvent, double nextEventTime, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_completed_event_iteration_res       (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_continuous_states_res           (int mid, const vector<double>& states, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_nominal_continuous_states_res   (int mid, const vector<double>& nominal, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_terminate_res                       (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_version_res                     (int mid, string version){}
        virtual void on_fmi2_import_set_debug_logging_res               (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_set_real_res                        (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_set_integer_res                     (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_set_boolean_res                     (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_set_string_res                      (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_real_res                        (int mid, const vector<double>& values, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_integer_res                     (int mid, const vector<int>& values, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_boolean_res                     (int mid, const vector<bool>& values, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_string_res                      (int mid, const vector<string>& values, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_get_fmu_state_res                   (int mid, int stateId){}
        virtual void on_fmi2_import_set_fmu_state_res                   (int mid, fmitcp_proto::fmi2_status_t status){}
        virtual void on_fmi2_import_free_fmu_state_res                  (int mid, fmitcp_proto::fmi2_status_t status){}
        /*virtual void on_fmi2_import_serialized_fmu_state_size_res(){}
        virtual void on_fmi2_import_serialize_fmu_state_res(){}
        virtual void on_fmi2_import_de_serialize_fmu_state_res(){}
        virtual void on_fmi2_import_get_directional_derivative_res(){}*/

        // =========== FMI 2.0 (CS) Co-Simulation functions ===========
        void fmi2_import_instantiate_slave( int message_id, int fmuId,
                                            string instanceName,
                                            string resourceLocation,
                                            bool visible);
        void fmi2_import_initialize_slave(int mid, int fmuId, double relTol, double tStart, bool stopTimeDefined, double tStop);
        void fmi2_import_terminate_slave(int mid, int fmuId);
        void fmi2_import_reset_slave(int mid, int fmuId);
        void fmi2_import_free_slave_instance(int mid, int fmuId);
        void fmi2_import_set_real_input_derivatives(int mid, int fmuId, std::vector<int> valueRefs, std::vector<int> orders, std::vector<double> values);
        void fmi2_import_get_real_output_derivatives(int mid, int fmuId, std::vector<int> valueRefs, std::vector<int> orders);
        void fmi2_import_cancel_step(int mid, int fmuId);
        void fmi2_import_do_step(int message_id,
                                 int fmuId,
                                 double currentCommunicationPoint,
                                 double communicationStepSize,
                                 bool newStep);
        void fmi2_import_get_status        (int message_id, int fmuId, fmitcp_proto::fmi2_status_kind_t s);
        void fmi2_import_get_real_status   (int message_id, int fmuId, fmitcp_proto::fmi2_status_kind_t s);
        void fmi2_import_get_integer_status(int message_id, int fmuId, fmitcp_proto::fmi2_status_kind_t s);
        void fmi2_import_get_boolean_status(int message_id, int fmuId, fmitcp_proto::fmi2_status_kind_t s);
        void fmi2_import_get_string_status (int message_id, int fmuId, fmitcp_proto::fmi2_status_kind_t s);

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
