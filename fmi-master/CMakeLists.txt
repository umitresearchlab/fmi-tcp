cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(../ ${FMIL_INCLUDE_DIR} ${LACEWING_INCLUDE_DIR})
LINK_DIRECTORIES(${FMIL_LIBS_DIR} ${LACEWING_LIBS_DIR})

SET(SRCS
    src/main.c
    src/parseargs.c
    src/fmiserver.c
    ../helper.c
)
SET(HEADERS
    src/main.h
    src/parseargs.h
    src/fmiserver.h
    ../commands.h
    ../helper.h
)

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../bin")

ADD_EXECUTABLE(fmi-master ${HEADERS} ${SRCS})

IF(WIN32)
TARGET_LINK_LIBRARIES(fmi-master fmilib shlwapi lacewing ws2_32 mswsock crypt32 secur32 mpr)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(fmi-master fmilib dl lacewing)
ENDIF(WIN32)

INSTALL (TARGETS fmi-master DESTINATION bin)
