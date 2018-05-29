#ifndef MAKINA_API_HPP_
#define MAKINA_API_HPP_

#ifdef _WIN32
// Needs to be right here due to Windows antics.
#include <windows.h>
#undef MemoryBarrier
#endif

// Exposed libraries.
#include <fi/free_image.hpp>

// Exposed local content.
#include <makina/audio/audio_listener.hpp>
#include <makina/audio/audio_source.hpp>
#include <makina/audio/audio_system.hpp>
#include <makina/core/engine.hpp>
#include <makina/core/frame_timer.hpp>
#include <makina/core/logger.hpp>
#include <makina/core/metadata.hpp>
#include <makina/core/operating_system.hpp>
#include <makina/core/registry.hpp>
#include <makina/core/scene.hpp>
#include <makina/core/system.hpp>
#include <makina/display/display_system.hpp>
#include <makina/input/controller.hpp>
#include <makina/input/input_system.hpp>
#include <makina/input/wasd_controller.hpp>
#include <makina/physics/mesh_collider.hpp>
#include <makina/physics/physics_system.hpp>
#include <makina/physics/rigidbody.hpp>
#include <makina/renderer/animation_system.hpp>
#include <makina/renderer/animator.hpp>
#include <makina/renderer/bone.hpp>
#include <makina/renderer/im3d_context.hpp>
#include <makina/renderer/light.hpp>
#include <makina/renderer/line_render.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/point_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/renderer.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/renderer/volume_render.hpp>
#include <makina/resources/animation_clip.hpp>
#include <makina/resources/audio_clip.hpp>
#include <makina/resources/audio_clip_load.hpp>
#include <makina/resources/dynamic_material.hpp>
#include <makina/resources/field.hpp>
#include <makina/resources/field_load.hpp>
#include <makina/resources/image.hpp>
#include <makina/resources/image_load.hpp>
#include <makina/resources/line_segments.hpp>
#include <makina/resources/mesh.hpp>
#include <makina/resources/model.hpp>
#include <makina/resources/model_load.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>
#include <makina/resources/point_cloud.hpp>
#include <makina/resources/transfer_function.hpp>
#include <makina/scripting/behavior.hpp>
#include <makina/scripting/scripting_system.hpp>
#include <makina/ui/imgui_context.hpp>
#include <makina/ui/ui_system.hpp>
#include <makina/utility/enable_dedicated_gpu.hpp>
#include <makina/utility/random_number_generator.hpp>
#include <makina/vr/vr_system.hpp>
#include <makina/default_engine.hpp>

#endif