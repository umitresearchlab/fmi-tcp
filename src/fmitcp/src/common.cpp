#include "common.h"
#include <vector>
#include <string>

void fmitcp::sendProtoBuffer(lw_client c, fmitcp_proto::fmitcp_message * message){
    std::string s;
    bool status = message->SerializeToString(&s);
    //printf("serialize status=%d\n", status);
    lw_stream_write(c, s.c_str(), s.size());
    std::string newline = "\n";
    lw_stream_write(c, newline.c_str(),newline.size());
    fflush(NULL);
}

std::string fmitcp::dataToString(const char* data, long size) {
    std::string data2(data, size);
    return data2;
    /*
    // Make this data safe!
    std::string data2;
    for(int i=0; i<size; i++){
        if(data[i] != '\0')
            data2.push_back(data[i]);
        else
            printf("contained null!\n");
    }
    return data2;
    */
}

fmitcp_proto::jm_status_enu_t fmitcp::fmiJMStatusToProtoJMStatus(jm_status_enu_t status) {
  switch (status) {
  case jm_status_error:
    return fmitcp_proto::jm_status_error;
  case jm_status_success:
    return fmitcp_proto::jm_status_success;
  case jm_status_warning:
    return fmitcp_proto::jm_status_warning;
  default: // should never be reached
    return fmitcp_proto::jm_status_error;
  }
}
