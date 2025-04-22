cmake_minimum_required(VERSION 3.23)
project(rlImGui VERSION 0.1.0)

option(BUILD_EXAMPLES "Build examples" OFF)

include(FetchContent)

find_package(imgui QUIET)
if (NOT imgui_FOUND)
    FetchContent_Declare(
        imgui
        URL      https://github.com/ocornut/imgui/archive/refs/heads/master.zip
        URL_HASH MD5=cc306a442c28bc853615eb9579292130
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(imgui)
    add_library(imgui STATIC)
    add_library(imgui::imgui ALIAS imgui)
    target_sources(imgui
        PUBLIC
            FILE_SET HEADERS
                FILES
                ${imgui_SOURCE_DIR}/imgui.h
                ${imgui_SOURCE_DIR}/imgui_internal.h
                ${imgui_SOURCE_DIR}/imstb_rectpack.h
                ${imgui_SOURCE_DIR}/imstb_textedit.h
                ${imgui_SOURCE_DIR}/imstb_truetype.h
                BASE_DIRS ${imgui_SOURCE_DIR}
        PRIVATE
            ${imgui_SOURCE_DIR}/imgui.cpp
            ${imgui_SOURCE_DIR}/imgui_demo.cpp
            ${imgui_SOURCE_DIR}/imgui_draw.cpp
            ${imgui_SOURCE_DIR}/imgui_tables.cpp
            ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    )
endif ()

find_package(raylib QUIET)
if (NOT raylib_FOUND)
    FetchContent_Declare(
        raylib
        URL      https://github.com/raysan5/raylib/archive/refs/heads/master.zip
        URL_HASH MD5=cbb82e5c9beca98fd27787821e7c0cf2
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif ()


add_library(rlImGui STATIC)
target_sources(rlImGui
    PUBLIC
        FILE_SET HEADERS
        FILES rlImGui.h rlImGuiColors.h imgui_impl_raylib.h
    PRIVATE
        rlImGui.cpp
)
target_link_libraries(rlImGui PUBLIC imgui::imgui raylib)

set_target_properties(rlImGui PROPERTIES EXPORT_NAME "rlImGui")
install(TARGETS rlImGui
        EXPORT rlImGuiTargets
        ARCHIVE DESTINATION lib
        FILE_SET HEADERS DESTINATION include
)

install(EXPORT rlImGuiTargets
        NAMESPACE rlImGui::
        DESTINATION lib/cmake/rlImGui
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/rlImGuiConfigVersion.cmake"
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/rlImGuiConfigVersion.cmake"
    DESTINATION lib/cmake/rlImGui
)

configure_file(cmake/rlImGuiConfig.cmake.in rlImGuiConfig.cmake @ONLY)
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/rlImGuiConfig.cmake"
    DESTINATION lib/cmake/rlImGui
)


if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()

