#include "oripch.h"
#include "SpotLight.h"

namespace Orion
{
	void SpotLight::RenderLightModel(Shared<Shader>& shader)
	{
		if (m_LightModel)
		{
			shader->Bind();
			shader->SetMat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), m_LightProp.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
			shader->SetFloat3("u_LightColor", m_LightProp.DiffuseLightColor);

			m_LightModel->Render(shader);
		}
	}
	void SpotLight::SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc)
	{
		auto& shader = Orion::ShaderLibrary::Get("CubemapDepthShader");

		shader->Bind();
		shader->SetMat4("u_ViewProjMatrix", m_ViewMatrix * m_ProjMatrix);
	}
	void SpotLight::LoadToLightShader()
	{

		auto& shader = Orion::ShaderLibrary::Get("PhongShader");


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