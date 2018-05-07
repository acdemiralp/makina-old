#ifndef MAKINA_RESOURCES_MODEL_LOAD_HPP_
#define MAKINA_RESOURCES_MODEL_LOAD_HPP_

#include <filesystem>
#include <fstream>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <boost/lexical_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <ra/load.hpp>

#include <makina/core/logger.hpp>
#include <makina/renderer/animator.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/animation_clip.hpp>
#include <makina/resources/image.hpp>
#include <makina/resources/model.hpp>
#include <makina/resources/phong_material.hpp>
#include <makina/resources/physically_based_material.hpp>

template<>
inline void ra::load(const mak::model::description& description, mak::model* model)
{
  if (!model)
  {
    mak::logger->error("Failed to load model: Output is nullptr.");
    return;
  }

  Assimp::Importer importer;
  const auto scene = importer.ReadFile(description.filepath.c_str(), 
    aiProcess_CalcTangentSpace |
    aiProcess_MakeLeftHanded   |
    aiProcess_FlipWindingOrder |
    aiProcess_GenSmoothNormals | 
    aiProcess_Triangulate      );

  if (!scene)
  {
    mak::logger->error("Failed to load model: Assimp scene is nullptr.");
    return;
  }

  model->set_name(description.filepath);

  const auto filename   = std::experimental::filesystem::path(description.filepath).replace_extension("").filename().string();
  const auto folderpath = std::experimental::filesystem::path(description.filepath).parent_path().string();

  std::vector<std::pair<std::string, mak::bone>> bones;
  for (auto i = 0; i < scene->mNumMeshes; ++i)
  {
    model->meshes.push_back(std::make_unique<mak::mesh>());
    auto       mesh        = model->meshes.back().get();
    const auto assimp_mesh = scene->mMeshes[i];

    mesh->set_name(assimp_mesh->mName.C_Str());
    
    mesh->vertices             .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mVertices        ), reinterpret_cast<glm::vec3*>(assimp_mesh->mVertices         + assimp_mesh->mNumVertices));
    mesh->normals              .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mNormals         ), reinterpret_cast<glm::vec3*>(assimp_mesh->mNormals          + assimp_mesh->mNumVertices));
    if (assimp_mesh->HasTextureCoords(0))
      mesh->texture_coordinates.assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mTextureCoords[0]), reinterpret_cast<glm::vec3*>(assimp_mesh->mTextureCoords[0] + assimp_mesh->mNumVertices));
    if (assimp_mesh->HasVertexColors (0))
      mesh->colors             .assign (reinterpret_cast<glm::vec4*>(assimp_mesh->mColors       [0]), reinterpret_cast<glm::vec4*>(assimp_mesh->mColors       [0] + assimp_mesh->mNumVertices));
    if (assimp_mesh->HasTangentsAndBitangents())
    {
      mesh->tangents           .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mTangents        ), reinterpret_cast<glm::vec3*>(assimp_mesh->mTangents         + assimp_mesh->mNumVertices));
      mesh->bitangents         .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mBitangents      ), reinterpret_cast<glm::vec3*>(assimp_mesh->mBitangents       + assimp_mesh->mNumVertices));
    }

    mesh->indices              .reserve(3 * assimp_mesh->mNumFaces);
    for (auto j = 0; j < assimp_mesh->mNumFaces; ++j) 
    {
      auto& face = assimp_mesh->mFaces[j];
      mesh->indices.insert(mesh->indices.end(), {face.mIndices[0], face.mIndices[1], face.mIndices[2]});
    }

    if (assimp_mesh->HasBones()) 
    {
      mesh->bone_ids       .resize(assimp_mesh->mNumVertices);
      mesh->bone_weights   .resize(assimp_mesh->mNumVertices);
      std::vector<unsigned> counts(assimp_mesh->mNumVertices);

      for (std::uint32_t j = 0; j < assimp_mesh->mNumBones; ++j) 
      {
        const auto& assimp_bone = assimp_mesh->mBones[j];

        bones.push_back({assimp_bone->mName.C_Str(), {j, glm::transpose(glm::make_mat4(&assimp_bone->mOffsetMatrix[0][0]))}});
        
        for (auto k = 0; k < assimp_bone->mNumWeights; ++k)
        {
          auto* weight = &assimp_bone->mWeights[k];
          mesh->bone_ids    [weight->mVertexId][counts[weight->mVertexId]] = j;
          mesh->bone_weights[weight->mVertexId][counts[weight->mVertexId]] = weight->mWeight;
          counts[weight->mVertexId]++;
        }
      }
    }
  }

  for (auto i = 0; i < scene->mNumMaterials; ++i)
  {
    const auto assimp_material = scene->mMaterials[i];
      
    auto load_texture = [&] (aiTextureType type, std::unique_ptr<mak::image>& image)
    {
      aiString relative_filepath;
      if (AI_SUCCESS == assimp_material->GetTexture(type, 0, &relative_filepath) && relative_filepath.length > 0)
      {
        if(relative_filepath.data[0] == '*')
        {
          auto texture = scene->mTextures[boost::lexical_cast<int>(relative_filepath.data[1])];
          if  (texture->mHeight == 0) // Compressed format.
          {
            auto filepath = folderpath + "/" + filename + "_" + relative_filepath.data[1] + "." + std::string(texture->achFormatHint);
            std::ofstream(filepath, std::ios::binary).write(reinterpret_cast<char*>(texture->pcData), texture->mWidth);
            image = std::make_unique<mak::image>(filepath);
            image->to_32_bits();
          }
          else
          {
            image = std::make_unique<mak::image>(&texture->pcData[0].r, std::array<std::size_t, 2>{texture->mWidth, texture->mHeight}, fi::type::bitmap, 32);
            image->to_32_bits();
          }
        }
        else
        {
          image = std::make_unique<mak::image>(folderpath + "/" + relative_filepath.C_Str());
          image->to_32_bits();
        }
      }
    };

    if (description.pbr_materials)
    {
      model->materials.push_back(std::make_unique <mak::physically_based_material>());
      auto material = reinterpret_cast<mak::physically_based_material*>(model->materials.back().get());

      aiString name; 
      assimp_material->Get(AI_MATKEY_NAME, name);
      material->set_name(name.C_Str());

      aiColor3D diffuse, specular;
      float     shininess;
      assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE , diffuse  );
      assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, specular );
      assimp_material->Get(AI_MATKEY_SHININESS     , shininess);
      material->albedo      = {diffuse[0], diffuse[1], diffuse[2]}; 
      material->metallicity = specular[0];                          
      material->roughness   = sqrt(2.0f / (shininess + 2.0f)); // Beckmann distribution.

      load_texture(aiTextureType_DIFFUSE  , material->albedo_image           );
      load_texture(aiTextureType_SPECULAR , material->metallicity_image      );
      load_texture(aiTextureType_SHININESS, material->roughness_image        );
      load_texture(aiTextureType_NORMALS  , material->normal_image           );
      load_texture(aiTextureType_LIGHTMAP , material->ambient_occlusion_image);
    }
    else
    {
      model->materials.push_back(std::make_unique<mak::phong_material>());
      auto material = reinterpret_cast<mak::phong_material*>(model->materials.back().get());
      
      aiString name; 
      assimp_material->Get(AI_MATKEY_NAME, name);
      material->set_name(name.C_Str());

      assimp_material->Get(AI_MATKEY_COLOR_AMBIENT , reinterpret_cast<aiColor3D&>(material->ambient  ));
      assimp_material->Get(AI_MATKEY_COLOR_DIFFUSE , reinterpret_cast<aiColor3D&>(material->diffuse  ));
      assimp_material->Get(AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor3D&>(material->specular ));
      assimp_material->Get(AI_MATKEY_SHININESS     , reinterpret_cast<float&>    (material->shininess));

      load_texture(aiTextureType_AMBIENT , material->ambient_image );
      load_texture(aiTextureType_DIFFUSE , material->diffuse_image );
      load_texture(aiTextureType_SPECULAR, material->specular_image);
    }
  }

  for (auto i = 0; i < scene->mNumAnimations; ++i)
  {
    model->animation_clips.push_back(std::make_unique<mak::animation_clip>());
    auto       animation_clip        = model->animation_clips.back().get();
    const auto assimp_animation_clip = scene->mAnimations[i];
    animation_clip->set_name(assimp_animation_clip->mName.C_Str());
    animation_clip->framerate = assimp_animation_clip->mTicksPerSecond ;
    animation_clip->length    = std::chrono::duration<float, std::ratio<1>>(assimp_animation_clip->mDuration / assimp_animation_clip->mTicksPerSecond);
    animation_clip->wrap_mode = mak::animation_clip::wrap_mode::loop   ;

    for (auto j = 0; j < assimp_animation_clip->mNumChannels; ++j) 
    {
      auto position_curve = mak::animation_curve<glm::vec3>();
      auto scaling_curve  = mak::animation_curve<glm::vec3>();
      auto rotation_curve = mak::animation_curve<glm::quat>();

      const auto assimp_curve = assimp_animation_clip->mChannels[j];
      for (auto k = 0; k < assimp_curve->mNumPositionKeys; k++)
        position_curve.keyframes.push_back({
          mak::frame_timer::duration(std::chrono::duration<float, std::ratio<1>>(assimp_curve->mPositionKeys[k].mTime)), 
          glm::vec3(assimp_curve->mPositionKeys[k].mValue.x, assimp_curve->mPositionKeys[k].mValue.y, assimp_curve->mPositionKeys[k].mValue.z)});
      for (auto k = 0; k < assimp_curve->mNumRotationKeys; k++)
        rotation_curve.keyframes.push_back({
          mak::frame_timer::duration(std::chrono::duration<float, std::ratio<1>>(assimp_curve->mRotationKeys[k].mTime)), 
          glm::quat(assimp_curve->mRotationKeys[k].mValue.w, assimp_curve->mRotationKeys[k].mValue.x, assimp_curve->mRotationKeys[k].mValue.y, assimp_curve->mRotationKeys[k].mValue.z)});
      for (auto k = 0; k < assimp_curve->mNumScalingKeys; k++)
        scaling_curve .keyframes.push_back({
          mak::frame_timer::duration(std::chrono::duration<float, std::ratio<1>>(assimp_curve->mScalingKeys [k].mTime)), 
          glm::vec3(assimp_curve->mScalingKeys [k].mValue.x, assimp_curve->mScalingKeys [k].mValue.y, assimp_curve->mScalingKeys [k].mValue.z)});

      animation_clip->translation_curves[assimp_curve->mNodeName.C_Str()] = position_curve;
      animation_clip->rotation_curves   [assimp_curve->mNodeName.C_Str()] = rotation_curve;
      animation_clip->scaling_curves    [assimp_curve->mNodeName.C_Str()] = scaling_curve ;
    }
  }

  mak::transform* root_transform = nullptr;
  model->scene = std::make_unique<mak::scene>();
  std::function<void(const aiNode*, mak::transform*)> hierarchy_traverser;
  hierarchy_traverser = [&] (const aiNode* node, mak::transform* parent)
  {
    auto entity      = model->scene->add_entity();
    auto metadata    = entity->add_component<mak::metadata> ();
    auto transform   = entity->add_component<mak::transform>(metadata);
    metadata->entity = entity;
    metadata->name   = node->mName.C_Str();
    transform->set_matrix(glm::transpose(glm::make_mat4(&node->mTransformation[0][0])));
    transform->set_parent(parent, false);
  
    if (node->mNumMeshes > 0)
    {
      auto mesh_render        = entity->add_component<mak::mesh_render>  ();
      auto mesh_collider      = entity->add_component<mak::mesh_collider>();
      auto mesh_index         = node ->mMeshes  [0];
      mesh_render  ->mesh     = model->meshes   [mesh_index].get();
      mesh_render  ->material = model->materials[scene->mMeshes[mesh_index]->mMaterialIndex].get();
      mesh_collider->set_mesh ( model->meshes   [mesh_index].get());

      if (model->animation_clips.size() > 0)
        entity->add_component<mak::animator>()->clip = model->animation_clips[0].get();
    }
    else // Meshes and bones cannot be on the same entity (as in Unity).
    {
      auto _bone = std::find_if(bones.begin(), bones.end(), [&metadata] (const std::pair<std::string, mak::bone>& iteratee) { return iteratee.first == metadata->name; });
      if  (_bone != bones.end())
      {
        auto bone           = entity->add_component<mak::bone>();
        bone->index         = _bone->second.index;
        bone->offset_matrix = _bone->second.offset_matrix;
      }
    }
   
    if (!root_transform)
    {
      root_transform = transform;
      root_transform->metadata()->tags.push_back("root_bone");
    }
  
    for (auto i = 0; i < node->mNumChildren; ++i)
      hierarchy_traverser(node->mChildren[i], transform);
  };
  hierarchy_traverser(scene->mRootNode, nullptr);
  auto animator_entities = model->scene->entities<mak::animator>();
  for (auto entity : animator_entities)
    entity->component<mak::animator>()->root_transform = root_transform;
}

template<>
inline void ra::load(const std::string& filepath, mak::model* model)
{
  ra::load<mak::model::description, mak::model>({filepath}, model);
}

#endif