#ifndef WEAKCONNECTION_H_
#define WEAKCONNECTION_H_

#include "Connection.h"

class WeakConnection : public Connection {
public:
    WeakConnection();
    ~WeakConnection();
};

#endif
