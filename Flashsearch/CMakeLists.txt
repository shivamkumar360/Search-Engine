# 1. Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# 2. Project Name
project(FlashSearch VERSION 1.0)

# 3. Set C++ Standard to 17 (Required for std::filesystem)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Find the Threading library (pthreads)
find_package(Threads REQUIRED)


# 5. Tell CMake where to find your source code
add_executable(FlashSearch src/main.cpp src/SearchEngine.cpp)

# 6. Link the threading library to your app
target_link_libraries(FlashSearch PRIVATE 

Threads::Threads


)








# 7. Optimization: This makes your Search Engine run MUCH faster
if(NOT MSVC)
    target_compile_options(FlashSearch PRIVATE -O3)
endif()

# This automatically copies your existing index and data into the build folder
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/index.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/timestamp.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data DESTINATION ${CMAKE_CURRENT_BINARY_DIR})