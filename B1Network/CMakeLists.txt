# Sources
file(GLOB SRCS_G "*.cpp")
MACRO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB HDRS_G "*.h")
MACRO_HEADERS_AUTO(SRCS ${HDRS_G})

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost ${REQUIRED_BOOST_VERSION})

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_library(B1Network ${SRCS})
if(MSVC)
	target_precompile_headers(B1Network
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/B1Network.h>
	)
endif()

if(MSVC)
	set_target_properties (B1Network PROPERTIES FOLDER "B1" )
endif()

set_target_properties(B1Network
	PROPERTIES
	OUTPUT_NAME B1Network
)

target_include_directories(B1Network
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

if(WIN32)
	target_compile_definitions(B1Network PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
