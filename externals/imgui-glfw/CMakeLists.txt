# Only tested in 3.1, may work in older versions.
# .
# Install GLFW or set GLFW_ROOT and set GLFW_ROOT prior to running cmake
# .
# Provides the follow variables:
# IMGUI_GLFW_INCLUDE_DIRS - imgui and imgui_glfw include paths
# IMGUI_SOURCES - imgui.cpp source path to link with your binary
# IMGUI_GLFW_SOURCES - imgui_glfw.cpp source path to link with your binary
# IMGUI_GLFW_DEPENDENCIES - found dependencies to link with your library (glfw)
# .
# Sample usage:
#   add_subdirectory(repos/imgui-glfw)
#   include_directories("${IMGUI_GLFW_INCLUDE_DIRS}")
#   add_executable(MY_PROJECT ${IMGUI_SOURCES} ${IMGUI_GLFW_SOURCES} ${SRCS})
#   ...
#   target_link_libraries(MY_PROJECT ${IMGUI_GLFW_DEPENDENCIES})
# .
# Define spine-glfw library
if(IMGUI_ROOT)
    set(IMGUI_SERACH_PATH
        ${IMGUI_ROOT}
        $ENV{IMGUI_ROOT}
        )
    find_path(IMGUI_INCLUDE_DIR imgui.cpp
              PATHS ${IMGUI_SERACH_PATH})
    if(NOT IMGUI_INCLUDE_DIR)
        message(FATAL_ERROR "---> IMGUI imgui.cpp not found. Set IMGUI_ROOT to imgui's top-level path (containing \"imgui.cpp\" and \"imgui.h\" files).\n")
    else()
        file(GLOB IMGUI_FILES
            "${IMGUI_INCLUDE_DIR}/imgui_draw.cpp"
            "${IMGUI_INCLUDE_DIR}/imgui_demo.cpp"
            "${IMGUI_INCLUDE_DIR}/imgui.cpp"
            "${IMGUI_INCLUDE_DIR}/imgui.h"
            "${IMGUI_INCLUDE_DIR}/imgui_widgets.cpp"
			"${IMGUI_INCLUDE_DIR}/imgui_tables.cpp"
                )
        include_directories(${IMGUI_INCLUDE_DIR})
        message("Found imgui.cpp in ${IMGUI_INCLUDE_DIR}")
        # Rename that pesky imconfig.h file for the user.
        install(FILES ${IMGUI_INCLUDE_DIR}/imconfig.h DESTINATION include RENAME imconfig-sample.h)
    endif()
endif()
file(GLOB_RECURSE INCLUDES "imgui_impl_glfw.h" "imgui_impl_opengl3.h" "imgui-config.h")
include_directories(${GLFW_DIR}/include ${IMGUI}/include)
file(GLOB_RECURSE SOURCES "imgui_impl_glfw.cpp" "imgui_impl_opengl3.cpp")
add_definitions(-DGLFW_STATIC)
add_library(imgui-glfw STATIC ${SOURCES} ${INCLUDES} ${IMGUI_FILES})
target_link_libraries(imgui-glfw) #glfw-graphics glfw-system glfw-window)
install(TARGETS imgui-glfw DESTINATION dist/lib)
install(FILES ${INCLUDES} DESTINATION dist/include)
