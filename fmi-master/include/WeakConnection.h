#ifndef WEAKCONNECTION_H_
#define WEAKCONNECTION_H_

#include "Connection.h"

namespace fmitcp {

    class WeakConnection : public Connection {

    protected:
        int m_valueRefA;
        int m_valueRefB;

    public:
        WeakConnection( Slave* slaveA,
                        Slave* slaveB,
                        int valueRefA,
                        int valueRefB );
        ~WeakConnection();

        int getValueRefA();
        int getValueRefB();

    };
};

#endif
