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
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.reserve(6);
		shadowTransforms.push_back(
			m_ProjMatrix *
			glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_LightProp.Position, m_LightProp.Position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		auto& depthShader = Orion::ShaderLibrary::Get("CubemapDepthShader");
		depthShader->Bind();
		depthShader->SetMat4Array("u_ViewProjMatrices", shadowTransforms.data(),6);
		depthShader->SetFloat("u_FarPlane", m_FarPlane);
		depthShader->SetFloat3("u_LightPos", m_LightProp.Position);

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
	void PointLight::LoadToLightShader(const Shared<Shader>& shader)
	{
		
	
		shader->Bind();
		shader->SetFloat3("u_Pointlight.position", m_LightProp.Position);

		shader->SetFloat3("u_Pointlight.ambient", m_LightProp.AmbientLightColor);
		shader->SetFloat3("u_Pointlight.diffuse", m_LightProp.DiffuseLightColor);
		shader->SetFloat3("u_Pointlight.specular", m_LightProp.SpecularLightColor);

		shader->SetFloat("u_Pointlight.constant", m_LightProp.ConstantAttenuation);
		shader->SetFloat("u_Pointlight.linear", m_LightProp.LinearAttenuation);
		shader->SetFloat("u_Pointlight.quadratic", m_LightProp.QuadraticAttenuation);

		shader->SetFloat("u_Pointlight.farPlane", m_FarPlane);


		m_ShadowMap->GetDepthAttachmentTexture()->Bind(5);
		shader->SetInt("u_ShadowCubemap", m_ShadowMap->GetDepthAttachmentTexture()->GetCurrentSlot());
		


	}


}