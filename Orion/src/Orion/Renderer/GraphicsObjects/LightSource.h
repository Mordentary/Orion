#pragma once
#include"Mesh.h"
#include"../GraphicsCore/Shader.h"

namespace Orion
{

	struct LightProperties
	{
		glm::vec3 Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 Direction{ 0.0f,0.0f, -1.0f };

		glm::vec3 AmbientLightColor{ 0.1f, 0.1f, 0.1f };
		glm::vec3 DiffuseLightColor{ 0.5f, 0.5f, 0.5f};
		glm::vec3 SpecularLightColor{ 0.5f, 0.5f, 0.5f };

		float ConstantAttenuation =  1.0f;
		float LinearAttenuation =  0.045;
		float QuadraticAttenuation =  0.0075;
	};
	class LightSource : Mesh
	{
	public:

		LightSource() = default;

		LightSource(const std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, const Material& material) : Mesh(vertices, indices, material) 
		{
			
			SetupLight();
		}

		LightSource(const Shared<Shader>& shader, const Shared<Mesh>& mesh) :Mesh(mesh)
		{
			SetupLight();
		}
		virtual void LoadToShader(const Shared<Shader>& shader) = 0;
		
		LightProperties& GetLightProperties() { return m_LightProp; }

		 static uint32_t& GetCountOfSpotLights();
		 static uint32_t& GetCountOfPointLights();

	protected:
		LightProperties m_LightProp;
		static uint32_t s_SpotLightIndex;
		static uint32_t s_PointLightIndex;

		virtual void SetupLight() = 0;


	};
		
}

