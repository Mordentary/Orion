#include "oripch.h"
#include "DirectionalLight.h"
#include"../LightHandler.h"



namespace Orion
{
	void DirectionalLight::RenderLightModel(Shared<Shader>& shader)
	{
		/*if (m_LightModel) 
		{
			shader->Bind();
			shader->SetMat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), m_LightProp.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
			shader->SetFloat3("u_LightColor", m_LightProp.DiffuseLightColor);

			m_LightModel->Render(shader);
		}*/
	}
	void DirectionalLight::SetupLight(
		Shared<Shader>& currentShader, 
		std::vector<Shared<LightSource>>& otherLights, 
		std::function<void()> renderFunc)
	{
		m_ViewMatrix = glm::lookAt(glm::vec3(0.f), m_Prop.GeneralProp.Direction, glm::vec3(0.0f, 1.0f, 0.0f));


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
	void DirectionalLight::LoadLightToShader(const Shared<Shader>& shader)
	{
		m_ShadowMap->GetDepthAttachmentTexture()->Bind(13);
		shader->SetInt("u_ShadowMapDir", m_ShadowMap->GetDepthAttachmentTexture()->GetCurrentSlot());

	}

	void DirectionalLight::IncreaseLightCounter()
	{
		++Orion::LightHandler::m_LightCounters.DirLightCount;
	}


	void DirectionalLight::UpdateFrustum()
	{

		
	}


	void DirectionalLight::LoadLightToUBO(const Shared<UniformBuffer>& ubo)
	{
		m_Prop.ViewProj = m_ProjMatrix * m_ViewMatrix;

		//uint32_t  align = alignof(LightSource::DirectionalLightProp);
		//ORI_ASSERT(alignof(LightSource::DirectionalLightProp) == 16, "Inproper alignment");


		ubo->SetData(&m_Prop,  (sizeof(LightSource::PointLightProp) + sizeof(LightSource::SpotLightProp)) * Orion::LightHandler::MAX_LIGHTS, sizeof(m_Prop));

	}

}