cmake_minimum_required(VERSION 3.10)

project(libyart LANGUAGES CXX)

set(YART_VERSION_MAJOR 0)
set(YART_VERSION_MINOR 1)
set(YART_VERSION_PATCH 1)
set(YART_VERSION ${YART_VERSION_MAJOR}.${YART_VERSION_MINOR}.${YART_VERSION_PATCH})

add_subdirectory(src)
add_subdirectory(examples)
