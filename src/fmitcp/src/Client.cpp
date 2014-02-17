#include "Client.h"
#include "Logger.h"

using namespace std;
using namespace fmitcp;

void clientOnConnect(lw_client c) {
    Client * client = (Client*)lw_stream_tag(c);
    client->clientConnected(c);
}
void clientOnData(lw_client c, const char* data, long size) {
    Client * client = (Client*)lw_stream_tag(c);
    client->clientData(c,data,size);
}
void clientOnDisconnect(lw_client c) {
    Client * client = (Client*)lw_stream_tag(c);
    client->clientDisconnected(c);
}
void clientOnError(lw_client c, lw_error error) {
    Client * client = (Client*)lw_stream_tag(c);
    client->clientError(c,error);
}

void Client::clientConnected(lw_client c){
    m_logger.log(Logger::NETWORK,"+ Connected to FMU server.\n");
    onConnect();
}

void Client::clientData(lw_client c, const char* data, long size){
    if(strcmp(data,"\n") == 0 || strcmp(data,"\0") == 0 || size<=1)
        return;

    // Construct message
    fmitcp_proto::fmitcp_message res;
    res.ParseFromString(data);
    fmitcp_proto::fmitcp_message_Type type = res.type();

    if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_slave_res){
        fmitcp_proto::fmi2_import_instantiate_slave_res * instantiateSlaveRes = res.mutable_fmi2_import_instantiate_slave_res();
        m_logger.log(Logger::NETWORK,
            "< fmi2_import_instantiate_slave_res(mid=%d,status=%d)\n",
            instantiateSlaveRes->message_id(),
            instantiateSlaveRes->status());
        on_fmi2_import_instantiate_slave_res(
            instantiateSlaveRes->message_id(),
            instantiateSlaveRes->status()
        );

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_slave_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_slave_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_reset_slave_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_slave_instance_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_input_derivatives_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_output_derivatives_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_cancel_step_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_res){
        fmitcp_proto::fmi2_import_do_step_res * doStepRes = res.mutable_fmi2_import_do_step_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_do_step_res(status=%d)\n",doStepRes->status());

        on_fmi2_import_do_step_res(
            doStepRes->message_id(),
            doStepRes->status()
        );

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_status_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_status_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_status_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_status_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_status_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_model_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_model_instance_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_time_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_continuous_states_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_integrator_step_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_model_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_derivatives_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_event_indicators_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_eventUpdate_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_event_iteration_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_continuous_states_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_nominal_continuous_states_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_version_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_debug_logging_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_integer_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_boolean_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_string_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_fmu_state_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_fmu_state_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_fmu_state_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialized_fmu_state_size_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialize_fmu_state_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_de_serialize_fmu_state_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_directional_derivative_res){
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_get_xml_res){
    } else {
        m_logger.log(Logger::ERROR,"Message type not recognized: %d!\n",type);
    }
}

void Client::clientDisconnected(lw_client c){
    m_logger.log(Logger::NETWORK,"- Disconnected from server.\n");
    onDisconnect();
}

void Client::clientError(lw_client c, lw_error error){
    string err = lw_error_tostring(error);
    m_logger.log(Logger::ERROR,"Error: %s\n",err.c_str());
    onError(err);
}

Client::Client(EventPump * pump){
    GOOGLE_PROTOBUF_VERIFY_VERSION;
    m_pump = pump;
    m_client = lw_client_new(m_pump->getPump());
}

Client::~Client(){
    lw_stream_delete(m_client);
    lw_pump_delete(m_pump->getPump());
    google::protobuf::ShutdownProtobufLibrary();
}

void Client::connect(string host, long port){

    // Set the master object as tag
    lw_stream_set_tag(m_client, (void*)this);

    // connect the event handlers
    lw_client_on_connect(   m_client, clientOnConnect);
    lw_client_on_data(      m_client, clientOnData);
    lw_client_on_disconnect(m_client, clientOnDisconnect);
    lw_client_on_error(     m_client, clientOnError);

    // connect the client to the server
    lw_client_connect(m_client, host.c_str(), port);
    fflush(NULL);

    m_logger.log(Logger::DEBUG,"Connecting to %s:%ld...\n",host.c_str(),port);
}

void Client::disconnect(){
    //lw_eventpump_post_eventloop_exit(m_pump->getPump());
    //lw_stream_close(m_client,lw_true);
    //lw_stream_delete(m_client);
    //lw_pump_delete(m_pump->getPump());
}

void Client::fmi2_import_do_step(int message_id,
                                 int fmuId,
                                 double currentCommunicationPoint,
                                 double communicationStepSize,
                                 bool newStep){
    //printf("Sending doStep!\n");fflush(NULL);

    // Construct message
    fmitcp_proto::fmitcp_message m;
    m.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_req);

    fmitcp_proto::fmi2_import_do_step_req * req = m.mutable_fmi2_import_do_step_req();
    req->set_message_id(message_id);
    req->set_fmuid(fmuId);
    req->set_currentcommunicationpoint(currentCommunicationPoint);
    req->set_communicationstepsize(communicationStepSize);
    req->set_newstep(newStep);

    m_logger.log(Logger::NETWORK,
        "> fmi2_import_do_step_req(mid=%d,fmu=%d,commPoint=%g,stepSize=%g,newStep=%d)\n",
        message_id,
        fmuId,
        currentCommunicationPoint,
        communicationStepSize,
        newStep);

    // Send
    string s;
    m.SerializeToString(&s);
    fflush(NULL);
    lw_stream_write(m_client, s.c_str(), s.size());
    fflush(NULL);
}

void Client::fmi2_import_instantiate_slave( int message_id, int fmuId,
                                            string instanceName,
                                            string resourceLocation,
                                            bool visible){
    //printf("Sending doStep!\n");fflush(NULL);

    // Construct message
    fmitcp_proto::fmitcp_message m;
    m.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_slave_req);

    fmitcp_proto::fmi2_import_instantiate_slave_req * req = m.mutable_fmi2_import_instantiate_slave_req();
    req->set_message_id(message_id);
    req->set_fmuid(fmuId);
    req->set_instancename(instanceName);
    req->set_fmuresourcelocation(resourceLocation);
    req->set_visible(visible);

    m_logger.log(Logger::NETWORK,
        "> fmi2_import_instantiate_slave_req(mid=%d,fmu=%d,instanceName=%s,resourceLocation=%s,visible=%d)\n",
        message_id,
        fmuId,
        instanceName.c_str(),
        resourceLocation.c_str(),
        visible);

    // Send
    string s;
    m.SerializeToString(&s);
    fflush(NULL);
    lw_stream_write(m_client, s.c_str(), s.size());
    fflush(NULL);
}
