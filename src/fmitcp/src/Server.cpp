#include <lacewing.h>
#include "Server.h"
#include "Logger.h"
#include "common.h"
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
    m_logger.log(Logger::NETWORK,"+ Client connected.\n");
    onClientConnect();
}
void Server::clientDisconnected(lw_client c){
    m_logger.log(Logger::NETWORK,"- Client disconnected.\n");
    onClientDisconnect();
}
void Server::clientData(lw_client c, const char* data, size_t size){
    if(strcmp(data,"\n") == 0)
        return;

    // Construct message
    fmitcp_proto::fmitcp_message req;
    req.ParseFromString(data);
    string s = data;
    fmitcp_proto::fmitcp_message_Type type = req.type();

    fmitcp_proto::fmitcp_message res;
    bool sendResponse = true;

    if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_slave_req){

        // Unpack message
        fmitcp_proto::fmi2_import_instantiate_slave_req * r = req.mutable_fmi2_import_instantiate_slave_req();
        int fmuId = r->fmuid();
        int messageId = r->message_id();
        string instanceName = r->instancename();
        string fmuResourceLocation = r->fmuresourcelocation();
        bool visible = r->visible();

        m_logger.log(Logger::NETWORK,"< fmi2_import_instantiate_slave_req(fmuId=%d,instanceName=%s,resourceLoc=%s,visible=%d)\n",fmuId,instanceName.c_str(),fmuResourceLocation.c_str(),visible);

        if(!m_sendDummyResponses){
            // instantiate FMU here
            // TODO
        }

        // Create response message
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_slave_res);
        fmitcp_proto::jm_status_enu_t status = fmitcp_proto::jm_status_success;
        fmitcp_proto::fmi2_import_instantiate_slave_res * instantiateRes = res.mutable_fmi2_import_instantiate_slave_res();
        instantiateRes->set_message_id(messageId);
        instantiateRes->set_status(status);
        m_logger.log(Logger::NETWORK,"> fmi2_import_instantiate_slave_res(mid=%d,status=%d)\n",1,status);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_slave_req){

        // Unpack message
        fmitcp_proto::fmi2_import_initialize_slave_req * r = req.mutable_fmi2_import_initialize_slave_req();
        int fmuId = r->fmuid();
        int messageId = r->message_id();
        double relTol = r->relativetolerance();
        double tStart = r->tstart();
        double tStop = 0.0;
        bool tStopGiven = r->has_tstop();

        m_logger.log(Logger::NETWORK,"< fmi2_import_initialize_slave_req(fmuId=%d,relTol=%g,tStart=%g,tStop=%g)\n",fmuId,relTol,tStart,tStop);

        if(!m_sendDummyResponses){
            // initialize FMU here
            // TODO
        }

        // Create response message
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_slave_res);
        fmitcp_proto::fmi2_status_t status = fmitcp_proto::fmi2_status_ok;
        fmitcp_proto::fmi2_import_initialize_slave_res * initializeRes = res.mutable_fmi2_import_initialize_slave_res();
        initializeRes->set_message_id(messageId);
        initializeRes->set_status(status);
        m_logger.log(Logger::NETWORK,"> fmi2_import_initialize_slave_res(mid=%d,status=%d)\n",1,status);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_slave_req){

        // Unpack message
        fmitcp_proto::fmi2_import_terminate_slave_req * r = req.mutable_fmi2_import_terminate_slave_req();
        int fmuId = r->fmuid();
        int messageId = r->message_id();

        m_logger.log(Logger::NETWORK,"< fmi2_import_terminate_slave_req(fmuId=%d)\n",fmuId);

        if(!m_sendDummyResponses){
            // terminate FMU here
            // TODO
        }

        // Create response message
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_slave_res);
        fmitcp_proto::fmi2_status_t status = fmitcp_proto::fmi2_status_ok;
        fmitcp_proto::fmi2_import_terminate_slave_res * terminateRes = res.mutable_fmi2_import_terminate_slave_res();
        terminateRes->set_message_id(messageId);
        terminateRes->set_status(status);
        m_logger.log(Logger::NETWORK,"> fmi2_import_terminate_slave_res(mid=%d,status=%d)\n",messageId,status);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_reset_slave_req){

        // Unpack message
        fmitcp_proto::fmi2_import_reset_slave_req * r = req.mutable_fmi2_import_reset_slave_req();
        int fmuId = r->fmuid();
        int messageId = r->message_id();

        m_logger.log(Logger::NETWORK,"< fmi2_import_reset_slave_req(fmuId=%d)\n",fmuId);

        // Create response message
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_reset_slave_res);
        fmitcp_proto::fmi2_import_reset_slave_res * resetRes = res.mutable_fmi2_import_reset_slave_res();
        resetRes->set_message_id(messageId);
        resetRes->set_status(fmitcp_proto::fmi2_status_ok);

        if(!m_sendDummyResponses){
            // reset FMU here
            // TODO
        }

        m_logger.log(Logger::NETWORK,"> fmi2_import_reset_slave_res(mid=%d,status=%d)\n",messageId,resetRes->status());

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_slave_instance_req){

        // Unpack message
        fmitcp_proto::fmi2_import_free_slave_instance_req * r = req.mutable_fmi2_import_free_slave_instance_req();
        int fmuId = r->fmuid(),
            messageId = r->message_id();

        m_logger.log(Logger::NETWORK,"< fmi2_import_free_slave_instance_req(fmuId=%d)\n",fmuId);

        // Create response message
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_slave_instance_res);
        fmitcp_proto::fmi2_import_free_slave_instance_res * resetRes = res.mutable_fmi2_import_free_slave_instance_res();
        resetRes->set_message_id(messageId);

        if(!m_sendDummyResponses){
            // Interact with FMU here
            // TODO
        }

        m_logger.log(Logger::NETWORK,"> fmi2_import_free_slave_instance_res(mid=%d)\n",messageId);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_input_derivatives_req){

        // Unpack message
        fmitcp_proto::fmi2_import_set_real_input_derivatives_req * r = req.mutable_fmi2_import_set_real_input_derivatives_req();
        int fmuId = r->fmuid(),
            messageId = r->message_id(),
            numValueRefs = r->valuereferences_size(),
            numValues = r->values_size();

        m_logger.log(Logger::NETWORK,"< fmi2_import_set_real_input_derivatives_req(mid=%d,fmuId=%d)\n",messageId,fmuId);

        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_input_derivatives_res);
        fmitcp_proto::fmi2_import_set_real_input_derivatives_res * resetRes = res.mutable_fmi2_import_set_real_input_derivatives_res();
        resetRes->set_message_id(messageId);
        resetRes->set_status(fmitcp_proto::fmi2_status_ok);

        if(!m_sendDummyResponses){

            // Get values
            for(int i=0; i<numValues; i++){
                r->values(i);
                r->valuereferences(i);
            }

            // Interact with FMU here
            // TODO
        }

        m_logger.log(Logger::NETWORK,"> fmi2_import_set_real_input_derivatives_res(mid=%d)\n",messageId);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_output_derivatives_req){

        // Unpack message
        fmitcp_proto::fmi2_import_get_real_output_derivatives_req * r = req.mutable_fmi2_import_get_real_output_derivatives_req();
        int fmuId = r->fmuid(),
            messageId = r->message_id(),
            numValueRefs = r->valuereferences_size();

        m_logger.log(Logger::NETWORK,"< fmi2_import_get_real_output_derivatives_req(mid=%d,fmuId=%d)\n",messageId,fmuId);

        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_output_derivatives_res);
        fmitcp_proto::fmi2_import_get_real_output_derivatives_res * resetRes = res.mutable_fmi2_import_get_real_output_derivatives_res();
        resetRes->set_message_id(messageId);
        resetRes->set_status(fmitcp_proto::fmi2_status_ok);

        if(!m_sendDummyResponses){

            // Set values
            for(int i=0; i<numValueRefs; i++){
                resetRes->set_values(i,0.0f); // Just testing
            }

            // Interact with FMU here
            // TODO
        }

        m_logger.log(Logger::NETWORK,"> fmi2_import_get_real_output_derivatives_res(mid=%d)\n",messageId);

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_cancel_step_req){

        // Unpack message
        fmitcp_proto::fmi2_import_cancel_step_req * r = req.mutable_fmi2_import_cancel_step_req();
        int fmuId = r->fmuid(),
            messageId = r->message_id();

        m_logger.log(Logger::NETWORK,"< fmi2_import_cancel_step_req(mid=%d,fmuId=%d)\n",messageId,fmuId);

        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_cancel_step_res);
        fmitcp_proto::fmi2_import_cancel_step_res * resetRes = res.mutable_fmi2_import_cancel_step_res();
        resetRes->set_message_id(messageId);
        resetRes->set_status(fmitcp_proto::fmi2_status_ok);

        if(!m_sendDummyResponses){

            // Interact with FMU here
            // TODO
        }

        m_logger.log(Logger::NETWORK,"> fmi2_import_cancel_step_res(mid=%d,status=%d)\n",messageId,resetRes->status());

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_req){

        // Unpack message
        fmitcp_proto::fmi2_import_do_step_req * r = req.mutable_fmi2_import_do_step_req();
        int fmuId = r->fmuid();
        double currentCommunicationPoint = r->currentcommunicationpoint(),
               communicationStepSize = r->communicationstepsize();
        bool newStep = r->newstep();
        m_logger.log(Logger::NETWORK,"< fmi2_import_do_step_req(fmuId=%d,commPoint=%g,stepSize=%g,newStep=%d)\n",fmuId,currentCommunicationPoint,communicationStepSize,newStep?1:0);

        // Defaults
        fmitcp_proto::fmi2_import_do_step_res * doStepRes = res.mutable_fmi2_import_do_step_res();
        res.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_res);
        doStepRes->set_message_id(r->message_id());
        doStepRes->set_status(fmitcp_proto::fmi2_status_ok);

        if(!m_sendDummyResponses){
            // TODO: Step the FMU
        }

        // Create response
        m_logger.log(Logger::NETWORK,"> fmi2_import_do_step_res(status=%d)\n",doStepRes->status());

    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_status_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_status_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_status_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_status_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_status_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_instantiate_model_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_model_instance_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_time_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_continuous_states_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_integrator_step_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_initialize_model_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_derivatives_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_event_indicators_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_eventUpdate_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_completed_event_iteration_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_continuous_states_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_nominal_continuous_states_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_terminate_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_version_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_debug_logging_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_real_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_integer_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_boolean_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_string_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_real_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_integer_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_boolean_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_string_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_fmu_state_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_set_fmu_state_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_free_fmu_state_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialized_fmu_state_size_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_serialize_fmu_state_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_de_serialize_fmu_state_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_get_directional_derivative_req){
        // TODO
        sendResponse = false;
    } else if(type == fmitcp_proto::fmitcp_message_Type_type_get_xml_req){
        // TODO
        sendResponse = false;
    } else {
        // Something is wrong.
        sendResponse = false;
        m_logger.log(Logger::ERROR,"Message type not recognized: %d.\n",type);
    }

    if(sendResponse){
        sendMessage(c,res);
    }
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
    m_sendDummyResponses = false;
}

