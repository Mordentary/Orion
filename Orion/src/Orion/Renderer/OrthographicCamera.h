#pragma once

#include"glm/glm.hpp"
#include "Orion/Events/ApplicationEvent.h"
#include "Orion/Events/MouseEvent.h"
#include<glm/gtc/matrix_transform.hpp>	
namespace Orion {
	class OrthographicCamera
	{
	public:
		//OrthographicCamera() = default;
		OrthographicCamera(float aspect, bool rotation = false);
		OrthographicCamera(float left, float right, float bottom, float top, bool rotation = false);
		void SetProjection(float left, float right, float bottom, float top);
	

		void inline SetPosition(const glm::vec3& position){	m_Position = position; RecalculateViewMatrix();}
		void inline SetRotation(float rotation){m_CameraRotation = rotation; RecalculateViewMatrix();}
		void inline SetZoomLevel(float zoom){ m_ZoomLevel = zoom; }
		const inline void SetAspectRatio(float aspect) { m_AspectRatio = aspect;  }
		inline void SetTraslationSpeed(float tranSpeed){m_CameraTranslationSpeed = tranSpeed;}
		inline void SetRotationSpeed(float rotSpeed){	m_CameraRotationSpeed = rotSpeed;}

		const inline glm::vec3& GetPosition() const { return m_Position;}
		const inline float GetRotation() const { return m_CameraRotation; }
		const inline float GetAspectRatio() const { return m_AspectRatio; }
		const inline float IsRotation() const { return m_Rotation; }
		const inline float GetZoomLevel() const { return m_ZoomLevel; }
		inline float GetTranslationSpeed(){return m_CameraTranslationSpeed;}
		inline float GetRotationSpeed(){return m_CameraRotationSpeed;}
		const inline glm::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
		const inline glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const inline glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
	
	private:
		void RecalculateViewMatrix();

	private:
		float m_AspectRatio = 1.6f;
		float m_ZoomLevel = 0.9f;
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 5.0f, m_CameraRotationSpeed = 180.0f;
		bool m_Rotation = false;

		glm::vec3 m_Position;
		glm::mat4 m_ProjectionViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;


	};
}
