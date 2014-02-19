#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <lacewing.h>
#include "EventPump.h"
#include "Logger.h"
#include "fmitcp.pb.h"

using namespace std;

namespace fmitcp {

    class Server {

    private:
        Logger m_logger;
        lw_server m_server;
        bool m_sendDummyResponses;

    protected:
        EventPump * m_pump;

    public:
        Server(EventPump * pump);
        Server(EventPump * pump, const Logger& logger);
        virtual ~Server();

        void init(EventPump * pump);

        /// To be implemented in subclass
        virtual void onClientConnect();

        /// To be implemented in subclass
        virtual void onClientDisconnect();

        /// To be implemented in subclass
        virtual void onError(string message);

        void clientConnected(lw_client c);
        void clientDisconnected(lw_client c);
        void clientData(lw_client c, const char* data, size_t size);
        void error(lw_server s, lw_error err);

        void host(string host, long port);
        void addFMU(string path);

        /// Set to true to start ignoring the local FMU and just send back dummy responses. Good for debugging the protocol.
        void sendDummyResponses(bool);

        void sendMessage(lw_client c, fmitcp_proto::fmitcp_message * message);

        Logger * getLogger();
        void setLogger(const Logger& logger);

    };

};

#endif
