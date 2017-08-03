cmake_minimum_required(VERSION 3.6)
project(virt_machine)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    common-lib/common-data-types.h
    common-lib/common-utils.c
    common-lib/common-utils.h
    vm/main.c
    vm/vm.c
    vm/vm.h)

add_executable(virt_machine ${SOURCE_FILES})