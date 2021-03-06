cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(MUSHROOM VERSION 0.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set EXTERNAL_DIR
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/externals)
#set INCLUDE_DIR and SRC_DIR
include_directories(include ${CMAKE_SOURCE_DIR}/include)

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
include_directories(${GLM})
LIST(APPEND MUSHROOM_LIBRARIES glm)

#JSON
set(JSON_BuildTests OFF CACHE INTERNAL "")
#find_package(nlohmann_json REQUIRED)
#add_subdirectory(${nlohmann_json_source} ${EXTERNAL_DIR}/json-3.10.4/include/nlohmann_json)
#LIST(APPEND MUSHROOM_LIBRARIES  nlohmann_json::nlohmann_json)


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
#file(GLOB_RECURSE MUSHROOM_SRC src/*.cpp include/*.h)

set_property(GLOBAL PROPERTY USE_FOLDERS On)
SET_SOURCE_FILES_PROPERTIES( ${MUSHROOM_SRC} PROPERTIES
                FOLDER "${CMAKE_SOURCE_DIR}/src" )
				
file(GLOB_RECURSE CORE_HEAD	src/core/*.cpp include/core/*.h include/core/*.tpp)
source_group("Core" FILES ${CORE_HEAD})
file(GLOB_RECURSE COMMON_HEAD src/common/*.cpp include/common/*.h)
source_group("Common" FILES ${COMMON_HEAD})
file(GLOB_RECURSE EDITOR_HEAD	src/editor/*.cpp include/editor/*.h)
source_group("Editor" FILES ${EDITOR_HEAD})
file(GLOB_RECURSE TOOL_HEAD	src/tool/*.cpp include/tool/*.h)
source_group("Tool" FILES ${TOOL_HEAD})
file(GLOB_RECURSE GRAPHIC_HEAD	src/graphic/*.cpp include/graphic/*.h)
source_group("Graphic" FILES ${GRAPHIC_HEAD})
file(GLOB_RECURSE COMPONENT_HEAD src/components/*.cpp include/components/*.h)
source_group("Components" FILES ${COMPONENT_HEAD})
file(GLOB_RECURSE PHYSIC_HEAD src/physic/*.cpp include/physic/*.h)
source_group("Physic" FILES ${PHYSIC_HEAD})
file(GLOB_RECURSE MATH_HEAD src/math/*.cpp include/math/*.h)
source_group("Math" FILES ${MATH_HEAD})

List(APPEND MUSHROOM_SRC  ${CORE_HEAD} ${COMMON_HEAD} ${EDITOR_HEAD} ${TOOL_HEAD} ${GRAPHIC_HEAD} ${COMPONENT_HEAD} ${PHYSIC_HEAD} ${MATH_HEAD})

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
target_link_libraries(MUSHROOM_TOOL PUBLIC MUSHROOM_COMMON)

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
