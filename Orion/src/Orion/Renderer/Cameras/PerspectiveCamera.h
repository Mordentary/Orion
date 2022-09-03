#pragma once

#include"DummyCamera.h"

namespace Orion {
	class Application;
	class Timestep;
	class PerspectiveCamera : public Orion::DummyCamera
	{
	public:
		PerspectiveCamera() = default;
		PerspectiveCamera(const glm::vec3& position, const glm::vec3& cameraTarget, float FOV = 90.0f);

		void Update(Timestep deltaTime) override;

		virtual const Cameras Get() { return Cameras::Perspective; }

		

		

	private:
		void UpdateCameraOrientation(Orion::Timestep dt);
		void UpdateCameraDisplacement(Orion::Timestep deltaTime);

		virtual void RecalculateView() override;
		virtual void RecalculateViewProjection() override;
		virtual void RecalculateProjection() override;

	private:

		float m_FOVdeg;
	
		const glm::vec3 m_WorldSpaceAxisY{ 0.0f,1.0f,0.0f };

		glm::vec3 m_CameraSpaceAxisX{ 1.0f, 0.0f, 0.0f};
		glm::vec3 m_CameraSpaceAxisY{0.0f, 1.0f, 0.0f};
		glm::vec3 m_CameraSpaceAxisZ{ 0.0f, 0.0f, -1.0f};
	



	};
}