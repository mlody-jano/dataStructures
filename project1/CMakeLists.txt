cmake_minimum_required(VERSION 3.25)
project(project1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/DoublyLinkedList.cpp
    src/DynamicTable.cpp
    src/Menu.cpp
    src/SinglyLinkedList.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)
