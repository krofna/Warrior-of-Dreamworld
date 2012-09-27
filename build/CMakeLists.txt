cmake_minimum_required(VERSION 2.8)

set (Boost_USE_STATIC_LIBS TRUE)

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE Debug CACHE STRING
            "Choose the type of build, options are: None Debug Release RelWithDebInfo
            MinSizeRel."
            FORCE )
endif()

if (CMAKE_COMPILER_IS_GNUCXX AND PROFILE_MODE)
    add_definitions(-pg)
    set (CMAKE_EXE_LINKER_FLAGS "-pg")
    set (CMAKE_BUILD_TYPE Debug)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

set(EXECUTABLE_OUTPUT_PATH ./bin/${CMAKE_BUILD_TYPE})

if (NOT BUILD_ONLY_SERVER OR NOT BUILD_ONLY_CLIENT OR NOT BUILD_ONLY_WORLD_EDIT)
    add_subdirectory(Server)
    add_subdirectory(Client)
    add_subdirectory(WorldEdit)
endif()

if (BUILD_ONLY_SERVER)
    add_subdirectory(Server)
endif()

if (BUILD_ONLY_CLIENT)
    add_subdirectory(Client)
endif()

if (BUILD_ONLY_WORLD_EDIT)
    add_subdirectory(WorldEdit)
endif()
