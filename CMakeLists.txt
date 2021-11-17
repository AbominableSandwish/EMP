cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(MUSHROOM VERSION 0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)



#set EXTERNAL_DIR
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)
#set INCLUDE_DIR and SRC_DIR
include_directories(include ${CMAKE_SOURCE_DIR}/include)
file(GLOB_RECURSE MUSHROOM_SRC src/*.cpp include/*.h)

#GLEW
set(GLEW_DIR ${EXTERNAL_DIR}/glew)
	add_compile_definitions(GLEW_STATIC)
	include_directories(${GLEW_DIR}/include)
	add_subdirectory(${GLEW_DIR})
	LIST(APPEND MUSHROOM_LIBRARIES
			glew)

#GLFW
find_package(OpenGL REQUIRED)
LIST(APPEND MUSHROOM_LIBRARIES OpenGL::GL)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(${EXTERNAL_DIR}/glfw3)
include_directories(${EXTERNAL_DIR}/glfw3/include)
LIST(APPEND MUSHROOM_LIBRARIES glfw3)

#IMGUI GLFW
set(GLFW_IMGUI ${EXTERNAL_DIR}/imgui-glfw/)
set(IMGUI_ROOT ${EXTERNAL_DIR}/imgui/)

set(GLFW_ROOT ${EXTERNAL_DIR}/GLFW/)
add_subdirectory(${GLFW_IMGUI})
include_directories(${IMGUI_ROOT} ${GLFW_IMGUI})
LIST(APPEND MUSHROOM_LIBRARIES imgui-glfw)

#GLM
set(GLM ${EXTERNAL_DIR}/glm/)
add_subdirectory(${GLM})
include_directories(${GLM} ${GLM})
LIST(APPEND MUSHROOM_LIBRARIES glm)

# FreeType
#set(FREETYPE_INCLUDE_DIRS ${EXTERNAL_DIR}/freetype2/include/)
set(FREETYPE ${EXTERNAL_DIR}/freetype2/)
add_subdirectory(${FREETYPE})
#find_package(Freetype REQUIRED)
#target_link_libraries(main PRIVATE ${FREETYPE_LIBRARIES})
#target_include_directories(MUSHROOM_COMMON PRIVATE ${FREETYPE_INCLUDE_DIRS})
LIST(APPEND MUSHROOM_LIBRARIES freetype)

#MUSHROOM_CORE
if(NOT WIN32)
LIST(APPEND MUSHROOM_LIBRARIES
  stdc++fs)
ENDIF()
file(GLOB_RECURSE CORE_SRC src/core/*.cpp include/core/*.h)
add_library(MUSHROOM_CORE STATIC ${CORE_SRC})
include_directories(include/)
target_link_libraries(MUSHROOM_CORE PUBLIC MUSHROOM_COMMON)

#MUSHROOM_COMMON
if(NOT WIN32)
LIST(APPEND MUSHROOM_LIBRARIES
  stdc++fs)
ENDIF()
add_library(MUSHROOM_COMMON STATIC ${MUSHROOM_SRC})
include_directories(include/)
target_link_libraries(MUSHROOM_COMMON PUBLIC ${MUSHROOM_LIBRARIES})

#MUSHROOM_TOOL
if(NOT WIN32)
LIST(APPEND MUSHROOM_LIBRARIES
  stdc++fs)
ENDIF()
file(GLOB_RECURSE TOOL_SRC src/tool/*.cpp include/tool/*.h)
add_library(MUSHROOM_TOOL STATIC ${TOOL_SRC})
include_directories(include/)
target_link_libraries(MUSHROOM_TOOL PRIVATE MUSHROOM_COMMON)

#MUSHROOM_TEST
SET(MUSHROOM_TEST_DIR ${CMAKE_SOURCE_DIR}/tests)
file(GLOB TEST_FILES ${MUSHROOM_TEST_DIR}/*.cpp )
add_executable(MUSHROOM_TEST ${TEST_FILES} tests/test.cpp)
target_link_libraries(MUSHROOM_TEST MUSHROOM_COMMON)

# add the executable
add_executable(MUSHROOM src/main.cpp)
target_compile_definitions(MUSHROOM PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
MESSAGE("MUSHROOM_COMMON: " ${MUSHROOM_LIBRARIES})
target_link_libraries(MUSHROOM PUBLIC MUSHROOM_COMMON)
