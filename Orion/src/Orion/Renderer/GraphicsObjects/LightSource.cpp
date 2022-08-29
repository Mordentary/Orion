#include "oripch.h"
#include "LightSource.h"



namespace Orion
{
	uint32_t LightSource::s_PointLightIndex = 0;
	uint32_t LightSource::s_SpotLightIndex = 0;

	uint32_t& LightSource::GetCountOfSpotLights() { return s_SpotLightIndex; }
	uint32_t& LightSource::GetCountOfPointLights() { return s_PointLightIndex; }
}