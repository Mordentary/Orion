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
        void Render();
        void BindAllTexture()
        {


            for (auto& mesh : m_Meshes)
            {
                if (mesh->GetMaterial().diffuseMap) {
                    mesh->GetMaterial().diffuseMap->Bind(3);
                }
            }

        }
        Shared<Mesh>* GetMeshData() { return m_Meshes.data(); }
    private:
        // model data
        std::vector<Shared<Mesh>> m_Meshes;
        std::string m_Directory;

        void LoadModel(const std::string& path);
        void processNode(aiNode* node, const aiScene* scene);
       Shared<Mesh> processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Shared<Texture2D>> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
            std::string typeName);
    };
}
