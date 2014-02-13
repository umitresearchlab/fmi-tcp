CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

EXECUTE_PROCESS(COMMAND protoc --cpp_out=common/proto/out common/proto/fmitcp.proto
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(fmi-master)
ADD_SUBDIRECTORY(fmi-slave)
