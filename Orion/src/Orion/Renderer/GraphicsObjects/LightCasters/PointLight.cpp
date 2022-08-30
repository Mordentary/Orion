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
		shader->SetFloat3("u_Pointlight.position", m_LightProp.Position);

		shader->SetFloat3("u_Pointlight.ambient", m_LightProp.AmbientLightColor);
		shader->SetFloat3("u_Pointlight.diffuse", m_LightProp.DiffuseLightColor);
		shader->SetFloat3("u_Pointlight.specular", m_LightProp.SpecularLightColor);

		shader->SetFloat("u_Pointlight.constant", m_LightProp.ConstantAttenuation);
		shader->SetFloat("u_Pointlight.linear", m_LightProp.LinearAttenuation);
		shader->SetFloat("u_Pointlight.quadratic", m_LightProp.QuadraticAttenuation);


	}


}