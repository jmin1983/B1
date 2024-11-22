# Sources
file(GLOB SRCS_G "*.cpp")
MACRO_SOURCES_AUTO(SRCS ${SRCS_G})

# Headers
file(GLOB HDRS_G "*.h")
MACRO_HEADERS_AUTO(SRCS ${HDRS_G})

add_library(B1SMTP ${SRCS})
if(MSVC)
	target_precompile_headers(B1SMTP
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/B1SMTP.h>
	)
endif()

if(MSVC)
	set_target_properties (B1SMTP PROPERTIES FOLDER "B1" )
endif()

set_target_properties(B1SMTP
	PROPERTIES
	OUTPUT_NAME B1SMTP
)

target_include_directories(B1SMTP
	PRIVATE
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

if(WIN32)
	target_compile_definitions(B1SMTP PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()
