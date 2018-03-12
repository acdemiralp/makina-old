#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <ra/load.hpp>

#include <filesystem>

#include <makina/core/logger.hpp>
#include <makina/resources/image.hpp>
#include <makina/resources/model.hpp>

template<>
inline void ra::load(const std::string& filepath, mak::model* model)
{
  if (!model)
  {
    mak::logger->error("Failed to load model: Output is nullptr.");
    return;
  }

  Assimp::Importer importer;
  const auto scene = importer.ReadFile(filepath.c_str(), aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_MakeLeftHanded | aiProcess_Triangulate);

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
    
    mesh->vertices             .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mVertices     ), reinterpret_cast<glm::vec3*>(assimp_mesh->mVertices      + assimp_mesh->mNumVertices));
    mesh->normals              .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mNormals      ), reinterpret_cast<glm::vec3*>(assimp_mesh->mNormals       + assimp_mesh->mNumVertices));
    if (assimp_mesh->HasTextureCoords(0))
      mesh->texture_coordinates.assign (reinterpret_cast<glm::vec2*>(assimp_mesh->mTextureCoords), reinterpret_cast<glm::vec2*>(assimp_mesh->mTextureCoords + assimp_mesh->mNumVertices));
    if (assimp_mesh->HasVertexColors (0))
      mesh->colors             .assign (reinterpret_cast<glm::vec4*>(assimp_mesh->mColors       ), reinterpret_cast<glm::vec4*>(assimp_mesh->mColors        + assimp_mesh->mNumVertices));
    mesh->tangents             .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mTangents     ), reinterpret_cast<glm::vec3*>(assimp_mesh->mTangents      + assimp_mesh->mNumVertices));
    mesh->bitangents           .assign (reinterpret_cast<glm::vec3*>(assimp_mesh->mBitangents   ), reinterpret_cast<glm::vec3*>(assimp_mesh->mBitangents    + assimp_mesh->mNumVertices));

    mesh->indices              .reserve(3 * assimp_mesh->mNumFaces);
    for (auto j = 0; j < assimp_mesh->mNumFaces; ++j) 
    {
      auto& face = assimp_mesh->mFaces[j];
      mesh->indices.insert(mesh->indices.end(), {face.mIndices[j], face.mIndices[j + 1], face.mIndices[j + 2]});
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

    aiString relative_image_filepath;
    if (AI_SUCCESS == assimp_material->GetTexture(aiTextureType_AMBIENT , 0, &relative_image_filepath))
      material->ambient_image  = std::make_unique<mak::image>(folderpath + "/" + relative_image_filepath.C_Str());
    if (AI_SUCCESS == assimp_material->GetTexture(aiTextureType_DIFFUSE , 0, &relative_image_filepath))
      material->diffuse_image  = std::make_unique<mak::image>(folderpath + "/" + relative_image_filepath.C_Str());
    if (AI_SUCCESS == assimp_material->GetTexture(aiTextureType_SPECULAR, 0, &relative_image_filepath))
      material->specular_image = std::make_unique<mak::image>(folderpath + "/" + relative_image_filepath.C_Str());
  }


}