#ifndef COMMON_H_
#define COMMON_H_

#define FMITCP_VERSION "0.0.1"

#include "fmitcp.pb.h"
#include <lacewing.h>

namespace fmitcp {

    void sendProtoBuffer(lw_client c, fmitcp_proto::fmitcp_message message);

}

#endif
