#include <lacewing.h>
#include "Server.h"
#include "Logger.h"
#include "fmitcp.pb.h"

using namespace std;
using namespace fmitcp;

void Server::onClientConnect(){}
void Server::onClientDisconnect(){}
void Server::onError(string message){}

void serverOnConnect(lw_server s, lw_client c) {
    Server * server = (Server*)lw_server_tag(s);
    server->clientConnected(c);
}
void serverOnData(lw_server s, lw_client client, const char* data, size_t size) {
    Server * server = (Server*)lw_server_tag(s);
    server->clientData(client,data,size);
}
void serverOnDisconnect(lw_server s, lw_client c) {
    Server * server = (Server*)lw_server_tag(s);
    server->clientDisconnected(c);
}
void serverOnError(lw_server s, lw_error error) {
    Server * server = (Server*)lw_server_tag(s);
    server->error(s,error);
}

void Server::clientConnected(lw_client c){
    onClientConnect();
}
void Server::clientDisconnected(lw_client c){
    onClientDisconnect();
}
void Server::clientData(lw_client c, const char* data, size_t size){
    // Construct message
    fmitcp_proto::fmitcp_message m;
    m.ParseFromString(data);
    fmitcp_proto::fmitcp_message_Type type = m.type();


    /*
    switch(type){
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_slave_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_slave_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_slave_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_reset_slave_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_slave_instance_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_input_derivatives_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_output_derivatives_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_cancel_step_req: break;
    */
    if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_req){
        printf("Requested doStep\n");

        fmitcp_proto::fmitcp_message responseMessage;

        // Step here!
        responseMessage.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_res);

        fmitcp_proto::fmi2_status_t status = fmitcp_proto::fmi2_status_ok;
        fmitcp_proto::fmi2_import_do_step_res * res = responseMessage.mutable_fmi2_import_do_step_res();
        res->set_message_id(0);
        res->set_status(status);

        // Send
        string s;
        responseMessage.SerializeToString(&s);
        lw_stream_write(c, s.c_str(), s.size());
    }
    /*
        break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_status_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_status_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_status_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_status_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_status_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_model_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_model_instance_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_time_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_continuous_states_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_integrator_step_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_model_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_derivatives_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_event_indicators_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_eventUpdate_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_event_iteration_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_continuous_states_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_nominal_continuous_states_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_version_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_debug_logging_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_integer_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_boolean_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_string_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_fmu_state_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_fmu_state_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_fmu_state_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialized_fmu_state_size_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialize_fmu_state_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_de_serialize_fmu_state_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_directional_derivative_req: break;
    case fmitcp_proto::fmitcp_message_Type_type_get_xml_req: break;
    default:
        break;
    }
    */

    //lw_stream_write(m_client, s.c_str(), s.size());
}
void Server::error(lw_server s, lw_error error){
    string err = lw_error_tostring(error);
    onError(err);
}

Server::Server(EventPump * pump){
    init(pump);
}
Server::Server(EventPump * pump, const Logger& logger){
    m_logger = logger;
    init(pump);
}
void Server::init(EventPump * pump){
    m_pump = pump;
    m_server = lw_server_new(pump->getPump());
}

Server::~Server(){
    lw_server_delete(m_server);
}

void Server::addFMU(string path){

}

void Server::host(string hostName, long port){

    // save this object in the server tag so we can use it later on.
    lw_server_set_tag(m_server, (void*)this);

    // connect the hooks
    lw_server_on_connect(m_server, serverOnConnect);
    lw_server_on_data(m_server, serverOnData);
    lw_server_on_disconnect(m_server, serverOnDisconnect);
    lw_server_on_error(m_server, serverOnError);

    // setup the server host name and port
    lw_addr host = lw_addr_new_port(hostName.c_str(), port);
    lw_filter filter = lw_filter_new();
    lw_filter_set_ipv6(filter, lw_false);
    lw_filter_set_local(filter, host);

    // host/start the server
    lw_server_host_filter(m_server, filter);
    lw_filter_delete(filter);
}

Logger Server::getLogger() const {
    return m_logger;
}
