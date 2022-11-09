#include "oripch.h"
#include "DirectionalLight.h"




namespace Orion
{
	void DirectionalLight::RenderLightModel(Shared<Shader>& shader)
	{
		if (m_LightModel) 
		{
			shader->Bind();
			shader->SetMat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), m_LightProp.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
			shader->SetFloat3("u_LightColor", m_LightProp.DiffuseLightColor);

			m_LightModel->Render(shader);
		}
	}
	void DirectionalLight::SetupLight(
		Shared<Shader>& currentShader, 
		std::vector<Shared<LightSource>>& otherLights, 
		std::function<void()> renderFunc)
	{
		m_ViewMatrix = glm::lookAt(glm::vec3(0.f), m_LightProp.Direction, glm::vec3(0.0f, 1.0f, 0.0f));


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
	void DirectionalLight::LoadToLightShader()
	{

		auto& shader = Orion::ShaderLibrary::Get("PhongShader");

		shader->Bind();
		shader->SetFloat3("u_Dirlight.direction", m_LightProp.Direction);
		shader->SetFloat3("u_Dirlight.ambient", m_LightProp.AmbientLightColor / 2.f);
		shader->SetFloat3("u_Dirlight.diffuse", m_LightProp.DiffuseLightColor / 2.f);
		shader->SetFloat3("u_Dirlight.specular", m_LightProp.SpecularLightColor / 2.f);

		m_ShadowMap->GetDepthAttachmentTexture()->Bind(4);
		shader->SetInt("u_ShadowMap", m_ShadowMap->GetDepthAttachmentTexture()->GetCurrentSlot());
		shader->SetMat4("u_DirLightMatrix", m_ProjMatrix * m_ViewMatrix);

	}


}