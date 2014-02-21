#ifndef COMMON_H_
#define COMMON_H_

#define FMITCP_VERSION "0.0.1"

#include "fmitcp.pb.h"
#define lw_import
#include <lacewing.h>
#define FMILIB_BUILDING_LIBRARY
#include <fmilib.h>
#include <string>

namespace fmitcp {

    void sendProtoBuffer(lw_client c, fmitcp_proto::fmitcp_message * message);
    std::string dataToString(const char* data, long size);

    fmitcp_proto::jm_status_enu_t fmiJMStatusToProtoJMStatus(jm_status_enu_t status);
    fmitcp_proto::fmi2_status_t fmi2StatusToProtofmi2Status(fmi2_status_t status);

}

#endif
