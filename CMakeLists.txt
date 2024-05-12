cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# Add the main executable target
add_executable(main 
    main.cpp 
    tokenizer/tokenizer.cpp 
    utils/utils.cpp
    tokenizer/parser/parser.cpp
    interpreter/visitor.cpp
)

# Include directories for libraries
target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/fs
    ${CMAKE_CURRENT_SOURCE_DIR}/tokenizer
    ${CMAKE_CURRENT_SOURCE_DIR}/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/interpreter
)

# Set C++ standard to C++17
target_compile_features(main PRIVATE cxx_std_17)

# Enable compiler optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(main PRIVATE "-O2")  # Optimization level 2
    target_compile_options(main PRIVATE "-O3")  # Optimization level 3
elseif(MSVC)
    target_compile_options(main PRIVATE "/O2") # Optimization level 2
    target_compile_options(main PRIVATE "/Ox") # Full optimization
endif()
