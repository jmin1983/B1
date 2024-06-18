# Sources
file(GLOB SRCS_G "*.cpp")
MACRO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB HDRS_G "*.h")
MACRO_HEADERS_AUTO(SRCS ${HDRS_G})

if(MSVC)
	add_compile_options("$<$<CONFIG:DEBUG>:/bigobj>")
endif()

add_library(B1Http ${SRCS})
if(MSVC)
	target_precompile_headers(B1Http
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/B1Http.h>
	)
endif()

if(MSVC)
	set_target_properties (B1Http PROPERTIES FOLDER "B1" )
endif()

set_target_properties(B1Http
	PROPERTIES
	OUTPUT_NAME B1Http
)

target_include_directories(B1Http
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/include>
)

if(WIN32)
	target_compile_definitions(B1Http PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")
endif()
