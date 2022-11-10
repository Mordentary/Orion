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
	
		m_ViewMatrix = glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::normalize(m_LightProp.Direction), glm::vec3(0.0, 0.0, 1.0));

		auto& depthShader = Orion::ShaderLibrary::Get("TextureDepthShader");
		depthShader->Bind();
		depthShader->SetMat4("u_ViewProjMatrix", m_ProjMatrix * m_ViewMatrix);

		currentShader = depthShader;

		m_ShadowMap->Bind();
		Orion::RenderCommand::Clear(ORI_CLEAR_DEPTH);

		for (Shared<LightSource>& light : otherLights)
		{
			if (light.get() == this) continue;
			light->RenderLightModel(depthShader);
		}

		renderFunc();

		m_ShadowMap->Unbind();

		
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

		m_ShadowMap->GetDepthAttachmentTexture()->Bind(6);
		shader->SetInt("u_ShadowMapSpot", m_ShadowMap->GetDepthAttachmentTexture()->GetCurrentSlot());

		shader->SetMat4("u_SpotLightMatrix", m_ProjMatrix * m_ViewMatrix);

	}


}