Server::~Server(){
    lw_server_delete(m_server);
}

void Server::addFMU(string path){

}

void Server::sendMessage(lw_client c, fmitcp_proto::fmitcp_message message){
   fmitcp::sendProtoBuffer(c,message);
}

void Server::sendDummyResponses(bool sendDummyResponses){
    m_sendDummyResponses = sendDummyResponses;
}

void Server::host(string hostName, long port){

    // save this object in the server tag so we can use it later on.
    lw_server_set_tag(m_server, (void*)this);

    // connect the hooks
    lw_server_on_connect(   m_server, serverOnConnect);
    lw_server_on_data(      m_server, serverOnData);
    lw_server_on_disconnect(m_server, serverOnDisconnect);
    lw_server_on_error(     m_server, serverOnError);

    // setup the server host name and port
    lw_addr host = lw_addr_new_port(hostName.c_str(), port);
    lw_filter filter = lw_filter_new();
    lw_filter_set_ipv6(filter, lw_false);
    lw_filter_set_local(filter, host);

    // host/start the server
    lw_server_host_filter(m_server, filter);
    lw_filter_delete(filter);

    m_logger.log(Logger::NETWORK,"Listening to %s:%ld\n",hostName.c_str(),port);
}

Logger * Server::getLogger() {
    return &m_logger;
}
