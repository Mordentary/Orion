#include "oripch.h"
#include "Model.h"



namespace Orion
{

	void Model::Render(Shared<Shader>& shader)
	{
		for (auto& mesh : m_Meshes)
                 mesh->Render(shader);
        
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
		m_Directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);

	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
            // process each mesh located at the current node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene. 
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
             
                m_Meshes.push_back(processMesh(mesh, scene));
            }
            // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene);
            }
	}

    Shared<Mesh> Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill 
        
        std::vector<MeshVertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<Shared<Texture2D>> textures;
        // walk through each of the mesh's vertices
        auto MinMax = (mesh->mAABB.mMax - mesh->mAABB.mMin);
        glm::vec3 scale = glm::vec3(1.0f) / glm::vec3(MinMax.x, MinMax.x, MinMax.x);

        for (uint32_t i = 0; i < mesh->mNumVertices; i++)
        {
            MeshVertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
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
            else vertex.Color = glm::vec4(1.0f);


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
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN
        aiColor3D color(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        Material mat;

     
        // 1. diffuse maps
        std::vector<Shared<Texture2D>> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Shared<Texture2D>> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Shared<Texture2D>> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. sheneness maps
        std::vector<Shared<Texture2D>> shininessMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_height");
        textures.insert(textures.end(), shininessMaps.begin(), shininessMaps.end());
        // 4. base color maps
        std::vector<Shared<Texture2D>> colorMaps = loadMaterialTextures(material, aiTextureType_BASE_COLOR, "texture_height");
        textures.insert(textures.end(), colorMaps.begin(), colorMaps.end());




        if (!diffuseMaps.empty())
          mat.diffuseMap = diffuseMaps[0];
       if(!specularMaps.empty())
            mat.specularMap = specularMaps[0];
        

        mat.shininess = 32.f;
        

        // return a mesh object created from the extracted mesh data
        return CreateShared<Mesh>(vertices, indices, mat);
    }

    std::vector<Shared<Texture2D>> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
            std::vector<Shared<Texture2D>> textures;
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str;
                mat->GetTexture(type, i, &str);
                Shared<Texture2D> texture;
                texture = Texture2D::Create(this->m_Directory + '/' + str.C_Str());
                textures.push_back(texture);
            }


            return textures;
	}


}