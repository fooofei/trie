
# cmake -DCMAKE_BUILD_TYPE=Debug -G "Visual Studio 16 2019" -A Win32 ..

cmake_minimum_required(VERSION 3.1)
project(patricia)

if(APPLE)
    set(CMAKE_MACOSX_RPATH 0)
endif()

set(BINARY_NAME trie)

set(Source_files 
    ${CMAKE_CURRENT_LIST_DIR}/patricia/tree.c
    ${CMAKE_CURRENT_LIST_DIR}/patricia/tree.h
    ${CMAKE_CURRENT_LIST_DIR}/patricia/prefix.h
    ${CMAKE_CURRENT_LIST_DIR}/patricia/prefix.c
    ${CMAKE_CURRENT_LIST_DIR}/patricia/iterator.c
    ${CMAKE_CURRENT_LIST_DIR}/patricia/iterator.h
    ${CMAKE_CURRENT_LIST_DIR}/test.cc
    ${CMAKE_CURRENT_LIST_DIR}/crt_dbg_leak.h
    ${CMAKE_CURRENT_LIST_DIR}/crt_dbg_leak.c
	)

add_executable(${BINARY_NAME} ${Source_files})

source_group(files FILES ${Source_files})

if(WIN32)
	target_compile_options(${BINARY_NAME} PRIVATE /source-charset:utf-8 /execution-charset:utf-8)
	target_compile_definitions(${BINARY_NAME} PRIVATE "WIN32")
	if(MSVC)
        set_property(TARGET ${BINARY_NAME}  APPEND PROPERTY LINK_FLAGS /DEBUG)
    endif()
endif()
