#include "Client.h"

using namespace std;
using namespace fmitcp;

void Client::onConnect(){}
void Client::onDisconnect(){}
void Client::onError(string err){}
void Client::onDoStepResponse(fmitcp_proto::fmi2_status_t status){}

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
    onConnect();
}

void Client::clientData(lw_client c, const char* data, long size){

    // Construct message
    fmitcp_proto::fmitcp_message m;
    m.ParseFromString(data);
    fmitcp_proto::fmitcp_message_Type type = m.type();

    if(type == fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_res){
        printf("doStep response!\n");

        fmitcp_proto::fmi2_import_do_step_res * res = m.mutable_fmi2_import_do_step_res();

        onDoStepResponse(res->status());
    }
}

void Client::clientDisconnected(lw_client c){
    onDisconnect();
}

void Client::clientError(lw_client c, lw_error error){
    string err = lw_error_tostring(error);
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
    fmitcp_proto::fmitcp_message m;
    m.set_type(fmitcp_proto::fmitcp_message_Type_type_fmi2_import_do_step_req);

    fmitcp_proto::fmi2_import_do_step_req * req = m.mutable_fmi2_import_do_step_req();
    req->set_message_id(message_id);
    req->set_fmuid(fmuId);
    req->set_currentcommunicationpoint(currentCommunicationPoint);
    req->set_communicationstepsize(communicationStepSize);
    req->set_newstep(newStep);

    // Send
    string s;
    m.SerializeToString(&s);
    lw_stream_write(m_client, s.c_str(), s.size());
}
