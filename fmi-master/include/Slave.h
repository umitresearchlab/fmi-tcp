#ifndef SLAVE_H_
#define SLAVE_H_

#include "lacewing.h"

enum SlaveState {
  NONE,
  INITIALIZED,
  STEPPING,
  STEPPINGFINISHED
};

class Slave {

private:
    SlaveState m_state;
    lw_client m_client;
    void sendCommand(char* data, size_t size);

public:

    Slave(lw_client);
    ~Slave();

    void doStep();
    lw_client getClient();
};

#endif
