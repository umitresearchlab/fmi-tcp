#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "Slave.h"

namespace fmitcp {

    enum ConnectionState {
      CONNECTION_INVALID,
      CONNECTION_REQUESTED,
      CONNECTION_COMPLETE
    };

    class Connection {

    protected:
        Slave* m_slaveA;
        Slave* m_slaveB;
        ConnectionState m_state;

    public:
        Connection(Slave* slaveA, Slave* slaveB);
        virtual ~Connection();

        ConnectionState getState();
        void setState(ConnectionState s);

        Slave * getSlaveA();
        Slave * getSlaveB();
    };
};

#endif
