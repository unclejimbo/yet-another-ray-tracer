function(config_example target)
endfunction()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/common.h
)

add_executable(examples main.cpp)

target_compile_features(examples PRIVATE cxx_std_17)
target_compile_options(examples PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
        -pipe -march=native
        $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
    $<$<CXX_COMPILER_ID:MSVC>:
        /experimental:external "SHELL: /external:I ${CMAKE_SOURCE_DIR}/3rdparty" /external:W0
        $<$<CONFIG:Debug>:/Od /W4 /Zi>>
)

target_include_directories(examples PRIVATE
    ${CMAKE_SOURCE_DIR}/3rdparty/
    ${CMAKE_CURRENT_BINARY_DIR}/
)

target_link_libraries(examples PRIVATE
    yart::yart
)

set_target_properties(examples PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
