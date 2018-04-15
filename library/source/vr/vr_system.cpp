#include <makina/vr/vr_system.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x4.hpp>

#include <makina/core/registry.hpp>
#include <makina/core/scene.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/resources/model.hpp>
#include <makina/resources/phong_material.hpp>

namespace mak
{
namespace detail
{
glm::mat4  convert_to_glm_matrix        (const std::array<float, 12>& matrix)
{
  return glm::transpose(glm::mat4(glm::make_mat3x4(matrix.data())));
}
glm::mat4  convert_to_glm_matrix        (const std::array<float, 16>& matrix)
{
  return glm::transpose(glm::make_mat4(matrix.data()));
}
template <di::tracking_device_type type>
transform* create_tracking_device_entity(di::tracking_device<type>* tracking_device, model* model, scene* scene)
{
  const auto openvr_model   = tracking_device->model();
  if (!openvr_model) 
    return nullptr;

  const auto openvr_texture = openvr_model->texture();
  if (!openvr_texture) 
    return nullptr;

  model->meshes   .push_back(std::make_unique<mak::mesh>          ());
  model->materials.push_back(std::make_unique<mak::phong_material>());
  auto mesh     = model->meshes.back().get();
  auto material = dynamic_cast<mak::phong_material*>(model->materials.back().get());
  mesh    ->vertices           .assign   (reinterpret_cast<glm::vec3*>(&openvr_model->vertices.front()), reinterpret_cast<glm::vec3*>(&openvr_model->vertices.back()));
  mesh    ->normals            .assign   (reinterpret_cast<glm::vec3*>(&openvr_model->normals .front()), reinterpret_cast<glm::vec3*>(&openvr_model->normals .back()));
  mesh    ->texture_coordinates.reserve  (openvr_model->texture_coordinates.size());
  for (auto& texture_coordinate : openvr_model->texture_coordinates)
    mesh  ->texture_coordinates.push_back(glm::vec3(texture_coordinate[0], texture_coordinate[1], 0.0f));
  mesh    ->indices            .assign   (openvr_model->indices.begin(), openvr_model->indices.end());
  material->diffuse_image      = std::make_unique<mak::image>(openvr_texture->data.data(), openvr_texture->size);

  auto entity           = scene->add_entity();
  auto transform        = entity->add_component<mak::transform>  ();
  auto mesh_render      = entity->add_component<mak::mesh_render>();
  mesh_render->mesh     = mesh;
  mesh_render->material = material;

  if(type == di::tracking_device_type::hmd)
  {
    const auto hmd = dynamic_cast<di::hmd*>(tracking_device);
    {
      auto eye            = scene ->add_entity();
      auto eye_transform  = entity->add_component<mak::transform> ();
      auto eye_projection = entity->add_component<mak::projection>();
      eye_transform ->set_parent(transform);
      eye_transform ->set_matrix(convert_to_glm_matrix(hmd->eye_to_head_transform(di::eye::left )));
      eye_projection->set_matrix(convert_to_glm_matrix(hmd->projection_matrix    (di::eye::left , 0.1f, 10000.0f)));
    }
    {
      auto eye            = scene ->add_entity();
      auto eye_transform  = entity->add_component<mak::transform> ();
      auto eye_projection = entity->add_component<mak::projection>();
      eye_transform ->set_parent(transform);
      eye_transform ->set_matrix(convert_to_glm_matrix(hmd->eye_to_head_transform(di::eye::right)));
      eye_projection->set_matrix(convert_to_glm_matrix(hmd->projection_matrix    (di::eye::right, 0.1f, 10000.0f)));
    }
  }
  
  return transform;
}
}

vr_system::vr_system(const di::tracking_mode tracking_mode) : di::vr_system(tracking_mode)
{

}

void vr_system::prepare(                             scene* scene)
{
  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();

  for (auto device : hmds                    ())
    hmd_transform_map_                    [device] = detail::create_tracking_device_entity(device, &model, scene);
  for (auto device : controllers             ())
    controller_transform_map_             [device] = detail::create_tracking_device_entity(device, &model, scene);
  for (auto device : tracking_references     ())
    tracking_reference_transform_map_     [device] = detail::create_tracking_device_entity(device, &model, scene);
  for (auto device : display_redirects       ())
    display_redirect_transform_map_       [device] = detail::create_tracking_device_entity(device, &model, scene);
  for (auto device : generic_tracking_devices())
    generic_tracking_device_transform_map_[device] = detail::create_tracking_device_entity(device, &model, scene);
}
void vr_system::update (frame_timer::duration delta, scene* scene)
{
  tick();
  
  for (auto& device : hmd_transform_map_)
    device.second->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix), true);
  for (auto& device : controller_transform_map_)
    device.second->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix), true);
  for (auto& device : tracking_reference_transform_map_)
    device.second->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix), true);
  for (auto& device : display_redirect_transform_map_)
    device.second->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix), true);
  for (auto& device : generic_tracking_device_transform_map_)
    device.second->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix), true);
}
}
