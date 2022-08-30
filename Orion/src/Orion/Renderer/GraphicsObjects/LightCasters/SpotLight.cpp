#include "oripch.h"
#include "SpotLight.h"

namespace Orion
{

	void SpotLight::SetupLight()
	{

	}
	void SpotLight::LoadToShader(const Shared<Shader>& shader)
	{
		shader->SetFloat3("u_Spotlight.position", m_LightProp.Position);
		shader->SetFloat3("u_Spotlight.direction", m_LightProp.Direction);

		shader->SetFloat("u_Spotlight.innerCutOff", cos(glm::radians(m_InnerCutOff)));
		shader->SetFloat("u_Spotlight.outerCutOff", cos(glm::radians(m_OuterCutOff)));

		shader->SetFloat3("u_Spotlight.ambient", m_LightProp.AmbientLightColor);
		shader->SetFloat3("u_Spotlight.diffuse", m_LightProp.DiffuseLightColor);
		shader->SetFloat3("u_Spotlight.specular", m_LightProp.SpecularLightColor);

		shader->SetFloat("u_Spotlight.constant", m_LightProp.ConstantAttenuation);
		shader->SetFloat("u_Spotlight.linear", m_LightProp.LinearAttenuation);
		shader->SetFloat("u_Spotlight.quadratic", m_LightProp.QuadraticAttenuation);

		
	}


}