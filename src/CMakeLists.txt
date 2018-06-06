option(BUILD_SHARED_LIBS "Build shared library" ON)

find_package(Eigen3 REQUIRED)
find_package(Embree 3.0 REQUIRED)
find_package(OpenMP)

add_library(yart
    "${CMAKE_CURRENT_SOURCE_DIR}/core/camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/device.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/geometry.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/scene.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/camera/perspective.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/geometry/sphere.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/material/lambertian.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/material/metal.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/material/dielectric.cpp"
)
add_library(yart::yart ALIAS yart)

target_compile_features(yart PUBLIC cxx_std_17)
target_compile_options(yart PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -pipe -march=native
        $<$<CONFIG:Debug>:-O0 -Wall -Wextra -fno-inline>>
    $<$<CXX_COMPILER_ID:MSVC>:
        $<$<CONFIG:Debug>:/Od /Wall /Zi>>
)

target_include_directories(yart
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        ${EMBREE_INCLUDE_DIRS}
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(yart PUBLIC
    Eigen3::Eigen
    ${EMBREE_LIBRARIES}
)

if(OPENMP_FOUND)
    target_link_libraries(yart PRIVATE OpenMP::OpenMP_CXX)
endif()

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
)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include
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

configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/yart-config.cmake.in
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
