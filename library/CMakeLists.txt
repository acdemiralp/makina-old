##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project               (makina VERSION 1.0 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_STANDARD 17)
set                   (CMAKE_CXX_VISIBILITY_PRESET hidden)
set                   (CMAKE_VISIBILITY_INLINES_HIDDEN 1)

include               (set_max_warning_level)
set_max_warning_level ()
if   (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4251 /wd4275")
else ()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-4251 -Wno-4275")
endif()

##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
option(BUILD_TESTS "Build tests." OFF)

##################################################    Sources     ##################################################
file(GLOB_RECURSE PROJECT_MISC *.md *.txt *.py)
file(GLOB_RECURSE PROJECT_CMAKE_UTILS cmake/*.cmake)
file(GLOB_RECURSE PROJECT_HPPS include/*.hpp include/*.h)
file(GLOB_RECURSE PROJECT_CPPS source/*.cpp source/*.c)
set (PROJECT_EXPORT_HPP "include/${PROJECT_NAME}/export.hpp")
set (PROJECT_SOURCES 
  ${PROJECT_MISC} 
  ${PROJECT_CMAKE_UTILS} 
  ${PROJECT_HPPS} 
  ${PROJECT_CPPS} 
  ${PROJECT_EXPORT_HPP})
  
include            (assign_source_group)
assign_source_group(${PROJECT_SOURCES} )

##################################################  Dependencies  ##################################################
include(add_conan_remote)
include(conan)
include(import_library)

set  (BINTRAY_USERNAME "" CACHE STRING "")
set  (BINTRAY_API_KEY  "" CACHE STRING "")
if   (BINTRAY_USERNAME STREQUAL "" OR BINTRAY_API_KEY STREQUAL "")
  message(FATAL_ERROR "BINTRAY_USERNAME and BINTRAY_API_KEY must be set!")
endif()

add_conan_remote(
  bincrafters 
  https://api.bintray.com/conan/bincrafters/public-conan 
  ${BINTRAY_USERNAME} 
  ${BINTRAY_API_KEY})
add_conan_remote(
  conan-community 
  https://api.bintray.com/conan/conan-community/conan 
  ${BINTRAY_USERNAME} 
  ${BINTRAY_API_KEY})
add_conan_remote(
  rwth-vr     
  https://api.bintray.com/conan/rwth-vr/conan            
  ${BINTRAY_USERNAME} 
  ${BINTRAY_API_KEY})
add_conan_remote(
  makina     
  https://api.bintray.com/conan/acdemiralp/makina        
  ${BINTRAY_USERNAME} 
  ${BINTRAY_API_KEY})
  
# Manual dependencies.
find_package(Cuda REQUIRED)
find_package(FMOD REQUIRED)
find_package(OpenGL REQUIRED)
find_package(OptiX REQUIRED)
find_package(Vulkan REQUIRED)

import_library(FMOD_INCLUDE_DIRS FMOD_LIBRARIES)
import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)
import_library(Vulkan_INCLUDE_DIR Vulkan_LIBRARY)

if   (CUDA_FOUND)
  list          (APPEND PROJECT_COMPILE_DEFINITIONS -DMAKINA_CUDA_SUPPORT)
  import_library(CUDA_INCLUDE_DIRS CUDA_LIBRARIES)
endif()
if   (OPTIX_FOUND)
  list          (APPEND PROJECT_COMPILE_DEFINITIONS -DMAKINA_OPTIX_SUPPORT)
  import_library(OPTIX_INCLUDE_DIR OPTIX_LIB)
endif()

list  (APPEND PROJECT_COMPILE_DEFINITIONS -DNOMINMAX -DGLM_FORCE_LEFT_HANDED -DGLM_FORCE_DEPTH_ZERO_TO_ONE)

if    (WIN32)
  list(APPEND PROJECT_COMPILE_DEFINITIONS -DVK_USE_PLATFORM_WIN32_KHR -DWIN32_LEAN_AND_MEAN)
elseif(APPLE)
  list(APPEND PROJECT_COMPILE_DEFINITIONS -DVK_USE_PLATFORM_WIN32_KHR)
elseif(UNIX )
  list(APPEND PROJECT_COMPILE_DEFINITIONS -DVK_USE_PLATFORM_XLIB_KHR)
endif ()

conan_cmake_run(CONANFILE conanfile.py BASIC_SETUP BUILD missing)
list           (APPEND PROJECT_INCLUDE_DIRS ${CONAN_INCLUDE_DIRS_RELEASE})
list           (APPEND PROJECT_COMPILE_DEFINITIONS ${CONAN_COMPILE_DEFINITIONS_RELEASE})
foreach        (_LIB ${CONAN_LIBS_DEBUG})
  list         (APPEND PROJECT_LIBRARIES debug ${_LIB})
endforeach     ()  
foreach        (_LIB ${CONAN_LIBS_RELEASE})
  list         (APPEND PROJECT_LIBRARIES optimized ${_LIB})
endforeach     ()

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
target_compile_definitions(${PROJECT_NAME} PUBLIC ${PROJECT_COMPILE_DEFINITIONS})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

##################################################    Postbuild   ##################################################
include               (GenerateExportHeader)
string                (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
generate_export_header(${PROJECT_NAME} 
  EXPORT_FILE_NAME     ${PROJECT_SOURCE_DIR}/include/${PROJECT_NAME}/export.hpp
  EXPORT_MACRO_NAME    ${PROJECT_NAME_UPPER}_EXPORT
  STATIC_DEFINE        ${PROJECT_NAME_UPPER}_STATIC
)

# Copy data folder to build directory.
file(COPY ${PROJECT_SOURCE_DIR}/../data DESTINATION ${PROJECT_BINARY_DIR})

##################################################    Testing     ##################################################
if(BUILD_TESTS)
  enable_testing     ()
  set                (TEST_MAIN_NAME catch_main)
  set                (TEST_MAIN_SOURCES tests/catch/main.cpp)
  add_library        (${TEST_MAIN_NAME} OBJECT ${TEST_MAIN_SOURCES})
  set_property       (TARGET ${TEST_MAIN_NAME} PROPERTY FOLDER tests/catch)
  assign_source_group(${TEST_MAIN_SOURCES})

  file(GLOB PROJECT_TEST_CPPS tests/*.cpp)
  foreach(_SOURCE ${PROJECT_TEST_CPPS})
    get_filename_component(_NAME ${_SOURCE} NAME_WE)
    add_executable        (${_NAME} ${_SOURCE} $<TARGET_OBJECTS:${TEST_MAIN_NAME}>)
    target_link_libraries (${_NAME} ${PROJECT_NAME})
    add_test              (${_NAME} ${_NAME})
    set_property          (TARGET ${_NAME} PROPERTY FOLDER tests)
    assign_source_group   (${_SOURCE})
  endforeach()
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}-config
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(EXPORT  ${PROJECT_NAME}-config DESTINATION cmake)
export (TARGETS ${PROJECT_NAME}        FILE        ${PROJECT_NAME}-config.cmake)