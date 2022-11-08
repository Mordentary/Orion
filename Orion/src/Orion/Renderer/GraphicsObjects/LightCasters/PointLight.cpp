#include "oripch.h"
#include "PointLight.h"
namespace Orion
{
	void PointLight::RenderLightModel(Shared<Shader>& shader)
	{
		if (m_LightModel)
		{
			shader->Bind();
			shader->SetMat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), m_LightProp.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
			shader->SetFloat3("u_LightColor", m_LightProp.DiffuseLightColor);

			m_LightModel->Render(shader);
		}
	}
	void PointLight::SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc)
	{

	}
	void PointLight::LoadToLightShader()
	{
		auto& shader = Orion::ShaderLibrary::Get("PhongShader");
	
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