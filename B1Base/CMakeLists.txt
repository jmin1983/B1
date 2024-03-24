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

add_library(B1Base ${SRCS})
if(MSVC)
	target_precompile_headers(B1Base
	PRIVATE
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/B1Base.h>
	)
endif()

if(MSVC)
	set_target_properties (B1Base PROPERTIES FOLDER "B1" )
endif()

set_target_properties(B1Base
	PROPERTIES
	OUTPUT_NAME B1Base
)

target_include_directories(B1Base
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
		$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../external/include>
)

if(WIN32)
	target_compile_definitions(B1Base PUBLIC _CRT_SECURE_NO_WARNINGS)
	target_link_libraries(B1Base PUBLIC iphlpapi)
else()
	target_link_libraries(B1Base PUBLIC pthread dl rt)
endif()

if(ENABLE_TESTS)
	set_property(TARGET B1Base PROPERTY POSITION_INDEPENDENT_CODE ON) # This is needed to build TestLibrary.so as shared.
	add_subdirectory(tests)
endif()