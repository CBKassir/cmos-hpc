cmake_minimum_required(VERSION 3.15)
project(cmos_hpc)

set(CMAKE_CXX_STANDARD 17)
find_package(Eigen3 REQUIRED)

add_executable(cmos_hpc main.cpp)

target_include_directories(cmos_hpc PRIVATE
    core
    devices
    analysis
)
target_link_libraries(cmos_hpc PRIVATE Eigen3::Eigen)