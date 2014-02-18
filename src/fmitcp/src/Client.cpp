#include "Client.h"
#include "Logger.h"

using namespace std;
using namespace fmitcp;
using namespace fmitcp_proto;

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
    //fprintf(stderr,"client size:%ld\n",size);

    // Parse message
    fmitcp_message res;
    res.ParseFromString(data);
    /*
    if(!res.ParseFromString(data)){
        m_logger.log(Logger::ERROR,"Could not parse message!\n");
        //m_logger.log(Logger::DEBUG,"Got message of size:%ld\n",size);
        return;
    }
    */
    fmitcp_message_Type type = res.type();

    // Check type and run the corresponding event handler
    if(type == fmitcp_message_Type_type_fmi2_import_instantiate_slave_res){
        fmi2_import_instantiate_slave_res * r = res.mutable_fmi2_import_instantiate_slave_res();
        m_logger.log(Logger::NETWORK,
            "< fmi2_import_instantiate_slave_res(mid=%d,status=%d)\n",
            r->message_id(),
            r->status());
        on_fmi2_import_instantiate_slave_res( r->message_id(), r->status() );

    } else if(type == fmitcp_message_Type_type_fmi2_import_initialize_slave_res){
        fmi2_import_initialize_slave_res * r = res.mutable_fmi2_import_initialize_slave_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_initialize_slave_res(status=%d)\n",r->status());
        on_fmi2_import_initialize_slave_res(r->message_id(), r->status());

    } else if(type == fmitcp_message_Type_type_fmi2_import_terminate_slave_res){
        fmi2_import_terminate_slave_res * r = res.mutable_fmi2_import_terminate_slave_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_terminate_slave_res(status=%d)\n",r->status());
        on_fmi2_import_terminate_slave_res(r->message_id(), r->status());

    } else if(type == fmitcp_message_Type_type_fmi2_import_reset_slave_res){
        fmi2_import_reset_slave_res * r = res.mutable_fmi2_import_reset_slave_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_reset_slave_res(status=%d)\n",r->status());
        on_fmi2_import_reset_slave_res(r->message_id(), r->status());

    } else if(type == fmitcp_message_Type_type_fmi2_import_free_slave_instance_res){
        fmi2_import_free_slave_instance_res * r = res.mutable_fmi2_import_free_slave_instance_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_free_slave_instance_res()\n");
        on_fmi2_import_free_slave_instance_res(r->message_id());

    } else if(type == fmitcp_message_Type_type_fmi2_import_set_real_input_derivatives_res){
        fmi2_import_set_real_input_derivatives_res * r = res.mutable_fmi2_import_set_real_input_derivatives_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_set_real_input_derivatives_res()\n");
        on_fmi2_import_set_real_input_derivatives_res(r->message_id(),r->status());

    } else if(type == fmitcp_message_Type_type_fmi2_import_get_real_output_derivatives_res){
        fmi2_import_get_real_output_derivatives_res * r = res.mutable_fmi2_import_get_real_output_derivatives_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_get_real_output_derivatives_res()\n");
        on_fmi2_import_get_real_output_derivatives_res(r->message_id(),r->status(),vector<double>());

    } else if(type == fmitcp_message_Type_type_fmi2_import_cancel_step_res){
        fmi2_import_cancel_step_res * r = res.mutable_fmi2_import_cancel_step_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_cancel_step_res()\n");
        on_fmi2_import_cancel_step_res(r->message_id(),r->status());

    } else if(type == fmitcp_message_Type_type_fmi2_import_do_step_res){
        fmi2_import_do_step_res * r = res.mutable_fmi2_import_do_step_res();
        m_logger.log(Logger::NETWORK,"< fmi2_import_do_step_res(status=%d)\n",r->status());
        on_fmi2_import_do_step_res(r->message_id(), r->status());

    } else if(type == fmitcp_message_Type_type_fmi2_import_get_status_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_real_status_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_integer_status_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_boolean_status_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_string_status_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_instantiate_model_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_free_model_instance_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_time_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_continuous_states_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_completed_integrator_step_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_initialize_model_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_derivatives_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_event_indicators_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_eventUpdate_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_completed_event_iteration_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_continuous_states_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_nominal_continuous_states_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_terminate_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_version_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_debug_logging_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_real_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_integer_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_boolean_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_string_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_real_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_integer_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_boolean_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_string_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_fmu_state_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_set_fmu_state_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_free_fmu_state_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_serialized_fmu_state_size_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_serialize_fmu_state_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_de_serialize_fmu_state_res){
    } else if(type == fmitcp_message_Type_type_fmi2_import_get_directional_derivative_res){
    } else if(type == fmitcp_message_Type_type_get_xml_res){
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

Logger * Client::getLogger() {
    return &m_logger;
}

void Client::sendMessage(fmitcp_proto::fmitcp_message message){
    string s;
    message.SerializeToString(&s);
    lw_stream_write(m_client, s.c_str(), s.size());
    fflush(NULL);
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

    // Construct message
    fmitcp_message m;
    m.set_type(fmitcp_message_Type_type_fmi2_import_do_step_req);

    fmi2_import_do_step_req * req = m.mutable_fmi2_import_do_step_req();
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
    m_logger.log(Logger::DEBUG,"sending message of size:%d\n",s.size());
    lw_stream_write(m_client, s.c_str(), s.size());
    fflush(NULL);
}

void Client::fmi2_import_instantiate_slave( int message_id, int fmuId,
                                            string instanceName,
                                            string resourceLocation,
                                            bool visible){
    //printf("Sending doStep!\n");fflush(NULL);

    // Construct message
    fmitcp_message m;
    m.set_type(fmitcp_message_Type_type_fmi2_import_instantiate_slave_req);

    fmi2_import_instantiate_slave_req * req = m.mutable_fmi2_import_instantiate_slave_req();
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

    sendMessage(m);
}

void Client::fmi2_import_initialize_slave(int message_id, int fmuId, double relTol, double tStart, bool stopTimeDefined, double tStop){

    // Construct message
    fmitcp_message m;
    m.set_type(fmitcp_message_Type_type_fmi2_import_initialize_slave_req);

    fmi2_import_initialize_slave_req * req = m.mutable_fmi2_import_initialize_slave_req();
    req->set_message_id(message_id);
    req->set_fmuid(fmuId);
    req->set_relativetolerance(relTol);
    req->set_tstart(tStart);
    if(stopTimeDefined)
        req->set_tstop(tStop);

    m_logger.log(Logger::NETWORK,
        "> fmi2_import_initialize_slave_req(mid=%d,fmu=%d,relTol=%g,tStart=%g,tStop=%g)\n",
        message_id,
        fmuId,
        relTol,
        tStart,
        tStop);

    sendMessage(m);
}

void Client::fmi2_import_terminate_slave(int message_id, int fmuId){
    fmitcp_message m;
    m.set_type(fmitcp_message_Type_type_fmi2_import_terminate_slave_req);

    fmi2_import_terminate_slave_req * req = m.mutable_fmi2_import_terminate_slave_req();
    req->set_message_id(message_id);
    req->set_fmuid(fmuId);

    m_logger.log(Logger::NETWORK,
        "> fmi2_import_terminate_slave_req(mid=%d,fmu=%d)\n",
        message_id,
        fmuId);

    sendMessage(m);
}

void Client::fmi2_import_reset_slave(int message_id, int fmuId){
    fmitcp_message m;
    m.set_type(fmitcp_message_Type_type_fmi2_import_reset_slave_req);

    fmi2_import_reset_slave_req * req = m.mutable_fmi2_import_reset_slave_req();
    req->set_message_id(message_id);
    req->set_fmuid(fmuId);

    m_logger.log(Logger::NETWORK,
        "> fmi2_import_reset_slave_req(mid=%d,fmu=%d)\n",
        message_id,
        fmuId);

    sendMessage(m);
}
