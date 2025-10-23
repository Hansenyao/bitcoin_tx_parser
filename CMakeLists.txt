cmake_minimum_required(VERSION 3.20)
project("bitcoin_tx_parser" VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE "Release")

# Set source file
add_executable(${CMAKE_PROJECT_NAME}
    ${CMAKE_SOURCE_DIR}/src/main/cpp
)