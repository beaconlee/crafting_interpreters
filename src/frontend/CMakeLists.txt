# src/scanner/CMakeLists.txt

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_BUILD_TYPE Debug)


add_library(frontend SHARED
   src/scanner.cc
)

target_include_directories(frontend PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(frontend PUBLIC
    
)
add_executable(test tests/scanner_test.cc)

target_include_directories(test PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)