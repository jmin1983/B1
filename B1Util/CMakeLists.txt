# Sources
file(GLOB SRCS_G "*.cpp")
MACRO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB HDRS_G "*.h")
MACRO_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(B1Util ${SRCS})
if(MSVC)
	target_precompile_headers(B1Util
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/B1Util.h>
	)
endif()

if(MSVC)
	set_target_properties (B1Util PROPERTIES FOLDER "B1" )
endif()

set_target_properties(B1Util
	PROPERTIES
	OUTPUT_NAME B1Util
)

target_include_directories(B1Util
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

if(WIN32)
	target_compile_definitions(B1Util PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
