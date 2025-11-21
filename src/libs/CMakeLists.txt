cmake_minimum_required(VERSION 3.16)

project(LibsWrapper LANGUAGES CXX)

set(SUBMODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/telegrambotlib-qt-fork")
set(CMAKE_AUTOMOC ON)

# --- 1. Sanity Checks ---
if(NOT EXISTS "${SUBMODULE_DIR}/src/telegrambot.cpp")
    message(FATAL_ERROR "Submodule source not found.")
endif()

add_library(telegrambotlib-qt-fork SHARED)

set_target_properties(telegrambotlib-qt-fork PROPERTIES 
    OUTPUT_NAME "telegrambotlib-qt"
)

# --- 2. Sources & Headers (The Fix) ---
target_sources(telegrambotlib-qt-fork PRIVATE
    # 1. Our Patched Source
    ${CMAKE_CURRENT_SOURCE_DIR}/telegrambot_patched.cpp 
    
    # 2. Original Sources
    ${SUBMODULE_DIR}/src/jsonhelper.cpp
    ${SUBMODULE_DIR}/modules/sslserver/sslserver.cpp
    ${SUBMODULE_DIR}/modules/httpserver/httpserver.cpp

    # 3. CRITICAL FIX: Explicitly list headers so AUTOMOC finds Q_OBJECT
    ${SUBMODULE_DIR}/src/telegrambot.h
    ${SUBMODULE_DIR}/modules/sslserver/sslserver.h
    ${SUBMODULE_DIR}/modules/httpserver/httpserver.h
)

# --- 3. Configure the API URL ---
target_compile_definitions(telegrambotlib-qt-fork PRIVATE
    TELEGRAM_API_BASE="http://localhost/bot%1/%2"
    TELEGRAM_API_PORT=8081
)

# --- 4. Headers & Includes ---
if(EXISTS "${SUBMODULE_DIR}/vendor/qdelegate/include/qdelegate.h")
    set(QDELEGATE_INC "${SUBMODULE_DIR}/vendor/qdelegate/include")
else()
    message(FATAL_ERROR "QDelegate header missing.")
endif()

target_include_directories(telegrambotlib-qt-fork PUBLIC
    ${SUBMODULE_DIR}/src
    ${SUBMODULE_DIR}/modules/sslserver
    ${SUBMODULE_DIR}/modules/httpserver
    ${QDELEGATE_INC}
)

find_package(Qt5 REQUIRED COMPONENTS Core Network)
target_link_libraries(telegrambotlib-qt-fork PUBLIC Qt5::Core Qt5::Network)
target_compile_features(telegrambotlib-qt-fork PUBLIC cxx_std_14)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(telegrambotlib-qt-fork PRIVATE -Wno-deprecated-declarations)
endif()