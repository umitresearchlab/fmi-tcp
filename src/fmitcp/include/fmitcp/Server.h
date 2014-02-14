#ifndef SERVER_H_
#define SERVER_H_

#include <string>
#include <lacewing.h>
#include "EventPump.h"

using namespace std;

namespace fmitcp {

    class Server {

    private:
        lw_server m_server;

    protected:
        EventPump * m_pump;

    public:
        Server(EventPump * pump);
        virtual ~Server();

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

    };

};

#endif
