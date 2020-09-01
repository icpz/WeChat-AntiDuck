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

add_library(antiduck SHARED ${SRCS})
target_link_libraries(antiduck PUBLIC ${DEPS})

