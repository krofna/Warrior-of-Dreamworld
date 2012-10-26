set (EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/Client/${CMAKE_BUILD_TYPE})
set (CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/Modules)

include(FindThreads)
include(FindSFGUI)

FIND_PACKAGE(SFML COMPONENTS SYSTEM WINDOW GRAPHICS REQUIRED)
FIND_PACKAGE(Boost COMPONENTS system thread serialization REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})
link_directories(${SFML_LIBRARY_DIRS})
link_directories(${SFGUI_LIBRARY_DIRS})

include_directories(
${CMAKE_BINARY_DIR}
${SFML_INCLUDE_DIRS}
${Boost_INCLUDE_DIRS}
${SFGUI_INCLUDE_DIRS}
${CMAKE_SOURCE_DIR}/../client/Session
${CMAKE_SOURCE_DIR}/../client/
${CMAKE_SOURCE_DIR}/..
)

file(
GLOB_RECURSE
SHARED_SRC
${CMAKE_SOURCE_DIR}/../shared/*
)

file(
GLOB_RECURSE
CLIENT_SRC
${CMAKE_SOURCE_DIR}/../client/*
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
