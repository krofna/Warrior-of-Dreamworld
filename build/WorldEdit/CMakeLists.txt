FIND_PACKAGE(Boost COMPONENTS system serialization REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

file(
GLOB
WORLD_EDIT_SRC
${CMAKE_SOURCE_DIR}/../world_edit/*
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
