SET(HEADERS
    include/fmitcp/Client.h
    include/fmitcp/Server.h
    include/fmitcp/EventPump.h
    src/fmitcp.pb.h
)
SET(SRCS
    src/Client.cpp
    src/Server.cpp
    src/EventPump.cpp
    src/fmitcp.pb.cc
)

# Compile proto
EXECUTE_PROCESS(COMMAND protoc --cpp_out=. fmitcp.proto
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/src)

INCLUDE_DIRECTORIES(include/fmitcp ${FMIL_INCLUDE_DIR} ${LACEWING_INCLUDE_DIR})
LINK_DIRECTORIES(${FMIL_LIBS_DIR} ${LACEWING_LIBS_DIR})

ADD_LIBRARY(fmitcp ${HEADERS} ${SRCS})

SET_TARGET_PROPERTIES( fmitcp
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
