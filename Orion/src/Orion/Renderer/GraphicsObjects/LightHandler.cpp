#include "oripch.h"
#include"LightHandler.h"
#include"../GraphicsObjects/LightSource.h"

namespace Orion
{

	void LightHandler::AddLightSource(const Shared<LightSource>& light) 
	{
		light->IncreaseLightCounter(*this);
		m_LightSources.push_back(light);
	}

	void LightHandler::PrepareLights(Shared<Shader>& currentShader, std::function<void()> renderFunc)
	{
		if (!m_LightSources.empty())
		{
			for (auto& lightSrc : m_LightSources)
			{
				lightSrc->SetupLight(currentShader, m_LightSources, renderFunc);
			}
		}
	}

	

	void LightHandler::LoadLightsToShaderAndRender(const Shared<Shader>& shader)
	{
		if (!m_LightSources.empty())
		{
			for (auto& lightSrc : m_LightSources)
			{
				lightSrc->LoadLightToShader(shader);
			}

			for (auto& lightSrc : m_LightSources)
			{
				lightSrc->RenderLightModel(m_LightShader);
			}
		}

	}
	void LightHandler::LoadLightsToShader(const Shared<Shader>& shader)
	{
		if (!m_LightSources.empty())
		{
			for (auto& lightSrc : m_LightSources)
			{
				lightSrc->LoadLightToShader(shader);
			}
		}

	}
	void LightHandler::LoadLightsToUBO(const Shared<UniformBuffer>& ubo)
	{
		if (!m_LightSources.empty())
		{
			for (auto& lightSrc : m_LightSources)
			{
				lightSrc->LoadLightToUBO(ubo);
			}
		}
	}


	void LightHandler::RenderLights()
	{
		if (!m_LightSources.empty())
		{
			for (auto& lightSrc : m_LightSources)
			{
				lightSrc->RenderLightModel(m_LightShader);
			}
		}

	}


	void LightHandler::UpdateLightSettings(float linearAttenuation, float quadraticAttenuation)
	{
		if (!m_LightSources.empty())
		{
			for (auto& lightSrc : m_LightSources)
			{
				lightSrc->SetLighAttenuation(linearAttenuation, quadraticAttenuation);
			}
		}
	}
}