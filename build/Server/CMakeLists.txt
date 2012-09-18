cmake_minimum_required(VERSION 2.8)

Project(Server)

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo
            MinSizeRel."
            FORCE )
endif()

if (GLOBAL_COMPILATION)
    set(ADD_PATH .)
else (GLOBAL_COMPILATION)
    set(ADD_PATH ..)
endif (GLOBAL_COMPILATION)

add_definitions(-std=gnu++0x)

set(EXECUTABLE_OUTPUT_PATH ${ADD_PATH}/bin/Server/${CMAKE_BUILD_TYPE})
IF (NOT GLOBAL_COMPILATION)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../Modules)
ELSE (NOT GLOBAL_COMPILATION)
    set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ./Modules)
ENDIF(NOT GLOBAL_COMPILATION)

include(FindThreads)

set(MYSQLCONNECTORCPP_ROOT_DIR /home/ryan/Documents/Développement/mysql-connector-c++)

FIND_PACKAGE(Boost COMPONENTS system thread chrono REQUIRED)
FIND_PACKAGE(MySQLConnectorCPP)
FIND_PACKAGE(Threads)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

include_directories(${MYSQLCONNECTORCPP_INCLUDE_DIR})

file(
GLOB_RECURSE
SHARED_SRC
${ADD_PATH}/../shared/*
)
file(
GLOB_RECURSE
SCRIPT_SRC
${ADD_PATH}/../scripts/*
)

file(
GLOB_RECURSE
SERVER_SRC
${ADD_PATH}/../server/*.cpp
${ADD_PATH}/../server/*.hpp
)
file(
GLOB
SERVER_LIBS
${Boost_LIBRARIES}
${MYSQLCONNECTORCPP_LIBRARIES}
${CMAKE_THREAD_LIBS_INIT}
)

add_executable(Server
    ${SERVER_SRC}
    ${SCRIPT_SRC}
    ${SHARED_SRC})
target_link_libraries(Server
    ${SERVER_LIBS})
