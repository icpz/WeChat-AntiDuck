cmake_minimum_required(VERSION 3.10)
project(WeChat-AntiDuck)

option(FUNCHOOK_BUILD_SHARED "" OFF)
option(FUNCHOOK_BUILD_TESTS  "" OFF)
add_subdirectory(funchook)

set(SRCS src/antiduck.c)

set(DEPS
    funchook-static
    "-framework CoreAudio"
    )

file(COPY injection DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(antiduck SHARED ${SRCS})
target_link_libraries(antiduck PUBLIC ${DEPS})
set_property(TARGET antiduck PROPERTY LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/injection)

add_custom_target(inject
    COMMAND sudo make inject
    DEPENDS antiduck
    COMMENT "Injecting..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/injection
    )

add_custom_target(restore
    COMMAND sudo make restore
    DEPENDS antiduck
    COMMENT "Restoring..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/injection
    )

