##################################################    Project     ##################################################
cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project               (makina VERSION 1.0 LANGUAGES CXX)
list                  (APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
set_property          (GLOBAL PROPERTY USE_FOLDERS ON)
set                   (CMAKE_CXX_VISIBILITY_PRESET hidden)
set                   (CMAKE_VISIBILITY_INLINES_HIDDEN 1)

##################################################    Options     ##################################################
option(BUILD_SHARED_LIBS "Build shared (dynamic) libraries." ON)
option(BUILD_TESTS "Build tests." OFF)

##################################################    Sources     ##################################################
set(PROJECT_SOURCES
  CMakeLists.txt

  cmake/assign_source_group.cmake
  cmake/import_library.cmake
  cmake/FindAssimp.cmake
  cmake/FindFMOD.cmake
  cmake/FindFreeImage.cmake
  cmake/FindFreeImagePlus.cmake
  cmake/FindOpenVR.cmake
  cmake/FindOptix.cmake
  cmake/FindSDL2.cmake
  
  include/makina/core/aspects/hierarchical.hpp
  include/makina/core/aspects/logged.hpp
  include/makina/core/aspects/named.hpp
  include/makina/core/assets/audio/importers/audio_clip_importer.hpp
  include/makina/core/assets/audio/importers/fmod_audio_clip_importer.hpp
  include/makina/core/assets/audio/audio_clip.hpp
  include/makina/core/assets/font/importers/font_importer.hpp
  include/makina/core/assets/font/importers/freetype_font_importer.hpp
  include/makina/core/assets/font/font.hpp
  include/makina/core/assets/image/importers/freeimage_image_importer.hpp
  include/makina/core/assets/image/importers/image_importer.hpp
  include/makina/core/assets/image/image.hpp
  include/makina/core/assets/model/importers/fbx_model_importer.hpp
  include/makina/core/assets/model/importers/model_importer.hpp
  include/makina/core/assets/model/animation_clip.hpp
  include/makina/core/assets/model/animation_curve.hpp
  include/makina/core/assets/model/bone.hpp
  include/makina/core/assets/model/keyframe.hpp
  include/makina/core/assets/model/material.hpp
  include/makina/core/assets/model/mesh.hpp
  include/makina/core/assets/model/model.hpp
  include/makina/core/entity_component_system/component.hpp
  include/makina/core/entity_component_system/component_mask.hpp
  include/makina/core/entity_component_system/entity.hpp
  include/makina/core/entity_component_system/scene.hpp
  include/makina/core/entity_component_system/system.hpp
  include/makina/core/rendering/framegraph/framegraph.hpp
  include/makina/core/rendering/framegraph/render_pass.hpp
  include/makina/core/rendering/framegraph/render_pass_resources.hpp
  include/makina/core/rendering/low_level_renderer/render_context.hpp
  include/makina/core/rendering/resources/types/render_target.hpp
  include/makina/core/rendering/resources/types/viewport.hpp
  include/makina/core/rendering/resources/resource.hpp
  include/makina/core/rendering/resources/resource_builder.hpp
  include/makina/core/rendering/resources/resource_registry.hpp
  include/makina/core/threading/task_graph.hpp
  include/makina/core/threading/thread_pool.hpp
  include/makina/core/time/frame_timer.hpp
  include/makina/core/utility/gpu_exports.hpp
  include/makina/core/engine.hpp
  include/makina/systems/audio/components/audio_listener.hpp
  include/makina/systems/audio/components/audio_source.hpp
  include/makina/systems/audio/audio_system.hpp
  include/makina/systems/display/outlets/display.hpp
  include/makina/systems/display/outlets/hmd.hpp
  include/makina/systems/display/outlets/window.hpp
  include/makina/systems/display/display_system.hpp
  include/makina/systems/input/types/key.hpp
  include/makina/systems/input/input_system.hpp
  include/makina/systems/physics/components/capsule_collider.hpp
  include/makina/systems/physics/components/collider.hpp
  include/makina/systems/physics/components/mesh_collider.hpp
  include/makina/systems/physics/components/obb_collider.hpp
  include/makina/systems/physics/components/rigidbody.hpp
  include/makina/systems/physics/physics_system.hpp
  include/makina/systems/rendering/components/mesh_render.hpp
  include/makina/systems/rendering/components/projection.hpp
  include/makina/systems/rendering/render_passes/blit_pass.hpp
  include/makina/systems/rendering/render_passes/clear_pass.hpp
  include/makina/systems/rendering/render_passes/deferred_shading_g_buffer_pass.hpp
  include/makina/systems/rendering/render_passes/deferred_shading_render_pass.hpp
  include/makina/systems/rendering/render_passes/forward_render_pass.hpp
  include/makina/systems/rendering/render_passes/openvr_tracking_pass.hpp
  include/makina/systems/rendering/render_passes/optix_trace_pass.hpp
  include/makina/systems/rendering/render_passes/post_to_display_pass.hpp
  include/makina/systems/rendering/rendering_system.hpp
  include/makina/systems/scripting/components/behaviour.hpp
  include/makina/systems/scripting/scripting_system.hpp
  include/makina/systems/transform/components/transform.hpp
  include/makina/systems/transform/transform_system.hpp
  include/makina/makina.hpp

  source/core/aspects/logged.cpp
  source/core/assets/audio/importers/fmod_audio_clip_importer.cpp
  source/core/assets/audio/audio_clip.cpp
  source/core/assets/font/importers/freetype_font_importer.cpp
  source/core/assets/font/font.cpp
  source/core/assets/image/importers/freeimage_image_importer.cpp
  source/core/assets/image/image.cpp
  source/core/assets/model/importers/fbx_model_importer.cpp
  source/core/assets/model/animation_clip.cpp
  source/core/assets/model/animation_curve.cpp
  source/core/assets/model/bone.cpp
  source/core/assets/model/keyframe.cpp
  source/core/assets/model/material.cpp
  source/core/assets/model/mesh.cpp
  source/core/assets/model/model.cpp
  source/core/entity_component_system/component.cpp
  source/core/entity_component_system/entity.cpp
  source/core/entity_component_system/scene.cpp
  source/core/entity_component_system/system.cpp
  source/core/rendering/framegraph/framegraph.cpp
  source/core/rendering/framegraph/render_pass.cpp
  source/core/rendering/framegraph/render_pass_resources.cpp
  source/core/rendering/low_level_renderer/render_context.cpp
  source/core/rendering/resources/types/render_target.cpp
  source/core/rendering/resources/types/viewport.cpp
  source/core/rendering/resources/resource.cpp
  source/core/rendering/resources/resource_builder.cpp
  source/core/rendering/resources/resource_registry.cpp
  source/core/threading/task_graph.cpp
  source/core/threading/thread_pool.cpp
  source/core/engine.cpp
  source/systems/audio/components/audio_listener.cpp
  source/systems/audio/components/audio_source.cpp
  source/systems/audio/audio_system.cpp
  source/systems/display/outlets/display.cpp
  source/systems/display/outlets/hmd.cpp
  source/systems/display/outlets/window.cpp
  source/systems/display/display_system.cpp
  source/systems/physics/components/capsule_collider.cpp
  source/systems/physics/components/collider.cpp
  source/systems/physics/components/mesh_collider.cpp
  source/systems/physics/components/obb_collider.cpp
  source/systems/physics/components/rigidbody.cpp
  source/systems/physics/physics_system.cpp
  source/systems/rendering/components/mesh_render.cpp
  source/systems/rendering/components/projection.cpp
  source/systems/rendering/render_passes/blit_pass.cpp
  source/systems/rendering/render_passes/clear_pass.cpp
  source/systems/rendering/render_passes/deferred_shading_g_buffer_pass.cpp
  source/systems/rendering/render_passes/deferred_shading_render_pass.cpp
  source/systems/rendering/render_passes/forward_render_pass.cpp
  source/systems/rendering/render_passes/openvr_tracking_pass.cpp
  source/systems/rendering/render_passes/optix_trace_pass.cpp
  source/systems/rendering/render_passes/post_to_display_pass.cpp
  source/systems/rendering/rendering_system.cpp
  source/systems/scripting/components/behaviour.cpp
  source/systems/scripting/scripting_system.cpp
  source/systems/transform/components/transform.cpp
  source/systems/transform/transform_system.cpp
  source/makina.cpp
)
include(assign_source_group)
assign_source_group(${PROJECT_SOURCES})

set         (EXPORT_HEADER "${PROJECT_BINARY_DIR}/export.hpp")
list        (APPEND PROJECT_SOURCES ${EXPORT_HEADER})
source_group("include\\${PROJECT_NAME}" FILES ${EXPORT_HEADER})

##################################################  Dependencies  ##################################################
include(import_library)

find_package  (Assimp REQUIRED)
import_library(ASSIMP_INCLUDE_DIR ASSIMP_LIBRARY_DEBUG ASSIMP_LIBRARY_RELEASE)

find_package  (Boost REQUIRED)
import_library(Boost_INCLUDE_DIRS)

find_package  (Bullet REQUIRED)
set(BULLET_LIBRARIES_DEBUG 
  ${BULLET_DYNAMICS_LIBRARY_DEBUG}
  ${BULLET_COLLISION_LIBRARY_DEBUG}
  ${BULLET_MATH_LIBRARY_DEBUG}
  ${BULLET_SOFTBODY_LIBRARY_DEBUG})
set(BULLET_LIBRARIES_RELEASE 
  ${BULLET_DYNAMICS_LIBRARY}
  ${BULLET_COLLISION_LIBRARY}
  ${BULLET_MATH_LIBRARY}
  ${BULLET_SOFTBODY_LIBRARY})
import_library(BULLET_INCLUDE_DIRS BULLET_LIBRARIES_DEBUG BULLET_LIBRARIES_RELEASE)

find_package (Cuda REQUIRED)
set(CUDA_NVCC_FLAGS 
  ${CUDA_NVCC_FLAGS} 
  "-gencode=arch=compute_35,code=sm_35" # Kepler
  "-gencode=arch=compute_50,code=sm_50" # Maxwell
  "-gencode=arch=compute_52,code=sm_52" # Maxwell
  "-gencode=arch=compute_61,code=sm_61" # Pascal
  "--expt-extended-lambda")
import_library(CUDA_INCLUDE_DIRS CUDA_LIBRARIES)

find_package  (FMOD REQUIRED)
import_library(FMOD_INCLUDE_DIRS FMOD_LIBRARIES)

find_package  (FreeImage REQUIRED)
import_library(FREEIMAGE_INCLUDE_DIRS FREEIMAGE_LIBRARIES)

find_package  (FreeImagePlus REQUIRED)
import_library(FREEIMAGEPLUS_INCLUDE_DIRS FREEIMAGEPLUS_LIBRARIES)

find_package  (FreeType REQUIRED)
import_library(FREETYPE_INCLUDE_DIRS FREETYPE_LIBRARY_DEBUG FREETYPE_LIBRARY_RELEASE)
       
find_package  (gl REQUIRED)
list          (APPEND PROJECT_LIBRARIES gl)
      
find_package  (GLEW REQUIRED)
import_library(GLEW_INCLUDE_DIRS GLEW_LIBRARIES)
              
find_package  (GLM REQUIRED)
import_library(GLM_INCLUDE_DIRS)
              
find_package  (OpenGL REQUIRED)
import_library(OPENGL_INCLUDE_DIR OPENGL_LIBRARIES)

find_package  (OpenVR REQUIRED)
import_library(OPENVR_INCLUDE_DIRS OPENVR_LIBRARY)

find_package  (Optix REQUIRED)
import_library(OPTIX_INCLUDE_DIR OPTIX_LIB)

find_package  (SDL2 REQUIRED)
import_library(SDL2_INCLUDE_DIR SDL2_LIBRARY)
if(WIN32)
  set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} winmm.lib Imm32.lib)
