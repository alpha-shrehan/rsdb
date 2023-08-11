cmake_minimum_required(VERSION 3.0.0)
project(rsdb VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

add_subdirectory(tests)

add_library(rsdb
    header.h
    mem.h mem.c
    textblob.h textblob.c
    vmgr.h vmgr.c
    rsdb.h rsdb.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
