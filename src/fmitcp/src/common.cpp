#include "common.h"
#include <vector>

void fmitcp::sendProtoBuffer(lw_client c, fmitcp_proto::fmitcp_message message){
    std::string s;
    message.SerializeToString(&s);
    lw_stream_write(c, s.c_str(), s.size());
    //lw_stream_write(c, "\n", 1);
    //fflush(NULL);
}
