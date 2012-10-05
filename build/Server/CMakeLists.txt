set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Server/${CMAKE_BUILD_TYPE})
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/Modules)

include(FindThreads)

set(MYSQLCONNECTORCPP_ROOT_DIR /home/ryan/Documents/Développement/mysql-connector-c++)

FIND_PACKAGE(Boost COMPONENTS system thread chrono REQUIRED)
FIND_PACKAGE(MySQLConnectorCPP)
FIND_PACKAGE(Threads)

link_directories(${Boost_LIBRARY_DIRS})

include_directories(
${CMAKE_BINARY_DIR}
${CMAKE_SOURCE_DIR}/..
${CMAKE_SOURCE_DIR}/../server/AI
${CMAKE_SOURCE_DIR}/../server/Entities/Item
${CMAKE_SOURCE_DIR}/../server/Entities/SpellBox
${CMAKE_SOURCE_DIR}/../server/Entities/WorldObject
${CMAKE_SOURCE_DIR}/../server/Map
${CMAKE_SOURCE_DIR}/../server/Server
${CMAKE_SOURCE_DIR}/../server/Server/Database
${CMAKE_SOURCE_DIR}/../server
${CMAKE_SOURCE_DIR}/../server/Chat
${Boost_INCLUDE_DIRS}
${MYSQLCONNECTORCPP_INCLUDE_DIR}
)

file(
GLOB_RECURSE
SHARED_SRC
${CMAKE_SOURCE_DIR}/../shared/*
)
file(
GLOB_RECURSE
SCRIPT_SRC
${CMAKE_SOURCE_DIR}/../scripts/*
)

file(
GLOB_RECURSE
SERVER_SRC
${CMAKE_SOURCE_DIR}/../server/*
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
