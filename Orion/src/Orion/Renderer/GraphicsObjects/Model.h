#pragma once

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Orion 
{
    class Model
    {
    public:
        Model(const std::string& path)
        {
            LoadModel(path);
        }
        void Render(Shared<Shader>& shader);
        void BindAllTexture() //TODO: REMOVE THIS SHIT
        {

            uint32_t index = 1;
            for (auto& mesh : m_Meshes)
            {
                if (mesh->GetMaterial().diffuseMap) {
                    mesh->GetMaterial().diffuseMap->Bind(index);

                }
                if (mesh->GetMaterial().specularMap) {
                    mesh->GetMaterial().specularMap->Bind(++index);
                }
                index++;
              
            }

        }
        Shared<Mesh>* GetMeshData() { return m_Meshes.data(); }
    private:
        // model data
        std::vector<Shared<Mesh>> m_Meshes;
        std::string m_Directory;
        float m_MaxScale = 0.f;
        void LoadModel(const std::string& path);
        void FindGreastestMinMax(aiNode* rootNode, const aiScene* scene);
        void ProcessNode(aiNode* node, const aiScene* scene);
       Shared<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Shared<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type,
            std::string typeName);
    };
}
