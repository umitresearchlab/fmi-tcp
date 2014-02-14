cmake_minimum_required(VERSION 2.8)

# Protobuf
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})

FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(../ ${FMIL_INCLUDE_DIR} ${LACEWING_INCLUDE_DIR})
LINK_DIRECTORIES(${FMIL_LIBS_DIR} ${LACEWING_LIBS_DIR})

SET(MASTER_SRCS
    #../common/src/Logger.cpp
    #../common/src/Message.cpp
    #src/Connection.cpp
    src/main.cpp
    #src/Master.cpp
    #src/Slave.cpp
    #src/StrongConnection.cpp
    #src/url_parser.c
    #src/WeakConnection.cpp
)
SET(MASTER_HEADERS
    include/Logger.h
    #include/Connection.h
    #include/Master.h
    #include/Slave.h
    #include/StrongConnection.h
    #include/url_parser.h
    #include/WeakConnection.h
)

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../bin")

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(../fmitcp/include)

ADD_EXECUTABLE(master     ${MASTER_HEADERS} ${MASTER_SRCS})

IF(WIN32)
    TARGET_LINK_LIBRARIES(master
        fmitcp
        fmilib
        shlwapi
        lacewing
        ws2_32
        mswsock
        crypt32
        secur32
        mpr
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${PROTOBUF_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
    )
ELSE(WIN32)
    TARGET_LINK_LIBRARIES(master
        fmitcp
        fmilib
        dl
        lacewing
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${PROTOBUF_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
    )
ENDIF(WIN32)

