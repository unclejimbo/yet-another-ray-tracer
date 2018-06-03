add_executable(yart-example
    "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp"
)

target_compile_features(yart-example PUBLIC cxx_std_17)
target_compile_options(yart-example PRIVATE
    -pipe -march=native
    $<$<CONFIG:Debug>:-O0 -Wall -Wextra>
)

target_include_directories(yart-example PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/
)

target_link_libraries(yart-example PRIVATE
    yart::yart
)

set_target_properties(yart-example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
