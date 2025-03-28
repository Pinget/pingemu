cmake_minimum_required(VERSION 3.10.0)
project(pingemu VERSION 0.1.0 LANGUAGES C CXX)

add_executable(pingemu APU.cpp CPU.cpp NES.cpp CPU_ops.cpp)

include(CTest)
enable_testing()

set_target_properties(pingemu
PROPERTIES
RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}
)

