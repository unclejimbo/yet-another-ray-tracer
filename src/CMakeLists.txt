find_package(Eigen3 REQUIRED)
find_package(Embree 3.0 REQUIRED)
find_package(OpenMP)

add_library(yart
    "${CMAKE_CURRENT_SOURCE_DIR}/camera/perspective.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/device.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/geometry.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/scene.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/sphere.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/material/dielectric.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/material/diffuse_light.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/material/lambertian.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/material/metal.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/texture/checkerboard.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/texture/constant.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/texture/image.cpp"
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
