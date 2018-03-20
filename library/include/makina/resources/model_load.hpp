#ifndef MAKINA_RESOURCES_MODEL_LOAD_HPP_
#define MAKINA_RESOURCES_MODEL_LOAD_HPP_

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <boost/lexical_cast.hpp>
#include <ra/load.hpp>

#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include <makina/core/logger.hpp>
#include <makina/renderer/mesh_render.hpp>
#include <makina/renderer/transform.hpp>
#include <makina/resources/image.hpp>
#include <makina/resources/model.hpp>
#include <makina/resources/phong_material.hpp>

template<>
inline void ra::load(const std::string& filepath, mak::model* model)
{
  if (!model)
  {
    mak::logger->error("Failed to load model: Output is nullptr.");
    return;
  }

  Assimp::Importer importer;
  const auto scene = importer.ReadFile(filepath.c_str(), 
    aiProcess_CalcTangentSpace    |
    aiProcess_ConvertToLeftHanded |
    aiProcess_GenSmoothNormals    | 
    aiProcess_Triangulate         );

  if (!scene)
  {
    mak::logger->error("Failed to load model: Assimp scene is nullptr.");
    return;
  }

  model->set_name(filepath);

  const auto folderpath = std::experimental::filesystem::path(filepath).parent_path().string();

  for (auto i = 0; i < scene->mNumMeshes; ++i)
  {
    model->meshes.push_back(std::make_unique<mak::mesh>());
    auto       mesh        = model->meshes.back().get();
    const auto assimp_mesh = scene->mMeshes[i];

    mesh->set_name(assimp_mesh->mName.C_Str());
    
    mesh->vertices             .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mVertices        ), reinterpret_cast<glm::vec3*>(assimp_mesh->mVertices         + assimp_mesh->mNumVertices));
    mesh->normals              .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mNormals         ), reinterpret_cast<glm::vec3*>(assimp_mesh->mNormals          + assimp_mesh->mNumVertices));
    if (assimp_mesh->HasTextureCoords(0))
      mesh->texture_coordinates.assign (reinterpret_cast<glm::vec2*>(assimp_mesh->mTextureCoords[0]), reinterpret_cast<glm::vec2*>(assimp_mesh->mTextureCoords[0] + assimp_mesh->mNumVertices));
    if (assimp_mesh->HasVertexColors (0))
      mesh->colors             .assign (reinterpret_cast<glm::vec4*>(assimp_mesh->mColors       [0]), reinterpret_cast<glm::vec4*>(assimp_mesh->mColors       [0] + assimp_mesh->mNumVertices));
    mesh->tangents             .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mTangents        ), reinterpret_cast<glm::vec3*>(assimp_mesh->mTangents         + assimp_mesh->mNumVertices));
    mesh->bitangents           .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mBitangents      ), reinterpret_cast<glm::vec3*>(assimp_mesh->mBitangents       + assimp_mesh->mNumVertices));

    mesh->indices              .reserve(3 * assimp_mesh->mNumFaces);
    for (auto j = 0; j < assimp_mesh->mNumFaces; ++j) 
    {
      auto& face = assimp_mesh->mFaces[j];
      mesh->indices.insert(mesh->indices.end(), {face.mIndices[0], face.mIndices[1], face.mIndices[2]});
    }
  }

  for (auto i = 0; i < scene->mNumMaterials; ++i)
  {
    model->materials.push_back(std::make_unique<mak::phong_material>());
    auto       material        = reinterpret_cast<mak::phong_material*>(model->materials.back().get());
    const auto assimp_material = scene->mMaterials[i];

    aiString name; 
    assimp_material->Get(AI_MATKEY_NAME, name);
    material->set_name(name.C_Str());

    aiGetMaterialColor(assimp_material, AI_MATKEY_COLOR_AMBIENT , reinterpret_cast<aiColor4D*>(&material->ambient  ));
    aiGetMaterialColor(assimp_material, AI_MATKEY_COLOR_DIFFUSE , reinterpret_cast<aiColor4D*>(&material->diffuse  ));
    aiGetMaterialColor(assimp_material, AI_MATKEY_COLOR_SPECULAR, reinterpret_cast<aiColor4D*>(&material->specular ));
    aiGetMaterialFloat(assimp_material, AI_MATKEY_SHININESS     , reinterpret_cast<float*>    (&material->shininess));

    auto load_texture = [&] (aiTextureType type, std::unique_ptr<mak::image>& image)
    {
      aiString relative_filepath;
      if (AI_SUCCESS == assimp_material->GetTexture(type, 0, &relative_filepath) && relative_filepath.length > 0)
      {
        if(relative_filepath.data[0] == '*')
        {
          auto texture = scene->mTextures[boost::lexical_cast<int>(relative_filepath.data[1])];
          image = std::make_unique<mak::image>(&texture->pcData[0].r, std::array<std::size_t, 2>{texture->mWidth, texture->mHeight});
          // TODO: Extract compressed types.
          image->to_32_bits();
        }
        else
        {
          image = std::make_unique<mak::image>(folderpath + "/" + relative_filepath.C_Str());
          image->to_32_bits();
        }
      }
    };
    load_texture(aiTextureType_AMBIENT , material->ambient_image );
    load_texture(aiTextureType_DIFFUSE , material->diffuse_image );
    load_texture(aiTextureType_SPECULAR, material->specular_image);
  }

  model->scene = std::make_unique<mak::scene>();
  std::function<void(const aiNode*, mak::transform*)> hierarchy_traverser;
  hierarchy_traverser = [&] (const aiNode* node, mak::transform* parent)
  {
    auto& matrix    = node->mTransformation;
  
    auto  entity    = model->scene->add_entity();
    auto  transform = entity->add_component<mak::transform>();
    transform->set_matrix(glm::mat4(
      {matrix[0][0], matrix[1][0], matrix[2][0], matrix[3][0]},
      {matrix[0][1], matrix[1][1], matrix[2][1], matrix[3][1]},
      {matrix[0][2], matrix[1][2], matrix[2][2], matrix[3][2]},
      {matrix[0][3], matrix[1][3], matrix[2][3], matrix[3][3]}));
    transform->set_parent(parent);
  
    if (node->mNumMeshes > 0)
    {
      auto mesh_render      = entity->add_component<mak::mesh_render>();
      auto mesh_index       = node->mMeshes[0];
      mesh_render->mesh     = model->meshes   [mesh_index].get();
      mesh_render->material = model->materials[scene->mMeshes[mesh_index]->mMaterialIndex].get();
    }
  
    for (auto i = 0; i < node->mNumChildren; ++i)
      hierarchy_traverser(node->mChildren[i], transform);
  };
  hierarchy_traverser (scene->mRootNode, nullptr);
}

#endif