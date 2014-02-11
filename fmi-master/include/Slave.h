#ifndef SLAVE_H_
#define SLAVE_H_

#include "lacewing.h"

namespace fmitcp {

    enum SlaveState {
        SLAVE_NONE,
        SLAVE_INITIALIZED,
        SLAVE_STEPPING,
        SLAVE_STEPPINGFINISHED
    };

    class Slave {

    private:
        SlaveState m_state;
        lw_client m_client;
        int m_id;
        void sendCommand(char* data, size_t size);

    public:

        Slave(lw_client);
        ~Slave();

        void doStep();
        lw_client getClient();
        int getId();
        void setId(int id);
        void setState(SlaveState s);
        SlaveState getState();
        void initialize();
        void instantiate();
        void setInitialValues();
        void terminate();
        void getReal(int valueRef);
        void setReal(int valueRef, double value);

    };
};

#endif
