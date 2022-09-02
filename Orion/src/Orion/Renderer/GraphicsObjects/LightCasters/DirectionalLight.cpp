#include "oripch.h"
#include "DirectionalLight.h"




namespace Orion
{

	void DirectionalLight::SetupLight()
	{
		
	}
	void DirectionalLight::LoadToShader(const Shared<Shader>& shader)
	{
		shader->SetFloat3("u_Dirlight.direction", m_LightProp.Direction);
		shader->SetFloat3("u_Dirlight.ambient", m_LightProp.AmbientLightColor);
		shader->SetFloat3("u_Dirlight.diffuse", m_LightProp.DiffuseLightColor);
		shader->SetFloat3("u_Dirlight.specular", m_LightProp.SpecularLightColor);
	}


}