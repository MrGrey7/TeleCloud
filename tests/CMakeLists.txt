cmake_minimum_required(VERSION 3.16)

project(TeleCloudTests LANGUAGES CXX)

# --- FIX: Enable AUTOMOC ---
# This tells CMake: "Scan my cpp files. If you see #include 'foo.moc', generate it."
set(CMAKE_AUTOMOC ON)

# Find Qt Test Module
find_package(Qt5 REQUIRED COMPONENTS Core Test Sql)

# Define the Test Executable
add_executable(tst_dbmanager
    tst_dbmanager.cpp

    # We include the source file we want to test directly
    ../src/telegrambot-gui/dbmanager.cpp
)

# Fix includes so dbmanager.cpp can find "dbmanager.h" and "types.h"
target_include_directories(tst_dbmanager PRIVATE
    ../src/telegrambot-gui
    ../include
)

# Link Qt Modules
target_link_libraries(tst_dbmanager PRIVATE
    Qt5::Core
    Qt5::Test
    Qt5::Sql
)

# Register the test with CTest
add_test(NAME DbManagerTest COMMAND tst_dbmanager)
