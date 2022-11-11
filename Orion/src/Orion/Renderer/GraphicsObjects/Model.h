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
             DeduceModelName();
        }
        void Render(Shared<Shader>& shader);
        bool IsIntersect(const CameraRay& ray);

        void BindAllTexture(); //TODO: REMOVE THIS SHIT
       
        Shared<Mesh>* GetMeshData() { return m_Meshes.data(); }
        const glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
        void SetModelMatrix(const glm::mat4& mat) { m_ModelMatrix = mat; }
        std::string GetModelName() { return m_Name; }

    private:
        // model data
        aiAABB m_ModelAABB;

        std::string m_Directory;
        std::string m_Name;

        glm::mat4 m_ModelMatrix{1.0f};
        std::vector<Shared<Mesh>> m_Meshes;
        float m_MaxCoord = 0.f;
      
    private:
        std::vector<Shared<Texture2D>> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        Shared<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        void LoadModel(const std::string& path);
        void FindGreastestCoord(aiNode* rootNode, const aiScene* scene);
        void ProcessNode(aiNode* node, const aiScene* scene);
        bool IsFileExists(const std::string& name);
        void DeduceModelName();
    };
}
