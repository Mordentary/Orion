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
        void SetModelMatrix(const glm::mat4& mat) 
        {
            m_ModelMatrix = mat; 
            float scale = 1.0f / m_MaxCoord;

            glm::vec3 mAScaled = glm::vec3(m_ModelAABB.mMin.x, m_ModelAABB.mMin.y, m_ModelAABB.mMin.z) * scale;
            glm::vec3 mIScaled = glm::vec3(m_ModelAABB.mMax.x, m_ModelAABB.mMax.y, m_ModelAABB.mMax.z) * scale;

            glm::vec4 min = glm::vec4(mIScaled.x, mIScaled.y, mIScaled.z, 1.0f);
            glm::vec4 max = glm::vec4(mAScaled.x, mAScaled.y, mAScaled.z, 1.0f);
            ;

            min = m_ModelMatrix * min;
            max = m_ModelMatrix * max;

            m_ModelAABB.mMin.x = min.x;
            m_ModelAABB.mMax.x = max.x;

            m_ModelAABB.mMin.y = min.y;
            m_ModelAABB.mMax.y = max.y;

            m_ModelAABB.mMin.z = min.z;
            m_ModelAABB.mMax.z = max.z;

        
        }
        std::string GetModelName() { return m_Name; }

    private:
        // model data
        aiAABB m_ModelAABB;

        std::string m_Directory;
        std::string m_Path;

        std::string m_Name;

        glm::mat4 m_ModelMatrix{1.0f};
        float m_MaxCoord = 0.f;
        std::vector<Shared<Mesh>> m_Meshes;
      
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
