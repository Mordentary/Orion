#pragma once
#include <Orion/Core/TimeHelper.h>


namespace Orion 
{

	class DummyCamera
	{

	public:

		DummyCamera(
			const glm::vec3& position,
			bool rotation = true,
			const glm::mat4& viewMatrix = glm::mat4(1.0f),
			const glm::mat4& projectionMatrix = glm::mat4(1.0f),
			const glm::mat4& projectionViewMatrix = glm::mat4(1.0f)
		);
		
		virtual void Update(Orion::Timestep deltaTime) {}
		virtual void RecalculateView() {}
		virtual void RecalculateViewProjection() {}
		virtual void RecalculateProjection() {}

		const inline glm::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
		const inline glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const inline glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		inline const glm::vec3& GetPosition() const { return m_Position; }

	    inline const float& GetAspectRatio() const { return m_AspectRatio; }
		inline const float& GetZoomLevel() const { return m_ZoomLevel; }

		inline const float& GetSensitivity() const { return m_CameraSensitivity; }
		inline const float& GetTranslationSpeed() { return m_CameraTranslationSpeed; }

		inline const float IsRotation() const { return m_Rotation; }
		

		inline void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline void SetScreenSize(const glm::vec2& size) { m_ScreenSize = size;}
		inline void SetZoomLevel(float zoom) { m_ZoomLevel = zoom;}
		inline void SetAspectRatio(float aspect) { m_AspectRatio = aspect; }
		inline void SetTraslationSpeed(float tranSpeed) { m_CameraTranslationSpeed = tranSpeed; }
		inline void SetSensitivity(float rotSpeed) { m_CameraSensitivity = rotSpeed; }

	

	protected: 
		float m_AspectRatio = 1.7f;
		float m_ZoomLevel = 1.0f;
		float m_CameraTranslationSpeed = 1.0f, m_CameraSensitivity = 180.0f;
		bool m_Rotation = false;

		glm::vec3 m_Position;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ProjectionViewMatrix;

		glm::vec2 m_ScreenSize;

	protected:


	private:




	};

}
