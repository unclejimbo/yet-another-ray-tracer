function(config_example target)
    target_compile_features(${target} PRIVATE cxx_std_17)
    target_compile_options(${target} PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>:
            -pipe -march=native
            $<$<CONFIG:Debug>:-O0 -Wall -Wextra>>
        $<$<CXX_COMPILER_ID:MSVC>:
            $<$<CONFIG:Debug>:/Od /Wall /Zi>>
    )

    target_include_directories(${target} PRIVATE
        ${CMAKE_SOURCE_DIR}/3rdparty/
        ${CMAKE_CURRENT_BINARY_DIR}/../
    )

    target_link_libraries(${target} PRIVATE
        yart::yart
    )

    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/common.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/common.h
)

add_subdirectory(rtiow)
add_subdirectory(rttnw)
