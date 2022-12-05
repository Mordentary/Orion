#pragma once

#include"DummyCamera.h"
#include<glm/gtc/matrix_transform.hpp>	
#include"glm/glm.hpp"
#include"Orion/Core/TimeHelper.h"

namespace Orion {


	class OrthographicCamera : public DummyCamera
	{
	public:
		//OrthographicCamera() = default;
		OrthographicCamera(glm::vec3& position, bool rotation = false, bool pixelByPixel = false);
		OrthographicCamera(glm::vec3& position, glm::vec3& dir, glm::vec4& borders, glm::vec2& nearFarPlanes);
		OrthographicCamera(float left, float right, float bottom, float top, bool rotation = false);


		virtual ~OrthographicCamera() = default;



		virtual void Update(Timestep deltaTime) override;

		virtual const Cameras Get() { return Cameras::Orthographic; }
		void SetProjection(float left, float right, float bottom, float top);
		void inline SetRotationAngles(float rotation) { m_CameraRotation = rotation; RecalculateView(); }
	
		void UpdateFrustum() override;
		void RenderFrustum() override;


	private:
		bool m_ProjMatchesPixelByPixel = false;
		float m_CameraRotation = 0.0f;
		glm::vec4 m_Borders;
		glm::vec2 m_NearFar;
	private:
		virtual void RecalculateView() override;
		virtual void RecalculateViewProjection() override;
		virtual void RecalculateProjection() override;


	};
}
