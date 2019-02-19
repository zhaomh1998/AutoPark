
# Put this CMakeLists.txt in the src folder (don't forget to rename it)

include_directories(../lib)
link_directories(../lib)
link_libraries(arduino_core)

add_executable(main.cpp)