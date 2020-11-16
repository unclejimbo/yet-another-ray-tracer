find_package(Eigen3 REQUIRED)
find_package(Embree 3.0 REQUIRED)
find_package(OpenMP)

add_library(yart
    ${PROJECT_SOURCE_DIR}/include/yart/camera/perspective.h
    ${PROJECT_SOURCE_DIR}/include/yart/core/camera.h
    ${PROJECT_SOURCE_DIR}/include/yart/core/device.h
    ${PROJECT_SOURCE_DIR}/include/yart/core/geometry.h
    ${PROJECT_SOURCE_DIR}/include/yart/core/material.h
    ${PROJECT_SOURCE_DIR}/include/yart/core/scene.h
    ${PROJECT_SOURCE_DIR}/include/yart/core/texture.h
    ${PROJECT_SOURCE_DIR}/include/yart/geometry/plane.h
    ${PROJECT_SOURCE_DIR}/include/yart/geometry/sphere.h
    ${PROJECT_SOURCE_DIR}/include/yart/material/dielectric.h
    ${PROJECT_SOURCE_DIR}/include/yart/material/diffuse_light.h
    ${PROJECT_SOURCE_DIR}/include/yart/material/lambertian.h
    ${PROJECT_SOURCE_DIR}/include/yart/material/metal.h
    ${PROJECT_SOURCE_DIR}/include/yart/texture/checkerboard.h
    ${PROJECT_SOURCE_DIR}/include/yart/texture/constant.h
    ${PROJECT_SOURCE_DIR}/include/yart/texture/image.h

    ${PROJECT_SOURCE_DIR}/src/camera/perspective.cpp
    ${PROJECT_SOURCE_DIR}/src/core/camera.cpp
    ${PROJECT_SOURCE_DIR}/src/core/device.cpp
    ${PROJECT_SOURCE_DIR}/src/core/geometry.cpp
    ${PROJECT_SOURCE_DIR}/src/core/material.cpp
    ${PROJECT_SOURCE_DIR}/src/core/scene.cpp
    ${PROJECT_SOURCE_DIR}/src/geometry/plane.cpp
    ${PROJECT_SOURCE_DIR}/src/geometry/sphere.cpp
    ${PROJECT_SOURCE_DIR}/src/material/dielectric.cpp
    ${PROJECT_SOURCE_DIR}/src/material/diffuse_light.cpp
    ${PROJECT_SOURCE_DIR}/src/material/lambertian.cpp
    ${PROJECT_SOURCE_DIR}/src/material/metal.cpp
    ${PROJECT_SOURCE_DIR}/src/texture/checkerboard.cpp
    ${PROJECT_SOURCE_DIR}/src/texture/constant.cpp
    ${PROJECT_SOURCE_DIR}/src/texture/image.cpp
    ${PROJECT_SOURCE_DIR}/src/util/macro.h
    ${PROJECT_SOURCE_DIR}/src/util/random.h
    ${PROJECT_SOURCE_DIR}/src/util/rayhit.h
)
add_library(yart::yart ALIAS yart)

option(BUILD_SHARED_LIBS "Build shared library" ON)
include(GenerateExportHeader)
generate_export_header(yart
    EXPORT_MACRO_NAME YART_API
    EXPORT_FILE_NAME ${CMAKE_BINARY_DIR}/include/yart/core/common.h
)

target_compile_features(yart PUBLIC cxx_std_17)
target_compile_options(yart PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -pipe -march=native
        $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /Wall /Zi>>
)

target_include_directories(yart
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
        $<BUILD_INTERFACE:${EMBREE_INCLUDE_DIRS}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(yart
    PUBLIC
        Eigen3::Eigen
        ${EMBREE_LIBRARIES}
    PRIVATE
        $<$<BOOL:${OPENMP_FOUND}>:OpenMP::OpenMP_CXX>
)

set_target_properties(yart PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(GNUInstallDirs)
set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/yart)

install(TARGETS yart
    EXPORT yart-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${EMBREE_INCLUDE_DIRS}
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/yart
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
install(DIRECTORY ${CMAKE_BINARY_DIR}/include/yart
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT yart-targets
    FILE yart-targets.cmake
    NAMESPACE yart::
    DESTINATION ${INSTALL_CONFIGDIR}
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/cmake/yart-config-version.cmake
    VERSION ${YART_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/yart-config.cmake.in
    ${CMAKE_BINARY_DIR}/cmake/yart-config.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
)

install(
    FILES
        ${CMAKE_BINARY_DIR}/cmake/yart-config.cmake
        ${CMAKE_BINARY_DIR}/cmake/yart-config-version.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
)

export(EXPORT yart-targets
    FILE ${CMAKE_BINARY_DIR}/cmake/yart-targets.cmake
    NAMESPACE yart::
)

export(PACKAGE yart)
