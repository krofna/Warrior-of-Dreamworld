cmake_minimum_required(VERSION 2.8)

Project(Client)

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
set(EXECUTABLE_OUTPUT_PATH ${ADD_PATH}/bin/Client/${CMAKE_BUILD_TYPE})

IF (NOT GLOBAL_COMPILATION)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ../Modules)
ELSE (NOT GLOBAL_COMPILATION)
    set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ./Modules)
ENDIF(NOT GLOBAL_COMPILATION)

include(FindThreads)
include(FindSFGUI)

FIND_PACKAGE(SFML COMPONENTS SYSTEM WINDOW GRAPHICS REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system thread serialization REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${SFML_LIBRARY_DIRS})
link_directories(${SFGUI_LIBRARY_DIRS})

include_directories(${SFML_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${SFGUI_INCLUDE_DIRS})

file(
GLOB_RECURSE
SHARED_SRC
${ADD_PATH}/../shared/*
)

file(
GLOB
CLIENT_SRC
${ADD_PATH}/../client/*.cpp
${ADD_PATH}/../client/*.hpp
)
file(
GLOB
CLIENT_LIBS
${SFML_LIBRARIES}
${Boost_LIBRARIES}
${CMAKE_THREAD_LIBS_INIT}
${SFGUI_LIBRARY}
)

add_executable(Client
    ${CLIENT_SRC}
    ${SHARED_SRC})
target_link_libraries(Client
    ${CLIENT_LIBS})
