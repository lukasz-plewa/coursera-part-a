
project(poker_probability)

cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 11)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

#
# Extra compiler / linker options
#
IF(CMAKE_COMPILER_IS_GNUCC)
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wextra -Wformat=2")
ENDIF(CMAKE_COMPILER_IS_GNUCC)
IF(CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra -Wformat=2")
ENDIF(CMAKE_COMPILER_IS_GNUCXX)

file(GLOB poker_SRC
    "src/*.h"
    "src/*.cpp"
)
add_executable(${PROJECT_NAME} ${poker_SRC})