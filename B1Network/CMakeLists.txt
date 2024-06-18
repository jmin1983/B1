# Sources
file(GLOB SRCS_G "*.cpp")
MACRO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB HDRS_G "*.h")
MACRO_HEADERS_AUTO(SRCS ${HDRS_G})

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
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/include>
)

if(WIN32)
	target_compile_definitions(B1Network PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
