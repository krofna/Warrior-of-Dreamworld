cmake_minimum_required(VERSION 2.8)

Project(Client)

add_definitions(-std=gnu++0x)
set(EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE})

include(FindSFML)
include(FindBoost)
include(FindThreads)

FIND_PACKAGE(SFML COMPONENTS SYSTEM WINDOW GRAPHICS REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system thread REQUIRED)
FIND_PACKAGE(Threads)

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${SFML_LIBRARY_DIRS})

include_directories(${SFML_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

file(
GLOB_RECURSE
SHARED_SRC
../../shared/*
)

file(
GLOB
CLIENT_SRC
../../client/*.cpp
../../client/*.hpp
)
file(
GLOB
CLIENT_LIBS
${SFML_LIBRARIES}
${Boost_LIBRARIES}
${CMAKE_THREAD_LIBS_INIT}
)

add_executable(Client
    ${CLIENT_SRC}
    ${SHARED_SRC})
target_link_libraries(Client
    ${CLIENT_LIBS})
