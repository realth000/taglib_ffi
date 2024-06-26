# The Flutter tooling requires that developers have CMake 3.10 or later
# installed. You should not increase this version, as doing so will cause
# the plugin to fail to compile for some customers of the plugin.
cmake_minimum_required(VERSION 3.13)

project(taglib_ffi_library VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -Wall -extra -g -O2")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -extra -g -O2")

# add_definitions(-DTAGLIB_STATIC)

if (MSVC)
  add_compile_options("/MP")
endif()

include_directories(taglib/taglib
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/toolkit
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/asf
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpeg
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg/flac
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/flac
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpc
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mp4
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg/vorbis
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg/speex
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ogg/opus
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpeg/id3v2
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpeg/id3v2/frames
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mpeg/id3v1
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/ape
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/wavpack
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/trueaudio
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/riff
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/riff/aiff
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/riff/wav
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/mod
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/s3m
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/it
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/taglib/xm
        ${CMAKE_CURRENT_SOURCE_DIR}/taglib/3rdparty
        ${CMAKE_BINARY_DIR}/taglib
        ${CMAKE_BINARY_DIR}/plugins/taglib_ffi/shared/taglib # For building as a flutter plugin
)

add_library(tag_ffi SHARED
        taglib_ffi.h
        taglib_ffi.cpp
        src/ReaderOption.hpp
        src/Reader.hpp
        src/Reader.cpp
        src/ID3v2.hpp
        src/ID3v2.cpp
        src/Utils.hpp
        src/Utils.cpp
)

add_subdirectory(taglib)
add_subdirectory(example)

set_target_properties(tag_ffi PROPERTIES
        PUBLIC_HEADER taglib_ffi.h
        OUTPUT_NAME "tag_ffi"
)

target_compile_definitions(tag PUBLIC DART_SHARED_LIB)
target_compile_definitions(tag_ffi PUBLIC DART_SHARED_LIB)
target_link_libraries(tag_ffi tag)