endif()

##################################################    Targets     ##################################################
add_library(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  $<INSTALL_INTERFACE:include> PRIVATE source)
target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_INCLUDE_DIRS})
target_link_libraries     (${PROJECT_NAME} PUBLIC ${PROJECT_LIBRARIES})
set_target_properties     (${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)

if(NOT BUILD_SHARED_LIBS)
  string               (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
  set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS -D${PROJECT_NAME_UPPER}_STATIC)
endif()

##################################################    Postbuild   ##################################################
include               (GenerateExportHeader)
string                (TOUPPER ${PROJECT_NAME} PROJECT_NAME_UPPER)
generate_export_header(${PROJECT_NAME} 
  EXPORT_FILE_NAME     export.hpp
  EXPORT_MACRO_NAME    ${PROJECT_NAME_UPPER}_EXPORT
  STATIC_DEFINE        ${PROJECT_NAME_UPPER}_STATIC
)

##################################################    Testing     ##################################################
if(BUILD_TESTS)
  enable_testing()

  set(PROJECT_TEST_SOURCES
  
  )

  foreach(_SOURCE ${PROJECT_TEST_SOURCES})
    get_filename_component(_NAME ${_SOURCE} NAME_WE)
    set                   (_SOURCES tests/catch.hpp tests/main.cpp ${_SOURCE})
    add_executable        (${_NAME} ${_SOURCES})
    target_link_libraries (${_NAME} ${PROJECT_NAME})
    add_test              (${_NAME} ${_NAME})
    set_property          (TARGET ${_NAME} PROPERTY FOLDER "Tests")
    source_group          ("source" FILES ${_SOURCES})
  endforeach()
endif()

##################################################  Installation  ##################################################
install(TARGETS ${PROJECT_NAME} EXPORT "${PROJECT_NAME}-config"
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
install(FILES   "${PROJECT_BINARY_DIR}/export.hpp" DESTINATION "include/${PROJECT_NAME}")
install(EXPORT  "${PROJECT_NAME}-config"           DESTINATION "cmake")
export (TARGETS "${PROJECT_NAME}"                  FILE        "${PROJECT_NAME}-config.cmake")
