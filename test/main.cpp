#include <fmilib.h>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <fmitcp/Server.h>
#include <fmitcp/Client.h>
#include <fmitcp/common.h>

using namespace fmitcp;

// Define test client
class TestClient : public Client {
public:
    TestClient(EventPump* pump) : Client(pump) {};
    ~TestClient(){};

    void onConnect(){
        fmi2_import_do_step(0,0,0.0,0.1,true);
    };

    void on_fmi2_import_do_step_res(int message_id, fmitcp_proto::fmi2_status_t status){
        fmi2_import_instantiate_slave(0,0,"","",true);
    };

    void on_fmi2_import_instantiate_slave_res(int message_id, fmitcp_proto::jm_status_enu_t status){
        m_pump->exitEventLoop();
    }

    void onDisconnect(){
        m_logger.log(Logger::DEBUG,"TestClient::onDisconnect\n");
        m_pump->exitEventLoop();
    };

    void onError(string err){
        m_logger.log(Logger::DEBUG,"TestClient::onError\n");
        m_pump->exitEventLoop();
    };
};

int main(int argc, char const *argv[]){

    string hostName = "localhost"; // TODO: Specify via command line
    long port = 3123;
    EventPump pump;

    Server server(&pump);
    server.sendDummyResponses(true);
    server.host(hostName,port);
    server.getLogger()->setPrefix("Server: ");

    TestClient client(&pump);
    client.getLogger()->setPrefix("Client:        ");
    client.connect(hostName,port);
    pump.startEventLoop();

    return 0;
}
