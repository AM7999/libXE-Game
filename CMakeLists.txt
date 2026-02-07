cmake_minimum_required(VERSION 3.16)

if(PLATFORM STREQUAL "VITA" AND DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
endif()

project(game)

set(CMAKE_CXX_STANDARD 20)

set(GAME SDL3)
set(BUILD_TESTS false)
set(BUILD_EXAMPLES OFF)

# set(PLATFORM PC)

set(CMAKE_CXX_STANDARD 20)

if(PLATFORM STREQUAL "PC")
    add_subdirectory("extern/SDL" SDL3)
    add_subdirectory("extern/SDL_image" SDL3_Image)
    add_subdirectory("extern/libzip" libzip)
    add_subdirectory("extern/iniparser" iniparser)
    #add_subdirectory("extern/dylib" dylib)
endif()

if(GAME STREQUAL "SDL3")
    if(PLATFORM STREQUAL "PC")
        file(GLOB_RECURSE GAME_SRC
            "${CMAKE_SOURCE_DIR}/src/Game/*.cpp"
            "${CMAKE_SOURCE_DIR}/src/Game/*.hpp"
            "${CMAKE_SOURCE_DIR}/src/include/*.c"
            "${CMAKE_SOURCE_DIR}/src/include/*.hpp"
            "${CMAKE_SOURCE_DIR}/src/include/*.h"
        )

        file(GLOB_RECURSE ENGINE_SRC
            "${CMAKE_SOURCE_DIR}/src/Engine/*.cpp"
            "${CMAKE_SOURCE_DIR}/src/Engine/*.hpp"
        )

        # --- BEGIN IMGUI INCLUDE --- #
        include(FetchContent)

        FetchContent_Declare(
                imgui
                GIT_REPOSITORY https://github.com/ocornut/imgui
                GIT_TAG        v1.92.5
        )
        FetchContent_MakeAvailable(imgui)

        add_library(imgui STATIC
                ${imgui_SOURCE_DIR}/imgui.cpp
                ${imgui_SOURCE_DIR}/imgui_draw.cpp
                ${imgui_SOURCE_DIR}/imgui_widgets.cpp
                ${imgui_SOURCE_DIR}/imgui_tables.cpp
                ${imgui_SOURCE_DIR}/imgui_demo.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
                ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
        )

        target_include_directories(imgui PUBLIC
                ${imgui_SOURCE_DIR}
                ${imgui_SOURCE_DIR}/backends
        )
        # --- END OF IMGUI INCLUDE --- #

        add_library(XE SHARED ${ENGINE_SRC})
        target_include_directories(XE PRIVATE ${CMAKE_SOURCE_DIR}/src/include)
        target_link_libraries(XE PRIVATE SDL3::SDL3 SDL3_image::SDL3_image zip)

        add_executable(${PROJECT_NAME} ${GAME_SRC})
        target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_image::SDL3_image zip iniparser-shared XE imgui)
        
        add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/resources.zip
            COMMAND zip -r "${CMAKE_BINARY_DIR}/resources.zip" res
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
            DEPENDS ${CMAKE_SOURCE_DIR}/res
            COMMENT "Compressing res folder into resources.zip"
            VERBATIM
        )

        # Add custom target to ensure the zip is created
        add_custom_target(compress_resources ALL
            DEPENDS ${CMAKE_BINARY_DIR}/resources.zip
        )

        # Make the executable depend on the compressed resources
        add_dependencies(${PROJECT_NAME} compress_resources)
    endif()
    if(PLATFORM STREQUAL "VITA")
        include("${VITASDK}/share/vita.cmake" REQUIRED)

        set(VITA_APP_NAME "libXE Game")
        set(VITA_TITLEID "XEG000001")
        set(VITA_VERSION "01.00")

        find_package(PkgConfig REQUIRED)
        find_package(SDL3 CONFIG REQUIRED)
        find_package(SDL3_image CONFIG REQUIRED)
        find_package(libzip CONFIG REQUIRED)

        file(GLOB_RECURSE GAME_SRC
            "${CMAKE_SOURCE_DIR}/src/Game/*.cpp"
            "${CMAKE_SOURCE_DIR}/src/Game/*.hpp"
        )

        file(GLOB_RECURSE ENGINE_SRC
            "${CMAKE_SOURCE_DIR}/src/Engine/*.cpp"
            "${CMAKE_SOURCE_DIR}/src/Engine/*.hpp"
        )

        add_library(XE STATIC ${ENGINE_SRC})
        target_include_directories(XE PRIVATE
            ${SDL3_INCLUDE_DIRS}
            ${SDL3_IMAGE_INCLUDE_DIRS}
        )
        target_link_libraries(XE
            ${SDL3_LIBRARIES}
            ${SDL3_IMAGE_LIBRARIES}
        )

        add_executable(${PROJECT_NAME} ${GAME_SRC})
        target_include_directories(${PROJECT_NAME} PRIVATE
            ${SDL3_INCLUDE_DIRS}
            ${SDL3_IMAGE_INCLUDE_DIRS}
        )

        target_link_libraries(${PROJECT_NAME}
            XE
            SDL3::SDL3
            SDL3_image::SDL3_image
            zip

            pthread
            bz2
            lzma
            zstd
            crypto
            ssl

            SceDisplay_stub
            SceGxm_stub
            SceCtrl_stub
            SceAudio_stub
            SceCommonDialog_stub
            ScePower_stub
            m
            c
        )

        target_compile_options(${PROJECT_NAME} PRIVATE -pthread -g -O2 -DVITA)
        target_link_options(${PROJECT_NAME} PRIVATE -pthread)

        vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})

        add_custom_command(
            OUTPUT ${CMAKE_BINARY_DIR}/resources.zip
            COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/resources.zip" --format=zip "${CMAKE_SOURCE_DIR}/res"
            DEPENDS ${CMAKE_SOURCE_DIR}/res
            COMMENT "Compressing res folder into resources.zip"
            VERBATIM
        )

        # Add custom target to ensure the zip is created
        add_custom_target(compress_resources_vita ALL
            DEPENDS ${CMAKE_BINARY_DIR}/resources.zip
        )

        # Make the self file depend on the compressed resources
        add_dependencies(${PROJECT_NAME}.self compress_resources_vita)

        vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
            VERSION ${VITA_VERSION}
            NAME ${VITA_APP_NAME}
            FILE data/sce_sys/icon0.png sce_sys/icon0.png
            FILE data/sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
            #FILE data/sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
            FILE data/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
            FILE ${CMAKE_BINARY_DIR}/resources.zip resources.zip
        )
    endif()
endif()


if(GAME STREQUAL "SDL2")
    file(GLOB_RECURSE GAME_SRC
        "${CMAKE_SOURCE_DIR}/legacy/*.cpp"
        "${CMAKE_SOURCE_DIR}/legacy/*.hpp"
    )

    add_executable(${PROJECT_NAME} ${GAME_SRC})
    target_link_libraries(${PROJECT_NAME} PRIVATE SDL0::SDL2 SDL2_image::SDL2_image zip iniparser-shared dylib)
endif()

if(BUILD_TESTS EQUAL true)
    add_library(testlib SHARED tests/TestLib.cpp)
    add_executable(dylib_test tests/Dylib-Main.cpp)

    add_executable(SoundTest tests/Sound-Main.cpp)
    target_link_libraries(SoundTest SDL3::SDL3)

    target_link_libraries(dylib_test PRIVATE dylib)
endif()