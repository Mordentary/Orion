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
             RecalculateAABBInModelSpace();
        }
        void Render(Shared<Shader>& shader);
        bool IsIntersect(const CameraRay& ray);


        inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateModelMatrix();}
        inline void SetScale(const glm::vec3& scale) { m_Scale = scale; RecalculateModelMatrix();}
        inline void SetRotationAngles(const glm::vec3& rotation) { m_RotationAngles = rotation; RecalculateModelMatrix();}
        inline float GetMaxModelDivider() { return m_MaxDivider; }


        inline Shared<Mesh>* GetMeshData() { return m_Meshes.data(); }
        inline std::vector<Shared<Mesh>>& GetMeshes() { return m_Meshes; }

        inline const glm::mat4& GetModelMatrix() { return m_ModelMatrix; }

        inline const glm::vec3& GetPosition() { return m_Position; }
        inline const glm::vec3& GetScale() { return m_Scale; }
        inline const glm::vec3& GetRotationAngles() { return m_RotationAngles; }


        inline const glm::vec3& GetLastHitedPoint() { return m_LastHitedPoint; }


        //inline  glm::vec3 GetPosition() { return m_Position; }
        //inline  glm::vec3 GetScale() { return m_Scale; }
        //inline  glm::vec3 GetRotationAngles() { return m_Rotation; }


        void SetModelMatrix(const glm::mat4& mat);
        const std::string& GetModelName() { return m_Name; }

    private:
        

        std::string m_Directory;
        std::string m_Name;
        std::string m_Path;


        glm::mat4 m_ModelMatrix{1.0f};


        glm::vec3 m_Position{0.0f};
        glm::vec3 m_Scale{ 1.0f };
        glm::vec3 m_RotationAngles{ 0.0f };

        glm::vec3 m_LastHitedPoint{};


        aiAABB m_OriginModelAABB;
        aiAABB m_ModelAABB;

        float m_MaxDivider = 0.f;

        std::vector<Shared<Mesh>> m_Meshes;

        uint32_t m_MeshIndex = 0;


        enum FILE_FORMATS 
        {
            PNG = BIT(0),
            JPEG = BIT(1),
            JPG = BIT(2)

        };

    private:
        void RenderModelAABB();
        Shared<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        Shared<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
        void RecalculateModelMatrix();
        void LoadModel(const std::string& path);
        void FindGreastestCoord(aiNode* rootNode, const aiScene* scene);
        void ProcessNode(aiNode* node, const aiScene* scene);
        void RecalculateAABBInModelSpace();
        void DeduceModelName();
    };
}
