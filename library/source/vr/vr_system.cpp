#include <makina/vr/vr_system.hpp>

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
glm::mat3  handedness_conversion_matrix()
{
  return glm::mat3(
    1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  0.0f, -1.0f);
}
glm::mat4  convert_to_glm_matrix       (const std::array<float, 12>& matrix)
{
  const auto translation = handedness_conversion_matrix() * glm::vec3(matrix[3], matrix[7], matrix[11]);
  const auto rotation    = handedness_conversion_matrix() * glm::mat3(glm::make_mat3x4(matrix.data())) * handedness_conversion_matrix();
  return glm::mat4(
    glm::vec4(rotation[0], 0.0f),
    glm::vec4(rotation[1], 0.0f),
    glm::vec4(rotation[2], 0.0f),
    glm::vec4(translation, 1.0f));
}

template <di::tracking_device_type type>
entity* create_tracking_device_entity(di::tracking_device<type>* tracking_device, model* model, scene* scene, entity* platform)
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

  auto       entity           = scene->add_entity();
  auto       metadata         = entity->add_component<mak::metadata>   ();
  const auto transform        = entity->add_component<mak::transform>  (metadata);
  const auto mesh_render      = entity->add_component<mak::mesh_render>();
  metadata   ->entity   = entity;
  metadata   ->name     = mesh->name();
  transform  ->set_parent(platform->component<mak::transform>());
  mesh_render->mesh     = mesh;
  mesh_render->material = material;

  if (type != di::tracking_device_type::controller)
    metadata->active = false;

  if(type == di::tracking_device_type::hmd)
  {
    const auto hmd = dynamic_cast<di::hmd*>(tracking_device);
    {
      auto       eye            = scene->add_entity();
      auto       eye_metadata   = eye  ->add_component<mak::metadata>  ();
      auto       eye_transform  = eye  ->add_component<mak::transform> (eye_metadata);
      auto       eye_projection = eye  ->add_component<mak::projection>();
      const auto rectangle      = hmd->projection_parameters(di::eye::left);
      auto       z_range        = std::array<float, 2>{0.1f, 10000.0f};
      eye_metadata  ->entity    = eye;
      eye_metadata  ->name      = "HMD Left Camera";
      eye_metadata  ->tags.push_back("hmd_left_camera");
      eye_transform ->set_parent    (transform);
      eye_transform ->set_matrix    (convert_to_glm_matrix (hmd->eye_to_head_transform(di::eye::left )));
      eye_projection->set_frustum   ({-z_range[0] * rectangle.right, -z_range[0] * rectangle.left, z_range[0] * rectangle.bottom, z_range[0] * rectangle.top}, z_range);
    }
    {
      auto       eye            = scene->add_entity();
      auto       eye_metadata   = eye  ->add_component<mak::metadata>  ();
      auto       eye_transform  = eye  ->add_component<mak::transform> (eye_metadata);
      auto       eye_projection = eye  ->add_component<mak::projection>();
      const auto rectangle      = hmd->projection_parameters(di::eye::right);
      auto       z_range        = std::array<float, 2>{0.1f, 10000.0f};
      eye_metadata  ->entity    = eye;
      eye_metadata  ->name      = "HMD Right Camera";
      eye_metadata  ->tags.push_back("hmd_right_camera");
      eye_transform ->set_parent    (transform);
      eye_transform ->set_matrix    (convert_to_glm_matrix (hmd->eye_to_head_transform(di::eye::right)));
      eye_projection->set_frustum   ({-z_range[0] * rectangle.right, -z_range[0] * rectangle.left, z_range[0] * rectangle.bottom, z_range[0] * rectangle.top}, z_range);
    }
  }
  
  return entity;
}
}

vr_system::vr_system(const di::tracking_mode tracking_mode) : di::vr_system(tracking_mode)
{

}

entity*                                                vr_system::platform                          () const
{
  return platform_;
}
const std::map<di::hmd*                    , entity*>& vr_system::hmd_entity_map                    () const
{
  return hmd_entity_map_;
}
const std::map<di::vr_controller*          , entity*>& vr_system::controller_entity_map             () const
{
  return controller_entity_map_;
}
const std::map<di::tracking_reference*     , entity*>& vr_system::tracking_reference_entity_map     () const
{
  return tracking_reference_entity_map_;
}
const std::map<di::display_redirect*       , entity*>& vr_system::display_redirect_entity_map       () const
{
  return display_redirect_entity_map_;
}
const std::map<di::generic_tracking_device*, entity*>& vr_system::generic_tracking_device_entity_map() const
{
  return generic_tracking_device_entity_map_;
}

void vr_system::prepare(                             scene* scene)
{
  auto& models = mak::registry->get<mak::model>().storage();
  auto& model  = models.emplace_back();
  
  if (platform_) 
    scene->remove_entity(platform_);

  hmd_entity_map_                    .clear();
  controller_entity_map_             .clear();
  tracking_reference_entity_map_     .clear();
  display_redirect_entity_map_       .clear();
  generic_tracking_device_entity_map_.clear();
  
  platform_            = scene->add_entity();
  auto       metadata  = platform_->add_component<mak::metadata> ();
  const auto transform = platform_->add_component<mak::transform>(metadata);
  metadata->entity     = platform_;
  metadata->name       = "platform";

  for (auto device : hmds                    ())
    hmd_entity_map_                    [device] = detail::create_tracking_device_entity(device, &model, scene, platform_);
  for (auto device : controllers             ())
    controller_entity_map_             [device] = detail::create_tracking_device_entity(device, &model, scene, platform_);
  for (auto device : tracking_references     ())
    tracking_reference_entity_map_     [device] = detail::create_tracking_device_entity(device, &model, scene, platform_);
  for (auto device : display_redirects       ())
    display_redirect_entity_map_       [device] = detail::create_tracking_device_entity(device, &model, scene, platform_);
  for (auto device : generic_tracking_devices())
    generic_tracking_device_entity_map_[device] = detail::create_tracking_device_entity(device, &model, scene, platform_);
}
void vr_system::update (frame_timer::duration delta, scene* scene)
{
  tick();
  
  for (auto& device : hmd_entity_map_)
    device.second->component<transform>()->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix));
  for (auto& device : controller_entity_map_)
    device.second->component<transform>()->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix));
  for (auto& device : tracking_reference_entity_map_)
    device.second->component<transform>()->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix));
  for (auto& device : display_redirect_entity_map_)
    device.second->component<transform>()->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix));
  for (auto& device : generic_tracking_device_entity_map_)
    device.second->component<transform>()->set_matrix(detail::convert_to_glm_matrix(device.first->pose()->absolute_matrix));
}
}
