cmake_minimum_required( VERSION 2.6 )
project( opengl )

set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake" )
find_package( GLEW REQUIRED )
find_package( OpenGL REQUIRED )
find_package( GLM REQUIRED )
find_package( PkgConfig REQUIRED )
find_package( Assimp REQUIRED )
pkg_search_module( GLFW REQUIRED glfw3 )

if ( ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++14" )
  if ( ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-c++98-compat" )
  endif ()
  set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g" )
  set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2" )
elseif ( ${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC" )
  if ( CMAKE_CXX_FLAGS MATCHES "/W[0-4]" )
    string( REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
  else ()
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4" )
  endif ()
endif ()

set( LIB_NAME ${PROJECT_NAME} )
set( LIBRARY_TYPE STATIC )

set( SOURCE ${SOURCE}
  "${PROJECT_SOURCE_DIR}/src/cpp/window.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/shader.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/vertex.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/mesh.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/texture.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/transform.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/camera.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/model.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/baseLight.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/directionalLight.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/pointLight.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/spotLight.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/attenuation.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/skybox.cpp"
  "${PROJECT_SOURCE_DIR}/src/cpp/framebuffer.cpp"
  "${PROJECT_SOURCE_DIR}/thirdparty/stb_image/stb_image.c"
  )

set ( EXTRA_DIRS ${EXTRA_DIRS}
  src/cpp
  "thirdparty/catch"
  "thirdparty/stb_image"
  ${GLFW_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIRS}
  ${GLM_INCLUDE_DIRS}
  ${ASSIMP_INCLUDE_DIR}
  ${SOIL_INCLUDE_DIRS}
  )

set( EXTRA_LIB ${EXTRA_LIB}
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLM_LIBRARIES}
  ${ASSIMP_LIBRARY}
  ${SOIL_LIBRARIES}
  )

add_library ( ${LIB_NAME}
  ${LIBRARY_TYPE}
  ${SOURCE}
  )

include_directories( ${EXTRA_DIRS} )
target_link_libraries( ${LIB_NAME} ${EXTRA_LIB} )
add_executable( "${PROJECT_NAME}code" src/cpp/main.cpp )
add_executable( "${PROJECT_NAME}test" src/cpp/test.cpp )


target_link_libraries( "${PROJECT_NAME}code" ${LIB_NAME} )
target_link_libraries( "${PROJECT_NAME}test" ${LIB_NAME} )

enable_testing()
add_test( NAME RunTests COMMAND "${PROJECT_NAME}test" )

find_package( Doxygen )
if ( DOXYGEN_FOUND )
  configure_file( ${PROJECT_SOURCE_DIR}/Doxyfile ${PROJECT_BINARY_DIR}/Doxyfile @ONLY )
  add_custom_target( doc
    ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}/..
    COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
endif ( DOXYGEN_FOUND )
