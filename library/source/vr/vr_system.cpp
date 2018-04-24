#include <makina/vr/vr_system.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat3x4.hpp>
#include <glm/mat4x4.hpp>

#include <makina/core/registry.hpp>
#include <makina/core/scene.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/projection.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/model.hpp>
#include <makina/resources/physically_based_material.hpp>

namespace mak
{
namespace detail
{
glm::mat4  handedness_conversion_matrix ()
{
  return glm::mat4(
    1.0f,  0.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,
    0.0f,  0.0f, -1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  1.0f);
}
glm::mat4  convert_to_glm_matrix        (const std::array<float, 12>& matrix)
{
  const auto& m = reinterpret_cast<const vr::HmdMatrix34_t*>(matrix.data())->m;
  return 
    handedness_conversion_matrix() * 
    glm::mat4(
      m[0][0], m[1][0], m[2][0], 0.0f, 
      m[0][1], m[1][1], m[2][1], 0.0f, 
      m[0][2], m[1][2], m[2][2], 0.0f, 
      m[0][3], m[1][3], m[2][3], 1.0f) *
    handedness_conversion_matrix();
}
glm::mat4  convert_to_glm_matrix        (const std::array<float, 16>& matrix)
{
  const auto& m = reinterpret_cast<const vr::HmdMatrix44_t*>(matrix.data())->m;
  return 
    handedness_conversion_matrix() *
    glm::mat4(
      m[0][0], m[1][0], m[2][0], m[3][0],
      m[0][1], m[1][1], m[2][1], m[3][1],
      m[0][2], m[1][2], m[2][2], m[3][2],
      m[0][3], m[1][3], m[2][3], m[3][3]) *
    handedness_conversion_matrix();
}
glm::mat4  make_projection_matrix       (const di::rectangle<float>& rectangle, const float near, const float far)
{
  return glm::frustum(near * rectangle.left, near * rectangle.right, near * rectangle.top, near * rectangle.bottom, near, far);
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

  model->meshes   .push_back(std::make_unique<mak::mesh>                     ());
  model->materials.push_back(std::make_unique<mak::physically_based_material>()); // TODO: Add support for other type of materials.
  auto mesh     = model->meshes.back().get();
  auto material = dynamic_cast<mak::physically_based_material*>(model->materials.back().get());
  mesh->set_name           (tracking_device->render_model_name());
  mesh->vertices           .reserve(openvr_model->vertices           .size());
  mesh->normals            .reserve(openvr_model->normals            .size());
  mesh->texture_coordinates.reserve(openvr_model->texture_coordinates.size());
  mesh->indices            .reserve(openvr_model->indices            .size());

  for (auto& vertex : openvr_model->vertices)
    mesh->vertices           .push_back({vertex[0], vertex[1], -vertex[2]});
  for (auto& normal : openvr_model->normals )
    mesh->normals            .push_back({normal[0], normal[1], -normal[2]});
  for (auto& texture_coordinate : openvr_model->texture_coordinates)
    mesh->texture_coordinates.push_back({texture_coordinate[0], texture_coordinate[1], 0.0f});
  for (auto i = 0; i < openvr_model->indices.size(); i+=3)
    mesh->indices            .insert(mesh->indices.end(), {openvr_model->indices[i + 0], openvr_model->indices[i + 2], openvr_model->indices[i + 1]});

  material->albedo_image = std::make_unique<mak::image>(openvr_texture->data.data(), openvr_texture->size, fi::type::bitmap, 32, std::array<fi::color_mask, 3>{fi::color_mask::red, fi::color_mask::green, fi::color_mask::blue});
  material->albedo_image->to_32_bits();

  auto entity           = scene->add_entity();
  auto metadata         = entity->add_component<mak::metadata>   ();
  auto transform        = entity->add_component<mak::transform>  (metadata);
  auto mesh_render      = entity->add_component<mak::mesh_render>();
  metadata   ->entity   = entity;
  metadata   ->name     = mesh->name();
  mesh_render->mesh     = mesh;
  mesh_render->material = material;

  if(type == di::tracking_device_type::hmd)
  {
    metadata->active = false;

    const auto hmd = dynamic_cast<di::hmd*>(tracking_device);
    {
      auto eye            = scene->add_entity();
      auto eye_metadata   = eye  ->add_component<mak::metadata>  ();
      auto eye_transform  = eye  ->add_component<mak::transform> (metadata);
      auto eye_projection = eye  ->add_component<mak::projection>();
      metadata->entity    = eye;
      metadata->name      = "HMD Left Camera";
      metadata->tags.push_back("hmd_left_camera");
      eye_transform ->set_parent(transform);
      eye_transform ->set_matrix(convert_to_glm_matrix (hmd->eye_to_head_transform(di::eye::left )));
      eye_projection->set_matrix(make_projection_matrix(hmd->projection_parameters(di::eye::left ), 0.1f, 10000.0f));
    }
    {
      auto eye            = scene->add_entity();
      auto eye_metadata   = eye  ->add_component<mak::metadata>  ();
      auto eye_transform  = eye  ->add_component<mak::transform> (metadata);
      auto eye_projection = eye  ->add_component<mak::projection>();
      metadata->entity    = eye;
      metadata->name      = "HMD Right Camera";
      metadata->tags.push_back("hmd_right_camera");
      eye_transform ->set_parent(transform);
      eye_transform ->set_matrix(convert_to_glm_matrix (hmd->eye_to_head_transform(di::eye::right)));
      eye_projection->set_matrix(make_projection_matrix(hmd->projection_parameters(di::eye::right), 0.1f, 10000.0f));
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

  hmd_transform_map_                    .clear();
  controller_transform_map_             .clear();
  tracking_reference_transform_map_     .clear();
  display_redirect_transform_map_       .clear();
  generic_tracking_device_transform_map_.clear();

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
