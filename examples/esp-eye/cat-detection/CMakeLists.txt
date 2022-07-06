cmake_minimum_required(VERSION 3.5)
set(PROJECT_PATH ${CMAKE_SOURCE_DIR})
set(PREFIX ${PROJECT_PATH}/../../..)
set(FOTAHUB_SRCDIR )
set(FOTAHUB_LIBDIR ${PREFIX}/lib)
set(FOTAHUB_INCDIR ${PREFIX}/include)
list(APPEND EXTRA_COMPONENT_DIRS
$ENV{WHO_PATH}/components
if(EXISTS ${FOTAHUB_INCDIR}/CMakeLists.txt)
  ${FOTAHUB_INCDIR}
endif()
)
add_compile_options(-fdiagnostics-color=always)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(demoproduct)
