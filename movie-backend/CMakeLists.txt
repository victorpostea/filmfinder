cmake_minimum_required(VERSION 3.10)
project(MovieBackend)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)

add_executable(movie-backend src/main.cpp)

# Include Crow, ASIO, and CPR
target_include_directories(movie-backend PRIVATE ${CMAKE_SOURCE_DIR}/external/crow/include)
target_include_directories(movie-backend PRIVATE ${CMAKE_SOURCE_DIR}/external/asio/asio/include)
target_include_directories(movie-backend PRIVATE ${CMAKE_SOURCE_DIR}/external/cpr/include)
target_include_directories(movie-backend PRIVATE ${CMAKE_SOURCE_DIR}/external/nlohmann)

# Add CPR as a subdirectory
add_subdirectory(${CMAKE_SOURCE_DIR}/external/cpr)

# Link libraries
target_link_libraries(movie-backend PRIVATE cpr::cpr pthread)