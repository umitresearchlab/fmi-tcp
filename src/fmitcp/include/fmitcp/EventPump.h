#ifndef EVENTPUMP_H_
#define EVENTPUMP_H_

#include <lacewing.h>

namespace fmitcp {

    class EventPump {
    private:
        lw_pump m_pump;
        bool m_exiting;

    public:
        EventPump();
        ~EventPump();
        lw_pump getPump();
        void startEventLoop();
        void exitEventLoop();
    };

};

#endif
