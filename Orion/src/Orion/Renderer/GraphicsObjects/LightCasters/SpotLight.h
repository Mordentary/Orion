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


		inline float& GetInnerCutOff() { return m_InnerCutOff; }
		inline float& GetOuterCutOff() { return m_OuterCutOff; }

	private: 
		float m_InnerCutOff = 0.5f, m_OuterCutOff = 5.f;
	};
}
