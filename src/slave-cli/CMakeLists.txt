cmake_minimum_required(VERSION 2.8)

# Protobuf
INCLUDE(FindProtobuf)
FIND_PACKAGE(Protobuf REQUIRED)
INCLUDE_DIRECTORIES(${PROTOBUF_INCLUDE_DIR})

FIND_PACKAGE(Threads REQUIRED)

INCLUDE_DIRECTORIES(../../lib ../ ${FMIL_INCLUDE_DIR} ${LACEWING_INCLUDE_DIR})
LINK_DIRECTORIES(${FMIL_LIBS_DIR} ${LACEWING_LIBS_DIR})

SET(SLAVE_SRCS
    src/main.cpp
)
SET(SLAVE_HEADERS
)

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../bin")

INCLUDE_DIRECTORIES(include)
INCLUDE_DIRECTORIES(../fmitcp/include)

ADD_EXECUTABLE(slave     ${SLAVE_HEADERS}   ${SLAVE_SRCS})

IF(WIN32)
    SET(WINLIBS
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
    TARGET_LINK_LIBRARIES(slave     ${WINLIBS})
ELSE(WIN32)
    SET(LINUXLIBS
        fmitcp
        fmilib
        dl
        lacewing
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        ${PROTOBUF_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
    )
    TARGET_LINK_LIBRARIES(slave     ${LINUXLIBS})
ENDIF(WIN32)

INSTALL (TARGETS slave      DESTINATION bin)
