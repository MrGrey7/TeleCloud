cmake_minimum_required(VERSION 3.16)

project(TeleCloudSolution VERSION 1.0 LANGUAGES CXX)

# --- 1. Global Configuration ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler Flags
add_compile_options(-Wno-unknown-pragmas)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wno-inconsistent-missing-override)
endif()

if(UNIX)
    add_compile_definitions(HAVE_CLOCK_SETTIME)
endif()

# --- 2. Output Layout ---
set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_DEBUG_POSTFIX "d")

# --- 3. Global Includes ---
include_directories("${CMAKE_SOURCE_DIR}/include")

# --- 4. Dependencies ---
# FIX: Added 'Xml' to the list below
find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets Network Sql Xml)

# --- 5. Sub-Projects ---
add_subdirectory(config)
add_subdirectory(src)

# --- 6. Deploy Database ---
if(EXISTS "${CMAKE_SOURCE_DIR}/db/telecloud.db")
    file(COPY "${CMAKE_SOURCE_DIR}/db/telecloud.db" 
         DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

# Optional: Tests
option(BUILD_TESTS "Build tests" OFF)
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()