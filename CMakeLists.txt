cmake_minimum_required(VERSION 3.10)

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)

project(libyart LANGUAGES CXX)

set(YART_MAJOR_VERSION 0)
set(YART_MINOR_VERSION 1)
set(YART_PATCH_VERSION 0)
set(YART_VERSION ${YART_MAJOR_VERSION}.${YART_MINOR_VERSION}.${YART_PATCH_VERSION})

add_subdirectory(src)
add_subdirectory(examples)
