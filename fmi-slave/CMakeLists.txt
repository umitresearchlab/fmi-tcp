cmake_minimum_required(VERSION 2.8)

INCLUDE_DIRECTORIES(${INCLUDE_PATHS})
LINK_DIRECTORIES(${LIBS_PATHS})

SET(SRCS
    src/main.c
    src/utils.c
    src/parseargs.c
    src/help.c
    src/jacobi.c
    src/gs.c
    src/simulate.c
    src/log.c
)
SET(HEADERS
    src/main.h
    src/utils.h
    src/parseargs.h
    src/help.h
    src/jacobi.h
    src/gs.h
    src/simulate.h
    src/log.h
)

SET(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_LIST_DIR}/../bin")

ADD_EXECUTABLE(fmi-slave ${HEADERS} ${SRCS})

IF(WIN32)
TARGET_LINK_LIBRARIES(fmi-slave fmilib shlwapi)
ELSE(WIN32)
TARGET_LINK_LIBRARIES(fmi-slave fmilib dl rt)
ENDIF(WIN32)

INSTALL (TARGETS fmi-slave DESTINATION bin)
INSTALL (FILES ${CMAKE_CURRENT_LIST_DIR}/man/fmi-slave.1 DESTINATION man/man1)
