include_directories(../)
add_executable(01_exe 01.c)
target_link_libraries(01_exe rsdb)
add_test(01 01_exe)