cmake_minimum_required(VERSION 3.10)
project(WeChat-AntiDuck)

add_library(fishhook-static-x64 STATIC fishhook/fishhook.c)
target_include_directories(fishhook-static-x64 PUBLIC fishhook/)
set_property(TARGET fishhook-static-x64 PROPERTY OSX_ARCHITECTURES x86_64)

add_library(fishhook-static-arm64 STATIC fishhook/fishhook.c)
target_include_directories(fishhook-static-arm64 PUBLIC fishhook/)
set_property(TARGET fishhook-static-arm64 PROPERTY OSX_ARCHITECTURES arm64)

add_custom_command(OUTPUT libfishhook-static.a
                   COMMAND lipo -create -output libfishhook-static.a
                            $<TARGET_LINKER_FILE:fishhook-static-x64>
                            $<TARGET_LINKER_FILE:fishhook-static-arm64>)

add_custom_target(fishhook-static-lipo DEPENDS libfishhook-static.a)

add_library(fishhook-static STATIC IMPORTED)
add_dependencies(fishhook-static fishhook-static-lipo)
set_property(TARGET fishhook-static PROPERTY IMPORTED_LOCATION libfishhook-static.a)
target_include_directories(fishhook-static INTERFACE fishhook/)

set(SRCS src/antiduck.c)

set(DEPS
    "-framework CoreAudio"
    fishhook-static)

add_library(antiduck-x64 SHARED ${SRCS})
target_link_libraries(antiduck-x64 PUBLIC ${DEPS})
set_property(TARGET antiduck-x64 PROPERTY OSX_ARCHITECTURES x86_64)

add_library(antiduck-arm64 SHARED ${SRCS})
target_link_libraries(antiduck-arm64 PUBLIC ${DEPS})
set_property(TARGET antiduck-arm64 PROPERTY OSX_ARCHITECTURES arm64)

add_custom_command(OUTPUT injection/libantiduck.dylib
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/injection
                   COMMAND lipo -create -output injection/libantiduck.dylib
                            $<TARGET_LINKER_FILE:antiduck-x64>
                            $<TARGET_LINKER_FILE:antiduck-arm64>)

add_custom_target(antiduck ALL DEPENDS injection/libantiduck.dylib)

add_custom_command(TARGET antiduck PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                                ${CMAKE_CURRENT_SOURCE_DIR}/injection
                                ${CMAKE_CURRENT_BINARY_DIR}/injection)

add_custom_target(inject
    COMMAND sudo make inject
    DEPENDS antiduck
    COMMENT "Injecting..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/injection)

add_custom_target(restore
    COMMAND sudo make restore
    DEPENDS antiduck
    COMMENT "Restoring..."
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/injection)

