cmake_minimum_required(VERSION 3.15)
project(CommandSystem LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
    add_compile_options(-Wall -Wextra -pedantic)
    
    if(WIN32)
        add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
    endif()
elseif(MSVC)
    add_compile_options(/W4 /utf-8)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(HEADERS
    iwrapper.h
    engine.h
    wrapper.h
    wrapper.tpp
    tests.h
)

set(SOURCES
    main.cpp
    tests.cpp
    engine.cpp
)

add_executable(main_program ${SOURCES} ${HEADERS})

if(WIN32)
    target_compile_definitions(main_program PRIVATE UNICODE _UNICODE)
endif()

message(STATUS "===========================================")
message(STATUS "Проект: ${PROJECT_NAME}")
message(STATUS "Система: ${CMAKE_SYSTEM_NAME}")
message(STATUS "Компилятор: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Стандарт C++: ${CMAKE_CXX_STANDARD}")
message(STATUS "===========================================")
