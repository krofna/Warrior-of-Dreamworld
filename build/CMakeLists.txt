cmake_minimum_required(VERSION 2.8)

project(Warrior-of-Dreamworld)

if (WIN32)
    set (Boost_USE_STATIC_LIBS TRUE)
endif()

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

if (HANDLER_TRACKING)
    add_definitions(-DBOOST_ASIO_ENABLE_HANDLER_TRACKING)
endif()

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++0x)
endif()

if (NOT BUILD_SERVER)
    set(BUILD_SERVER TRUE CACHE BOOL
    "Check it, if you want to build server"
    FORCE)
else (NOT BUILD_SERVER)
    add_subdirectory(Server)
endif()

if (NOT BUILD_CLIENT)
    set(BUILD_CLIENT TRUE CACHE BOOL
    "Check it, if you want to build client"
    FORCE)
else (NOT BUILD_CLIENT)
    add_subdirectory(Client)
endif()

if (NOT BUILD_WORLD_EDIT)
    set(BUILD_WORLD_EDIT TRUE CACHE BOOL
    "Check it, if you want to build world editor"
    FORCE)
else (NOT BUILD_WORLD_EDIT)
    add_subdirectory(WorldEdit)
endif()
