#pragma once

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace Orion 
{

    enum SHADING_MODELS
    {
        PBR         = 0,
        BlinnPhong  = 1,
        Cell        = 2,
        Flat        = 3

    };

    class Model
    {

    public:
        struct CustomMaterialValues
        {

            Shared<Texture2D> CustomRougnessMap = nullptr;
            Shared<Texture2D> CustomMetallicMap = nullptr;
            Shared<Texture2D> CustomEmissionMap = nullptr;

            float Roughness = 0.1f;
            float Metallic = 0.5f;
            glm::vec3 Emission{};

        };
    public:
        Model(const std::string& path, int32_t modelShadingID) : m_ShadingModel(modelShadingID), m_Path(path), m_Directory(path.substr(0, path.find_last_of('/')))
        {
             LoadModel(path);
             DeduceModelName();
             RecalculateAABBInModelSpace();

             m_CustomMaterial.CustomRougnessMap = Texture2D::Create(1, 1);
             m_CustomMaterial.CustomMetallicMap = Texture2D::Create(1, 1);
             m_CustomMaterial.CustomEmissionMap= Texture2D::Create(1, 1);

        }

       
        void Render(Shared<Shader>& shader, Shared<Orion::LightSource> dirLight = nullptr, bool doNotCull = false);
        bool IsIntersect(const Ray& ray);

        void ApplyCustomMaterialValues();
        void ApplyDefaultMaterialValues();



        inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateModelMatrix();}
        inline void SetScale(const glm::vec3& scale) { m_Scale = scale; RecalculateModelMatrix();}
        inline void SetRotationAngles(const glm::vec3& rotation) { m_RotationAngles = rotation; RecalculateModelMatrix();}


        inline Shared<Mesh>* GetMeshData() { return m_Meshes.data(); }
        inline std::vector<Shared<Mesh>>& GetMeshes() { return m_Meshes; }

        inline const glm::mat4& GetModelMatrix() { return m_ModelMatrix; }
        inline const glm::vec3& GetPosition() { return m_Position; }
        inline const glm::vec3& GetScale() { return m_Scale; }
        inline const glm::vec3& GetRotationAngles() { return m_RotationAngles; }

        inline float GetMaxModelDivider() { return m_CoordNormalizationFactor; }

        inline const glm::vec3& GetLastHitedPoint() { return m_LastHitedPoint; }

        inline const CustomMaterialValues& GetCustomMaterialValues() { return m_CustomMaterial; }


        const std::string& GetModelName() { return m_Name; }
        bool IsPBRModel() { return m_ShadingModel == SHADING_MODELS::PBR; }


        void SetCustomMaterialValues(float roughness, float metallic, const glm::vec3 emmision);

        void SetModelMatrix(const glm::mat4& mat);

    private:
        void RenderAABB(const glm::vec3& mMin, const glm::vec3& mMax);

        void SetupNonPBRMaterials(const aiScene* scene);
        void SetupPBRMaterials(const aiScene* scene);

        void SeparateRoughnessAndMetallicTextures();

        Shared<Texture2D> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
        Shared<Texture2D> LoadPBRMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

        Material SetupPBRMaterial(aiMaterial* mat);
        Material SetupNonPBRMaterial(aiMaterial* mat);

        Shared<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);

        void ConcatenateMeshByMaterial();

        void RecalculateModelMatrix();
        void LoadModel(const std::string& path);
        void FindGreastestAABBAndCoord(aiNode* rootNode, const aiScene* scene);
        void ProcessNode(aiNode* node, const aiScene* scene);
        
        void BindMaterial(Shared<Shader>& shader, const Material& material);


        void RecalculateAABBInModelSpace();
        void DeduceModelName();
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

        std::vector<Material> m_Materials;

        std::vector<Shared<Mesh>> m_Meshes;

        CustomMaterialValues m_CustomMaterial;

        float m_CoordNormalizationFactor = 0.f;
        uint32_t m_MeshIndex = 0;
        uint32_t m_ShadingModel;


        enum FILE_FORMATS 
        {
            PNG = BIT(0),
            JPEG = BIT(1),
            JPG = BIT(2)

        };

    };
}
