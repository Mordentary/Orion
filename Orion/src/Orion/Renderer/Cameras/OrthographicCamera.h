#pragma once

#include"DummyCamera.h"
#include<glm/gtc/matrix_transform.hpp>	
#include"glm/glm.hpp"
#include"Orion/Core/TimeHelper.h"

namespace Orion {

	class OrthographicCamera : public Orion::DummyCamera
	{
	public:
		//OrthographicCamera() = default;
		OrthographicCamera(glm::vec3& position, bool rotation = false, bool pixelByPixel = false);
		OrthographicCamera(float left, float right, float bottom, float top, bool rotation = false);
		virtual ~OrthographicCamera() = default;



		virtual void Update(Timestep deltaTime) override;

		virtual const Cameras Get() { return Cameras::Orthographic; }
		void SetProjection(float left, float right, float bottom, float top);
		void inline SetRotation(float rotation) { m_CameraRotation = rotation; RecalculateView(); }
	
		virtual void RecalculateView() override;
		virtual void RecalculateViewProjection() override;
		virtual void RecalculateProjection() override;

	private:
		bool m_ProjMatchesPixelByPixel = false;
		float m_CameraRotation = 0.0f;



	};
}
