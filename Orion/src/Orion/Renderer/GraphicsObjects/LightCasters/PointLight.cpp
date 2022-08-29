#include "oripch.h"
#include "PointLight.h"
namespace Orion
{

	void PointLight::SetupLight()
	{

	}
	void PointLight::LoadToShader(const Shared<Shader>& shader)
	{
		shader->Bind();
		shader->SetFloat3("u_Spotlight[" + std::to_string(s_PointLightIndex) + "].position", m_LightProp.Position);

		shader->SetFloat3("u_Spotlight[" + std::to_string(s_PointLightIndex) + "].ambient", m_LightProp.AmbientLightColor);
		shader->SetFloat3("u_Spotlight[" + std::to_string(s_PointLightIndex) + "].diffuse", m_LightProp.DiffuseLightColor);
		shader->SetFloat3("u_Spotlight[" + std::to_string(s_PointLightIndex) + "].specular", m_LightProp.SpecularLightColor);

		shader->SetFloat("u_Spotlight[" + std::to_string(s_PointLightIndex) + "].constant", m_LightProp.ConstantAttenuation);
		shader->SetFloat("u_Spotlight[" + std::to_string(s_PointLightIndex) + "].linear", m_LightProp.LinearAttenuation);
		shader->SetFloat("u_Spotlight[" + std::to_string(s_PointLightIndex) + "].quadratic", m_LightProp.QuadraticAttenuation);

		s_PointLightIndex++;

	}


}