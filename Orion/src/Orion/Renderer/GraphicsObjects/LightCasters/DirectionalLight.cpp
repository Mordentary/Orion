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

		UpdateFrustum();
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
		// Calculate the inverse of the projection-view matrix
		glm::mat4 invProjView = glm::inverse(m_ProjMatrix * m_ViewMatrix);

		std::array<glm::vec3, 8> corners;
		// Calculate the corner points by transforming the eight points (-1, -1, -1), (1, -1, -1), (1, 1, -1), (-1, 1, -1),
		// (-1, -1, 1), (1, -1, 1), (1, 1, 1), and (-1, 1, 1) through the inverse projection-view matrix
		corners[0] = glm::vec3(invProjView * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
		corners[1] = glm::vec3(invProjView * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
		corners[2] = glm::vec3(invProjView * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
		corners[3] = glm::vec3(invProjView * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));


		corners[4] = glm::vec3(invProjView * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
		corners[5] = glm::vec3(invProjView * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
		corners[6] = glm::vec3(invProjView * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		corners[7] = glm::vec3(invProjView * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));

		m_Frustum.Top.Normal = glm::normalize(glm::cross(corners[0] - corners[1], corners[4] - corners[0]));
		m_Frustum.Top.Point = corners[2]; // top

		m_Frustum.Right.Normal = glm::normalize(glm::cross(corners[0] - corners[4], corners[3] - corners[0]));
		m_Frustum.Right.Point = corners[2]; // right

		m_Frustum.Bottom.Normal = glm::normalize(glm::cross(corners[6] - corners[2], corners[7] - corners[6]));
		m_Frustum.Bottom.Point = corners[0]; // bottom

		m_Frustum.Left.Normal = -glm::normalize(glm::cross(corners[1] - corners[5], corners[2] - corners[5]));
		m_Frustum.Left.Point = corners[0]; // left

		m_Frustum.Near.Normal = glm::cross(m_Frustum.Left.Normal, m_Frustum.Top.Normal);
		m_Frustum.Near.Point = corners[0]; // near

		m_Frustum.Far.Normal = -glm::cross(m_Frustum.Left.Normal, m_Frustum.Top.Normal);
		m_Frustum.Far.Point = corners[4]; // far


		
	}


	void DirectionalLight::LoadLightToUBO(const Shared<UniformBuffer>& ubo)
	{
		m_Prop.ViewProj = m_ProjMatrix * m_ViewMatrix;

		//uint32_t  align = alignof(LightSource::DirectionalLightProp);
		//ORI_ASSERT(alignof(LightSource::DirectionalLightProp) == 16, "Inproper alignment");


		ubo->SetData(&m_Prop,  (sizeof(LightSource::PointLightProp) + sizeof(LightSource::SpotLightProp)) * Orion::LightHandler::MAX_LIGHTS, sizeof(m_Prop));

	}

}