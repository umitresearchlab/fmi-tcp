#include "Server.h"
#include <lacewing.h>

using namespace std;
using namespace fmitcp;

void Server::onClientConnect(){}
void Server::onClientDisconnect(){}

void serverOnConnect(lw_server s, lw_client c) {
    printf("serverOnConnect\n");
    Server * server = (Server*)lw_server_tag(s);
    server->clientConnected(c);
}
void serverOnData(lw_server s, lw_client client, const char* data, size_t size) {
    printf("serverOnData\n");
    Server * server = (Server*)lw_server_tag(s);
    server->clientData(client,data,size);
}
void serverOnDisconnect(lw_server s, lw_client c) {
    printf("serverOnDisconnect\n");
    Server * server = (Server*)lw_server_tag(s);
    server->clientDisconnected(c);
}
void serverOnError(lw_server s, lw_error error) {
    printf("serverOnError\n");
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
}
void Server::error(lw_server s, lw_error error){
    printf("Server::clientError\n");
    string err = lw_error_tostring(error);
    onError(err);
}

Server::Server(EventPump * pump){
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

    printf("Hosting %s:%ld\n",hostName.c_str(),port);
}
