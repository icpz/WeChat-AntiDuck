cmake_minimum_required(VERSION 3.10)
project(WeChat-AntiDuck)

add_library(fishhook-static-x64 STATIC fishhook/fishhook.c)
target_include_directories(fishhook-static-x64 PUBLIC fishhook/)
set_property(TARGET fishhook-static-x64 PROPERTY OSX_ARCHITECTURES x86_64)

add_library(fishhook-static-arm64 STATIC fishhook/fishhook.c)
target_include_directories(fishhook-static-arm64 PUBLIC fishhook/)
set_property(TARGET fishhook-static-arm64 PROPERTY OSX_ARCHITECTURES arm64)

add_custom_command(
    OUTPUT libfishhook-static.a
    COMMAND lipo -create -output libfishhook-static.a libfishhook-static-arm64.a libfishhook-static-x64.a
    DEPENDS fishhook-static-x64 fishhook-static-arm64)

add_custom_target(fishhook-static-gen DEPENDS libfishhook-static.a)

add_library(fishhook-static STATIC IMPORTED)
add_dependencies(fishhook-static fishhook-static-gen)
set_property(TARGET fishhook-static PROPERTY IMPORTED_LOCATION libfishhook-static.a)
target_include_directories(fishhook-static INTERFACE fishhook/)

set(SRCS
    src/antiduck.c
    )

set(DEPS
    "-framework CoreAudio"
    fishhook-static
    )

file(COPY injection DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_library(antiduck-x64 SHARED ${SRCS})
target_link_libraries(antiduck-x64 PUBLIC ${DEPS})
set_property(TARGET antiduck-x64 PROPERTY OSX_ARCHITECTURES x86_64)

add_library(antiduck-arm64 SHARED ${SRCS})
target_link_libraries(antiduck-arm64 PUBLIC ${DEPS})
set_property(TARGET antiduck-arm64 PROPERTY OSX_ARCHITECTURES arm64)

add_custom_command(
    OUTPUT injection/libantiduck.dylib
    COMMAND lipo -create -output injection/libantiduck.dylib libantiduck-x64.dylib libantiduck-arm64.dylib
    DEPENDS antiduck-x64 antiduck-arm64)

add_custom_target(antiduck ALL DEPENDS injection/libantiduck.dylib)

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

