CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Compile proto
EXECUTE_PROCESS(COMMAND protoc --cpp_out=. proto/fmitcp.proto
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
