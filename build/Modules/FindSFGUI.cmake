cmake_minimum_required( VERSION 2.8 )

set( SFGUI_FOUND FALSE )

if( SFGUI_STATIC_LIBRARIES )
	set( SFGUI_SUFFIX "-s" )
	add_definitions( -DSFGUI_STATIC )
else()
	set( SFGUI_SUFFIX "" )
endif()

find_path(
	SFGUI_INCLUDE_DIR
	SFGUI/Config.hpp
	PATH_SUFFIXES
		include
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
)

find_library(
	SFGUI_LIBRARY_RELEASE
	sfgui${SFGUI_SUFFIX}
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
)

find_library(
	SFGUI_LIBRARY_DEBUG
	sfgui${SFGUI_SUFFIX}-d
	PATH_SUFFIXES
		lib
		lib64
	PATHS
		/usr
		/usr/local
		${SFGUIDIR}
)

if( SFGUI_LIBRARY_RELEASE AND SFGUI_LIBRARY_DEBUG )
	set( SFGUI_LIBRARY debug ${SFGUI_LIBRARY_DEBUG} optimized ${SFGUI_LIBRARY_RELEASE} )
endif()

if( SFGUI_LIBRARY_RELEASE AND NOT SFGUI_LIBRARY_DEBUG )
	set( SFGUI_LIBRARY_DEBUG ${SFGUI_LIBRARY_RELEASE} )
	set( SFGUI_LIBRARY ${SFGUI_LIBRARY_RELEASE} )
endif()

if( SFGUI_LIBRARY_DEBUG AND NOT SFGUI_LIBRARY_RELEASE )
	set( SFGUI_LIBRARY_RELEASE ${SFGUI_LIBRARY_DEBUG} )
	set( SFGUI_LIBRARY ${SFGUI_LIBRARY_DEBUG} )
endif()

if( NOT SFGUI_INCLUDE_DIR OR NOT SFGUI_LIBRARY )
	message( FATAL_ERROR "SFGUI not found." )
else()
	message( "SFGUI found: ${SFGUI_INCLUDE_DIR}" )
endif()