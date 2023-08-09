cmake_minimum_required (VERSION 3.8)

project (posTest)

include_directories(../include .)

set ( SRC_LIST
    ./test.cpp
    ../src/dataReader.cpp
    ../src/antenna.cpp 
    ../src/dataAllocator.cpp 
    ../src/tag.cpp
    ../src/tagChecker.cpp
 )

add_executable(test ${SRC_LIST})

set(CMAKE_CXX_COMPILER g++)
#ADD_COMPILE_OPTIONS(-std=c++11 -g)
set(CMAKE_CXX_FLAGS "${CAMKE_CXX_FLAGS} -std=c++11 -pthread")