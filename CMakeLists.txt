cmake_minimum_required(VERSION 3.24.1)

# set the project name and version
project(MUSHROOM VERSION 0.3.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" )

#set EXTERNAL_DIR
set(EXTERNAL_DIR ${CMAKE_SOURCE_DIR}/external)
#set INCLUDE_DIR and SRC_DIR
include_directories(include ${CMAKE_SOURCE_DIR}/include)

include(GNUInstallDirs)

if (COMMAND cmake_policy)
  cmake_policy (SET CMP0003 NEW)
  cmake_policy (SET CMP0042 NEW)
endif()

set(CMAKE_DEBUG_POSTFIX d)

set(SDL_STATIC ON CACHE BOOL "" FORCE )
set(SDL_SHARED OFF CACHE BOOL "" FORCE )
find_package(SDL2 REQUIRED)
add_subdirectory(${EXTERNAL_DIR}/sdl2)
include_directories(${SDL2_INCLUDE_DIR})
LIST(APPEND MUSHROOM_LIBRARIES SDL2main SDL2-static)


option (BUILD_UTILS "utilities" ON)
option (GLEW_REGAL "Regal mode" OFF)
option (GLEW_OSMESA "OSMesa mode" OFF)
if (APPLE)
    option (BUILD_FRAMEWORK "Build Framework bundle for OSX" OFF)
endif ()

set (GLEW_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/glew)

# get version from config/version
file (STRINGS ${GLEW_DIR}/config/version  _VERSION_MAJOR_STRING REGEX "GLEW_MAJOR[ ]*=[ ]*[0-9]+.*")
string (REGEX REPLACE "GLEW_MAJOR[ ]*=[ ]*([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_MAJOR ${_VERSION_MAJOR_STRING})
file (STRINGS ${GLEW_DIR}/config/version  _VERSION_MINOR_STRING REGEX "GLEW_MINOR[ ]*=[ ]*[0-9]+.*")
string (REGEX REPLACE "GLEW_MINOR[ ]*=[ ]*([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_MINOR ${_VERSION_MINOR_STRING})
file (STRINGS ${GLEW_DIR}/config/version  _VERSION_PATCH_STRING REGEX "GLEW_MICRO[ ]*=[ ]*[0-9]+.*")
string (REGEX REPLACE "GLEW_MICRO[ ]*=[ ]*([0-9]+)" "\\1" CPACK_PACKAGE_VERSION_PATCH ${_VERSION_PATCH_STRING})
set (GLEW_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH})

set (CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package (OpenGL REQUIRED)

# X11 required when builing visualinfo and glewinfo,
# but not for Windows or Apple OSX platforms
if (BUILD_UTILS AND NOT WIN32 AND NOT APPLE)
  find_package (X11)
endif()

if (WIN32)
  set (GLEW_LIB_NAME glew32)
else ()
  set (GLEW_LIB_NAME GLEW)
  set (DLL_PREFIX lib)
endif ()

set (GLEW_LIBRARIES ${OPENGL_LIBRARIES})

add_definitions (-DGLEW_NO_GLU)

#### Regal mode ####

if (GLEW_REGAL)
  if (WIN32)
    set (REGAL_LIB_NAME regal32)
  else ()
    set (REGAL_LIB_NAME Regal)
  endif ()
  add_definitions (-DGLEW_REGAL)
  set (GLEW_LIBRARIES ${REGAL_LIB_NAME})
endif ()

#### OSMesa mode ####

if (GLEW_OSMESA)
  if (WIN32)
    set (OSMESA_LIB_NAME osmesa)
  else ()
    set (OSMESA_LIB_NAME OSMesa)
  endif ()
  add_definitions (-DGLEW_OSMESA)
  set (GLEW_LIBRARIES ${OSMESA_LIB_NAME} ${OPENGL_LIBRARIES})
  set (X11_LIBRARIES)
endif ()

#### GLEW ####

include_directories (${EXTERNAL_DIR}/glew/include)

set (GLEW_PUBLIC_HEADERS_FILES ${GLEW_DIR}/include/GL/wglew.h ${GLEW_DIR}/include/GL/glew.h ${GLEW_DIR}/include/GL/glxew.h)
set (GLEW_SRC_FILES ${GLEW_DIR}/src/glew.c)

if (WIN32)
  list (APPEND GLEW_SRC_FILES ${GLEW_DIR}/build/glew.rc)
endif ()

add_library (glew_s STATIC ${GLEW_PUBLIC_HEADERS_FILES} ${GLEW_SRC_FILES})
set_target_properties (glew_s PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC" OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX lib)

if (MSVC)
  # add options from visual studio project
  target_compile_definitions (glew_s PRIVATE "GLEW_STATIC;VC_EXTRALEAN")
  # kill security checks which are dependent on stdlib
  target_compile_options (glew_s PRIVATE -GS-)
elseif (WIN32 AND ((CMAKE_C_COMPILER_ID MATCHES "GNU") OR (CMAKE_C_COMPILER_ID MATCHES "Clang")))
  # remove stdlib dependency on windows with GCC and Clang (for similar reasons
  # as to MSVC - to allow it to be used with any Windows compiler)
  target_compile_options (glew_s PRIVATE -fno-builtin -fno-stack-protector)
  target_link_libraries (glew LINK_PRIVATE -nostdlib)
endif ()

if (BUILD_FRAMEWORK)
  set_target_properties(glew PROPERTIES
    FRAMEWORK TRUE
    FRAMEWORK_VERSION ${GLEW_VERSION}
    MACOSX_FRAMEWORK_IDENTIFIER net.sourceforge.glew
    MACOSX_FRAMEWORK_SHORT_VERSION_STRING ${GLEW_VERSION}
    MACOSX_FRAMEWORK_BUNDLE_VERSION ${GLEW_VERSION}
    XCODE_ATTRIBUTE_INSTALL_PATH "@rpath"
    PUBLIC_HEADER "${GLEW_PUBLIC_HEADERS_FILES}"
    OUTPUT_NAME GLEW
  )
endif()

LIST(APPEND MUSHROOM_LIBRARIES glew_s ${GLEW_LIBRARIES})

if(CMAKE_VERSION VERSION_LESS 2.8.12)
  set(MAYBE_EXPORT "")
else()
  target_compile_definitions(glew_s INTERFACE "GLEW_STATIC")
  foreach(t glew_s)
    target_include_directories(${t} PUBLIC $<INSTALL_INTERFACE:include>)
  endforeach()
  set(MAYBE_EXPORT EXPORT glew-targets)
endif()

if(NOT DEFINED BUILD_SHARED_LIBS OR NOT BUILD_SHARED_LIBS)
  list(APPEND targets_to_install glew_s)
endif()


set (prefix ${CMAKE_INSTALL_PREFIX})
set (exec_prefix ${CMAKE_INSTALL_PREFIX})
set (libdir ${CMAKE_INSTALL_FULL_LIBDIR})
set (includedir ${CMAKE_INSTALL_FULL_INCLUDEDIR})
set (version ${GLEW_VERSION})
set (libname ${GLEW_LIB_NAME})
set (cflags)
set (requireslib glu)

#  Mac OSX has no glu.pc unless optional X11/GLX is installed
if (APPLE)
  set (requireslib)
endif ()

configure_file (${GLEW_DIR}/glew.pc.in ${GLEW_DIR}/glew.pc @ONLY)

install(FILES ${GLEW_DIR}/glew.pc
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig
)

install (FILES
    ${GLEW_DIR}/include/GL/wglew.h
    ${GLEW_DIR}/include/GL/glew.h
    ${GLEW_DIR}/include/GL/glxew.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/GL)


if(NOT TARGET uninstall)
  configure_file(
      ${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in
      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
      IMMEDIATE @ONLY)

  add_custom_target(uninstall
      COMMAND ${CMAKE_COMMAND} -P
      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

#GLM
set(GLM ${EXTERNAL_DIR}/glm/)
add_subdirectory(${GLM})
include_directories(${GLM})
LIST(APPEND MUSHROOM_LIBRARIES glm)

set(ASSIMP ${EXTERNAL_DIR}/assimp/)
	add_subdirectory(${ASSIMP})
	include_directories(${ASSIMP})
	LIST(APPEND MUSHROOM_LIBRARIES assimp)

#FreeType
set(FREETYPE ${EXTERNAL_DIR}/freetype2/)
add_subdirectory(${FREETYPE})
LIST(APPEND MUSHROOM_LIBRARIES freetype)


#IMGUI SDL
set(SDL_IMGUI ${EXTERNAL_DIR}/imgui-sdl/)
set(IMGUI_ROOT ${EXTERNAL_DIR}/imgui/)
set(SDL_ROOT ${EXTERNAL_DIR}/SDL2/)
add_subdirectory(${SDL_IMGUI})
include_directories(${IMGUI_ROOT} ${SDL_IMGUI})
LIST(APPEND MUSHROOM_LIBRARIES imgui-sdl)




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

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})


# add the executable
add_executable(MUSHROOM src/main.cpp)
target_compile_definitions(MUSHROOM PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
MESSAGE("MUSHROOM_COMMON: " ${MUSHROOM_LIBRARIES})
target_link_libraries(MUSHROOM PUBLIC MUSHROOM_COMMON)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/shader/)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/data/)
