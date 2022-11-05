#include "oripch.h"
#include "DirectionalLight.h"




namespace Orion
{

	void DirectionalLight::SetupLight()
	{
		
	}
	void DirectionalLight::LoadToShader(const Shared<Shader>& shader)
	{
		shader->Bind();
		shader->SetFloat3("u_Dirlight.direction", m_LightProp.Direction);
		shader->SetFloat3("u_Dirlight.ambient", m_LightProp.AmbientLightColor / 2.f);
		shader->SetFloat3("u_Dirlight.diffuse", m_LightProp.DiffuseLightColor / 2.f);
		shader->SetFloat3("u_Dirlight.specular", m_LightProp.SpecularLightColor / 2.f);
	}


}