#pragma once
#include"../LightSource.h"
namespace Orion
{
	class PointLight : public LightSource
	{
	public:
		PointLight() = default;
		PointLight(const std::vector<MeshVertex>& vertices, std::vector<uint32_t>& indices, const Material& material) : LightSource(vertices, indices, material)
		{
		}

		virtual void SetupLight() override;
		virtual void LoadToShader(const Shared<Shader>& shader) override;


	};
}
