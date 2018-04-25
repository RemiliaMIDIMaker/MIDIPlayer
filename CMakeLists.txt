cmake_minimum_required(VERSION 3.0)
project(MIDI-Player)

include_directories(Source)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -std=c++14")

add_subdirectory(ThirdLibrary/portmidi)

aux_source_directory(Source SOURCE_FILES)
file(GLOB_RECURSE INCLUDE_FILES "*.h")

include(ThirdLibrary/portmidi/lib_needed.cmake)
message("Require Library of portmidi : ${PM_NEEDED_LIBS}")

add_executable(MIDI-Player ${SOURCE_FILES})
target_link_libraries(MIDI-Player portmidi)
target_link_libraries(MIDI-Player ${PM_NEEDED_LIBS})
