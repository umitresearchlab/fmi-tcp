#ifndef COMMON_H_
#define COMMON_H_

#define FMITCP_VERSION "0.0.1"

#include "fmitcp.pb.h"
#define lw_import
#include <lacewing.h>
#include <string>

namespace fmitcp {

    void sendProtoBuffer(lw_client c, fmitcp_proto::fmitcp_message * message);
    std::string dataToString(const char* data, long size);

}

#endif
