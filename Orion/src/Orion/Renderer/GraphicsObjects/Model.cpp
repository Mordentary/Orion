#include "oripch.h"
#include "Model.h"
#include <filesystem> 
#include "Orion/Core/AdvanceCamerasFamily/CamerasController.h"
namespace Orion
{
    void Model::SetModelMatrix(const glm::mat4& mat)
    {
        m_ModelMatrix = mat;
        m_Position = glm::vec3(m_ModelMatrix[3][0], m_ModelMatrix[3][1], m_ModelMatrix[3][2]);
        RecalculateAABBInModelSpace();
    }


    void Model::RecalculateAABBInModelSpace()
    {
      
        glm::vec4 min = m_ModelMatrix * glm::vec4(m_OriginModelAABB.mMin.x, m_OriginModelAABB.mMin.y, m_OriginModelAABB.mMin.z, 1.0f);
        glm::vec4 max = m_ModelMatrix * glm::vec4(m_OriginModelAABB.mMax.x, m_OriginModelAABB.mMax.y, m_OriginModelAABB.mMax.z, 1.0f);

        m_ModelAABB.mMax.x = max.x;
        m_ModelAABB.mMin.x = min.x;

        m_ModelAABB.mMax.y = max.y;
        m_ModelAABB.mMin.y = min.y;

        m_ModelAABB.mMax.z = max.z;
        m_ModelAABB.mMin.z = min.z;

    }

    void Model::RecalculateModelMatrix() 
    {
        m_ModelMatrix = glm::translate(glm::mat4(1.0f), m_Position) 
            * glm::scale(glm::mat4(1.0f), m_Scale)

            * glm::rotate(glm::mat4(1.0f), glm::radians<float>(m_RotationAngles.z), glm::vec3(0, 0, 1))
            * glm::rotate(glm::mat4(1.0f), glm::radians<float>(m_RotationAngles.y), glm::vec3(0, 1, 0))
            * glm::rotate(glm::mat4(1.0f), glm::radians<float>(m_RotationAngles.x), glm::vec3(1, 0, 0));

                

        RecalculateAABBInModelSpace(); 
    }

    void Model::RenderAABB(const glm::vec3& mMin, const glm::vec3& mMax)
    {
        glm::vec4 min = glm::vec4(mMax.x, mMax.y, mMax.z, 1.0f);
        glm::vec4 max = glm::vec4(mMin.x, mMin.y, mMin.z, 1.0f);
        auto color = glm::vec4(1.f, 0.2f, 0.2f, 1.0f);


       // Orion::Renderer2D::AddLine(glm::vec3(0.0f), m_LastHitedPoint, color + glm::vec4(0.2f));


        Orion::Renderer2D::AddLine(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, max.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), color);


