cmake_minimum_required(VERSION 2.8)

Project(WorldEdit)

if (GLOBAL_COMPILATION)
    set(ADD_PATH .)
else (GLOBAL_COMPILATION)
    set(ADD_PATH ..)
endif (GLOBAL_COMPILATION)

add_definitions(-std=gnu++0x)

set(EXECUTABLE_OUTPUT_PATH ${ADD_PATH}/bin/${CMAKE_BUILD_TYPE})

FIND_PACKAGE(Boost COMPONENTS system serialization REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})


file(
GLOB
WORLD_EDIT_SRC
${ADD_PATH}/../world_edit/*
)
file(
GLOB
WORLD_EDIT_LIBS
${Boost_LIBRARIES}
)

add_executable(WorldEdit
    ${WORLD_EDIT_SRC})
target_link_libraries(WorldEdit
    ${WORLD_EDIT_LIBS})
