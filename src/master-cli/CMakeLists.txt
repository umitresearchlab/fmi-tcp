cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(../ ${FMIL_INCLUDE_DIR} ${LACEWING_INCLUDE_DIR})
LINK_DIRECTORIES(${FMIL_LIBS_DIR} ${LACEWING_LIBS_DIR})

SET(MASTER_SRCS
    ../common/src/Logger.cpp
    ../common/src/Message.cpp
    src/Connection.cpp
    src/main.cpp
    src/Master.cpp
    src/Slave.cpp
    src/StrongConnection.cpp
    src/url_parser.c
    src/WeakConnection.cpp
)
SET(MASTER_HEADERS
    ../common/include/Logger.h
    ../common/include/Message.h
    include/Connection.h
    include/Master.h
    include/Slave.h
    include/StrongConnection.h
    include/url_parser.h
    include/WeakConnection.h
)

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../bin")

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(../common/include)

ADD_EXECUTABLE(master     ${MASTER_HEADERS} ${MASTER_SRCS})

IF(WIN32)
TARGET_LINK_LIBRARIES(master fmilib shlwapi lacewing ws2_32 mswsock crypt32 secur32 mpr)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(master fmilib dl lacewing)
ENDIF(WIN32)

