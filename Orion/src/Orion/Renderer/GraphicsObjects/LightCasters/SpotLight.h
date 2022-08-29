#pragma once
#include"../LightSource.h"

namespace Orion
{
	class SpotLight : public LightSource
	{
	public:
		SpotLight() = default;

		SpotLight(const std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, const Material& material) : LightSource(vertices, indices, material)
		{
		}
		

		virtual void SetupLight() override;
		virtual void LoadToShader(const Shared<Shader>& shader) override;

	private: 
		float m_InnerCutOff = 25.f, m_OuterCutOff = 45.f;
	};
}