        Orion::Renderer2D::AddLine(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, min.z), color);


        Orion::Renderer2D::AddLine(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color);
        Orion::Renderer2D::AddLine(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z), color);

    }
    void Model::Render(Shared<Shader>& shader)
    {
       
        for (Shared<Mesh>& mesh : m_Meshes) 
        {
            auto [min, max] = mesh->GetAABB();
            glm::vec3 transformedMin = m_ModelMatrix * glm::vec4(min, 1.0f);
            glm::vec3 transformedMax = m_ModelMatrix * glm::vec4(max, 1.0f) ;
            if (Orion::CamerasController::GetCamera("PerspectiveCamera2")->AABBVsFrustum(transformedMin, transformedMax))
            {
                mesh->Render(shader);
                RenderAABB(transformedMin, transformedMax);
            }
        }
       // ORI_INFO("Number meshes on screen: {0}", meshesOnScreen);
        //if (shader == Orion::ShaderLibrary::Get("PhongShader") || shader == Orion::ShaderLibrary::Get("GBufferShader"))
        //      RenderModelAABB(); //works only in forward pipeline

    }

    //For retrieving scene 
	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer import;
 
		const aiScene* scene = import.ReadFile(path, 

            aiProcess_Triangulate  | 
            aiProcess_SortByPType |
            aiProcess_RemoveRedundantMaterials |
            aiProcess_PreTransformVertices |
            aiProcess_OptimizeMeshes |
            aiProcess_OptimizeGraph |
            aiProcess_JoinIdenticalVertices |
            aiProcess_GenBoundingBoxes | 
            aiProcess_ImproveCacheLocality |
           // aiProcess_FlipUVs | 
            aiProcess_GenUVCoords |
            aiProcess_CalcTangentSpace);
      

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
    			ORI_ASSERT(false, (import.GetErrorString()));
			return;
		}

        m_Path = path;
		m_Directory = path.substr(0, path.find_last_of('/'));

        FindGreastestAABBAndCoord(scene->mRootNode, scene);

        float scale = 1.0f / m_MaxDivider;
        m_OriginModelAABB.mMin = m_OriginModelAABB.mMin * scale;
        m_OriginModelAABB.mMax = m_OriginModelAABB.mMax * scale;
        m_Meshes.reserve(scene->mNumMeshes);
		ProcessNode(scene->mRootNode, scene);

	}

    void Model::FindGreastestAABBAndCoord(aiNode* node, const aiScene* scene)
    {

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            aiVector3D diff = (mesh->mAABB.mMax - mesh->mAABB.mMin);
            float scaleF = glm::max(glm::max(diff.x, diff.y), diff.z);

            if (diff.x >= (m_OriginModelAABB.mMax - m_OriginModelAABB.mMin).x &&
                diff.y >= (m_OriginModelAABB.mMax - m_OriginModelAABB.mMin).y &&
                diff.z >= (m_OriginModelAABB.mMax - m_OriginModelAABB.mMin).z) 
            {
                m_OriginModelAABB = mesh->mAABB;
            }

          //  m_ModelAABB = mesh->mAABB;
            if (scaleF > m_MaxDivider) m_MaxDivider = scaleF;
        } 
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            FindGreastestAABBAndCoord(node->mChildren[i], scene);
        }


    }

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
            
            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
                m_Meshes.push_back(ProcessMesh(mesh, scene));

                ++m_MeshIndex;
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                ProcessNode(node->mChildren[i], scene);
            }
           
	}

    Shared<Mesh> Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill 
        
        std::vector<MeshVertex> vertices;
        vertices.reserve(mesh->mNumVertices);

        std::vector<uint32_t> indices;
         indices.reserve(mesh->mNumFaces * 3);
                 
       
        float scale = 1.0f / m_MaxDivider;  
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        glm::vec3 colorLinear = glm::pow(glm::vec3(color.r, color.g, color.b),glm::vec3(2.2f));

        color = aiColor3D(colorLinear.r, colorLinear.g, colorLinear.b);

        float shin = 0.f;
        material->Get(AI_MATKEY_SHININESS, shin);
     
        MeshVertex vertex{};
        glm::vec3 vector{};
        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            //we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x * scale;
            vector.y = mesh->mVertices[i].y * scale;
            vector.z = mesh->mVertices[i].z * scale;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            else 
            {
                vertex.Normal = glm::vec3(0.0f);
            }
            // texture coordinates
            if (mesh->HasTextureCoords(0)) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TextureCoord = vec;

            }
         
            if (mesh->HasTangentsAndBitangents())
            {
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }else
            {
                vertex.Tangent = glm::vec3(0.0f);
                vertex.Bitangent = glm::vec3(0.0f);

            }
            if (mesh->HasVertexColors(0))
            {
                vertex.Color.r = mesh->mColors[0][i].r;
                vertex.Color.g = mesh->mColors[0][i].g;
                vertex.Color.b = mesh->mColors[0][i].b;
                vertex.Color.a = mesh->mColors[0][i].a;
               
            }
            else vertex.Color = glm::vec4(color.r, color.g, color.b, 1.0f);



            vertices.emplace_back(std::move(vertex));
        }

        // now walk through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (uint32_t i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
           
            for (uint32_t j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        Material mat{};

     
        // 1. diffuse maps
        Shared<Texture2D> diffuseMap = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "DiffuseMap");
        // 2. specular maps
        Shared<Texture2D> specularMap = LoadMaterialTextures(material, aiTextureType_SPECULAR, "SpecularMap");
        // 3. normal maps
        Shared<Texture2D> normalMap = LoadMaterialTextures(material, aiTextureType_HEIGHT, "NormalMap");
        // 4. shineness maps
        Shared<Texture2D> shininessMap = LoadMaterialTextures(material, aiTextureType_SHININESS, "HeightMap");
        // 4. base color maps
        Shared<Texture2D> colorMap = LoadMaterialTextures(material, aiTextureType_BASE_COLOR, "ColorMap");
       



        if (diffuseMap) 
            mat.diffuseMap = diffuseMap;
        if (normalMap) 
            mat.normalMap = normalMap;
       if(specularMap)
            mat.specularMap = specularMap;
        
       if (shin)
           mat.shininess = shin;
       else
           mat.shininess = 32.f;
        
       mesh->mAABB.mMax *= scale;
       mesh->mAABB.mMin *= scale;

        // return a mesh object created from the extracted mesh data
            
       return CreateShared<Mesh>(vertices, indices, mat, glm::vec3{ mesh->mAABB.mMin.x,mesh->mAABB.mMin.y, mesh->mAABB.mMin.z}, glm::vec3{mesh->mAABB.mMax.x, mesh->mAABB.mMax.y, mesh->mAABB.mMax.z});
            
    }

    Shared<Texture2D> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
            if (mat->GetTextureCount(type) != 0) 
            {
                aiString str;
                mat->GetTexture(type, 0, &str);

                if (type == aiTextureType_DIFFUSE)
                {
                    return Texture2D::Create(this->m_Directory + '/' + str.C_Str(), { true, true });
                }
                else
                {
                    return Texture2D::Create(this->m_Directory + '/' + str.C_Str());

                }
            }

            // If textures didn't link to the model then manually search for them.

            std::string path = this->m_Directory + '/' + "textures" + '/' + std::to_string(m_MeshIndex) + "_";

            int32_t bitField = 0;

            if (type == aiTextureType_DIFFUSE)
            {
                bitField |= (std::filesystem::exists(path + "albedo.jpg") * FILE_FORMATS::JPG) | (std::filesystem::exists(path + "albedo.jpeg") * FILE_FORMATS::JPEG) | ((int32_t)std::filesystem::exists(path + "albedo.png") * FILE_FORMATS::PNG) ;

                if (bitField & FILE_FORMATS::JPG)
                    return Texture2D::Create(path + "albedo.jpg", { true, true });

                if (bitField & FILE_FORMATS::JPEG)
                    return Texture2D::Create(path + "albedo.jpeg", { true, true });

                if (bitField & FILE_FORMATS::PNG)
                    return  Texture2D::Create(path + "albedo.png", { true, true });

            }

            if (type == aiTextureType_HEIGHT && std::filesystem::exists(path + "normal.png"))
            {
                return  Texture2D::Create(path + "normal.png");
            }


            if (type == aiTextureType_SPECULAR)
            {
                bitField |= (std::filesystem::exists(path + "specular.jpg") * FILE_FORMATS::JPG) | (std::filesystem::exists(path + "specular.jpeg") * FILE_FORMATS::JPEG);

                if (bitField & FILE_FORMATS::JPG)
                    return Texture2D::Create(path + "specular.jpg");

                if (bitField & FILE_FORMATS::JPEG)
                    return Texture2D::Create(path + "specular.jpeg");
            }


            
            return nullptr;
	}

   
    bool Model::IsIntersect(const CameraRay& ray)
    {
        

        auto AABB = m_ModelAABB;

        double tx1 = (AABB.mMin.x - ray.GetOrigin().x) / (ray.GetDirection().x * ray.GetLength());
        double tx2 = (AABB.mMax.x - ray.GetOrigin().x) / (ray.GetDirection().x * ray.GetLength());

        double tmin = std::min(tx1, tx2);
        double tmax = std::max(tx1, tx2);

        double ty1 = (AABB.mMin.y - ray.GetOrigin().y) / (ray.GetDirection().y * ray.GetLength());
        double ty2 = (AABB.mMax.y - ray.GetOrigin().y) / (ray.GetDirection().y * ray.GetLength());

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        double tz1 = (AABB.mMin.z - ray.GetOrigin().z) / (ray.GetDirection().z * ray.GetLength());
        double tz2 = (AABB.mMax.z - ray.GetOrigin().z) / (ray.GetDirection().z * ray.GetLength());

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));

       

       
        if (tmax > std::max(tmin, 0.0))
        {
            m_LastHitedPoint = ray.GetOrigin() + glm::vec3(tmin * (ray.GetDirection().x * ray.GetLength()), tmin * (ray.GetDirection().y * ray.GetLength()), tmin * (ray.GetDirection().z * ray.GetLength()));
            return true;
        }

        return false;


    }

    void Model::DeduceModelName() 
    {
        std::string name = "";

        // assets/shaders/Texture.glsl
        auto lastSlash = m_Path.find_last_of("/\\");
        lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
        auto lastDot = m_Path.rfind('.');
        auto count = lastDot == std::string::npos ? m_Path.size() - lastSlash : lastDot - lastSlash;
        m_Name = m_Path.substr(lastSlash, count);

    }

 

}