#include "oripch.h"
#include "PointLight.h"
namespace Orion
{
	void PointLight::RenderLightModel(Shared<Shader>& shader)
	{
		if (m_LightModel)
		{
			shader->Bind();
			shader->SetMat4("u_ModelMatrix", glm::translate(glm::mat4(1.0f), m_Prop.GeneralProp.Position) * glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));
			shader->SetFloat3("u_LightColor", m_Prop.GeneralProp.DiffuseLightColor);

			m_LightModel->Render(shader);
		}
	}
	void PointLight::SetupLight(Shared<Shader>& currentShader, std::vector<Shared<LightSource>>& otherLights, std::function<void()> renderFunc)
	{
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.reserve(6);
		shadowTransforms.push_back(
			m_ProjMatrix *
			glm::lookAt(m_Prop.GeneralProp.Position, m_Prop.GeneralProp.Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_Prop.GeneralProp.Position, m_Prop.GeneralProp.Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_Prop.GeneralProp.Position, m_Prop.GeneralProp.Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_Prop.GeneralProp.Position, m_Prop.GeneralProp.Position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_Prop.GeneralProp.Position, m_Prop.GeneralProp.Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(m_ProjMatrix *
			glm::lookAt(m_Prop.GeneralProp.Position, m_Prop.GeneralProp.Position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		auto& depthShader = Orion::ShaderLibrary::Get("CubemapDepthShader");
		depthShader->Bind();
		depthShader->SetMat4Array("u_ViewProjMatrices", shadowTransforms.data(),6);
		depthShader->SetFloat("u_FarPlane", m_Prop.FarPlane);
		depthShader->SetFloat3("u_LightPos", m_Prop.GeneralProp.Position);

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

	void PointLight::LoadLightToShader(const Shared<Shader>& shader)
	{
		m_ShadowMap->GetDepthAttachmentTexture()->Bind(15);
		shader->SetInt("u_ShadowCubemap", m_ShadowMap->GetDepthAttachmentTexture()->GetCurrentSlot());
	}

	void PointLight::LoadLightToUBO(const Shared<UniformBuffer>& ubo)
	{
		CalculateBoundingVolume();
		
		ubo->SetData(&m_Prop, 0, sizeof(m_Prop));

	}

	void PointLight::SetLighAttenuation(float linear, float quadratic)
	{
		m_Prop.LinearAttenuation = linear;
		m_Prop.QuadraticAttenuation = quadratic;
	}
	void PointLight::CalculateBoundingVolume()
	{
		float MaxColor = std::fmaxf(std::fmaxf(m_Prop.GeneralProp.DiffuseLightColor.r, m_Prop.GeneralProp.DiffuseLightColor.g), m_Prop.GeneralProp.DiffuseLightColor.b);
		m_Prop.Radius =
			(-m_Prop.LinearAttenuation + std::sqrtf(m_Prop.LinearAttenuation * m_Prop.LinearAttenuation - 4 * m_Prop.QuadraticAttenuation* (m_Prop.ConstantAttenuation - (256.f / 5.0) * MaxColor)))
			/ (2 * m_Prop.QuadraticAttenuation);

	}
}