#include "oripch.h"
#include "Model.h"



namespace Orion
{
    inline bool IsFileExists(const std::string& name);

	void Model::Render(Shared<Shader>& shader)
	{
	
        

        auto color = glm::vec4(1.f, 0.2f, 0.2f, 1.0f);
     //   Orion::Renderer2D::DrawLine(glm::vec4(m_ModelAABB.mMin.x, m_ModelAABB.mMin.y, m_ModelAABB.mMin.z, 1.0f), glm::vec4(m_ModelAABB.mMax.x, m_ModelAABB.mMax.y, m_ModelAABB.mMax.z,1.0f), glm::vec4(1.f,0.2f,0.2f,1.0f));
      


        glm::vec4 min =  glm::vec4(m_ModelAABB.mMax.x, m_ModelAABB.mMax.y, m_ModelAABB.mMax.z, 1.0f);
        glm::vec4 max = glm::vec4(m_ModelAABB.mMin.x, m_ModelAABB.mMin.y, m_ModelAABB.mMin.z, 1.0f);
;

   
        


        Orion::Renderer2D::DrawLine(glm::vec3(min.x, min.y, min.z), glm::vec3(max.x, min.y, min.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, min.y, max.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(max.x, min.y, max.z), glm::vec3(min.x, min.y, max.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, min.y, min.z), color);


        Orion::Renderer2D::DrawLine(glm::vec3(min.x, max.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(max.x, max.y, min.z), glm::vec3(max.x, max.y, max.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(max.x, max.y, max.z), glm::vec3(min.x, max.y, max.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(min.x, max.y, max.z), glm::vec3(min.x, max.y, min.z), color);


        Orion::Renderer2D::DrawLine(glm::vec3(min.x, min.y, min.z), glm::vec3(min.x, max.y, min.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(max.x, min.y, min.z), glm::vec3(max.x, max.y, min.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(max.x, min.y, max.z), glm::vec3(max.x, max.y, max.z), color);
        Orion::Renderer2D::DrawLine(glm::vec3(min.x, min.y, max.z), glm::vec3(min.x, max.y, max.z), color);

        for (auto& mesh : m_Meshes)
            mesh->Render(shader);

            //glm::vec3(min.x, min.y, min.z)// bottom left
            //glm::vec3(max.x, min.y, min.z), //BOTTOM_RIGHT
            //glm::vec3(max.x, min.y, max.z), //BOTTOM_RIGHT_INNER
            //glm::vec3(min.x, min.y, max.z), //BOTTOM_LEFT_INNER
            //glm::vec3(min.x, max.y, max.z),//TOP_LEFT_INNER
            //glm::vec3(min.x, max.y, min.z),//TOP_LEFT
            //glm::vec3(max.x, max.y, min.z), //TOP_RIGHT
            ////
            //glm::vec3(max.x, max.y, max.z) //TOP_RIGHT_INNER

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
            aiProcess_GenBoundingBoxes | 
            aiProcess_GenSmoothNormals | 
            aiProcess_FixInfacingNormals |
            aiProcess_FindInvalidData |
            aiProcess_ValidateDataStructure | 
           
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

        FindGreastestCoord(scene->mRootNode, scene);

		ProcessNode(scene->mRootNode, scene);

	}

    void Model::FindGreastestCoord(aiNode* node, const aiScene* scene)
    {

        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

            aiVector3D diff = (mesh->mAABB.mMax - mesh->mAABB.mMin);
            float scaleF = glm::max(glm::max(diff.x, diff.y), diff.z);

            if (diff.x >= (m_ModelAABB.mMax - m_ModelAABB.mMin).x &&
                diff.y >= (m_ModelAABB.mMax - m_ModelAABB.mMin).y &&
                diff.z >= (m_ModelAABB.mMax - m_ModelAABB.mMin).z) 
            {
                m_ModelAABB = mesh->mAABB;
            }

          //  m_ModelAABB = mesh->mAABB;
            if (scaleF > m_MaxCoord) m_MaxCoord = scaleF;
        } 
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            FindGreastestCoord(node->mChildren[i], scene);
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

         
        std::vector<Shared<Texture2D>> textures;
        // walk through each of the mesh's vertices
        
       
        float scale = 1.0f / m_MaxCoord;
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        float shin = 0.f;
        material->Get(AI_MATKEY_SHININESS, shin);
     
        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            MeshVertex vertex;
            glm::vec3 vector; 
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


            vertex.TextureSlot = 0;

            vertices.push_back(vertex);
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
        std::vector<Shared<Texture2D>> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Shared<Texture2D>> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Shared<Texture2D>> normalMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. shineness maps
        std::vector<Shared<Texture2D>> shininessMaps = LoadMaterialTextures(material, aiTextureType_SHININESS, "texture_height");
        textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());
        // 4. base color maps
        std::vector<Shared<Texture2D>> colorMaps = LoadMaterialTextures(material, aiTextureType_BASE_COLOR, "texture_color");
        textures.insert(textures.end(), colorMaps.begin(), colorMaps.end());
       



        if (!diffuseMaps.empty()) //TODO: MAKE IT WORK WITH MULTIPLY TEXTURES
            mat.diffuseMap = diffuseMaps[0];
        if (!normalMaps.empty()) //TODO: MAKE IT WORK WITH MULTIPLY TEXTURES
            mat.normalMap = normalMaps[0];
       if(!specularMaps.empty())
            mat.specularMap = specularMaps[0];
        
       if (!shin)mat.shininess = shin;
       else mat.shininess = 32.f;
        

        // return a mesh object created from the extracted mesh data
        return CreateShared<Mesh>(vertices, indices, mat);
    }

    std::vector<Shared<Texture2D>> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
            std::vector<Shared<Texture2D>> textures;
            Shared<Texture2D> texture;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                texture = Texture2D::Create(this->m_Directory + '/' + str.C_Str());
                textures.push_back(texture);
            }
            if (!mat->GetTextureCount(type)) 
            {
                std::string path = this->m_Directory + '/' + "textures" + '/';

                if (type == aiTextureType_DIFFUSE && IsFileExists(path + "diffuse.jpg"))
                {
                    texture = Texture2D::Create(path + "diffuse.jpg");
                }

                if (type == aiTextureType_HEIGHT && IsFileExists(path + "normalMap.png"))
                {
                    texture = Texture2D::Create(path + "normalMap.png");
                }

                textures.push_back(texture);

            }
            
            return textures;
            
	}

    void Model::BindAllTexture()
    {

      /*  uint32_t index = 1;
        for (auto& mesh : m_Meshes)
        {
            if (mesh->GetMaterial().diffuseMap) {
                mesh->GetMaterial().diffuseMap->Bind(index);

            }
            if (mesh->GetMaterial().specularMap) {
                mesh->GetMaterial().specularMap->Bind(++index);
            }
            index++;

        }*/
    }
    bool Model::IsIntersect(const CameraRay& ray)
    {
        

        auto AABB = m_ModelAABB;


        double tx1 = (AABB.mMin.x - ray.GetOrigin().x) * (1 / ray.GetDirection().x);
        double tx2 = (AABB.mMax.x - ray.GetOrigin().x) * (1 / ray.GetDirection().x);

        double tmin = std::min(tx1, tx2);
        double tmax = std::max(tx1, tx2);

        double ty1 = (AABB.mMin.y - ray.GetOrigin().y) * (1 / ray.GetDirection().y);
        double ty2 = (AABB.mMax.y - ray.GetOrigin().y) * (1 / ray.GetDirection().y);

        tmin = std::max(tmin, std::min(ty1, ty2));
        tmax = std::min(tmax, std::max(ty1, ty2));

        double tz1 = (AABB.mMin.z - ray.GetOrigin().z) * (1 / ray.GetDirection().z);
        double tz2 = (AABB.mMax.z - ray.GetOrigin().z) * (1 / ray.GetDirection().z);

        tmin = std::max(tmin, std::min(tz1, tz2));
        tmax = std::min(tmax, std::max(tz1, tz2));


        return tmax >= tmin;


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

    inline bool Model::IsFileExists(const std::string& name)
    {
        struct stat buffer;
        return (stat(name.c_str(), &buffer) == 0);
    }

}