cmake_minimum_required (VERSION 3.1)

project (dspinc)
add_executable(dspinc main.c src/test.c src/wavFile.c)
target_link_libraries(dspinc Ws2_32.lib)