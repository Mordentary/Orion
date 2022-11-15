#include "oripch.h"
#include "PerspectiveCamera.h"
#include"Orion/Core/Application.h"
#include"Orion/Core/Input.h"
#include"Orion/Core/KeyCodes.h"
#include"Orion/Core/MouseButtonCodes.h"


#include<glm/gtc/matrix_transform.hpp>	
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
namespace Orion
{
	PerspectiveCamera::PerspectiveCamera(const glm::vec3& position, const glm::vec3& cameraTarget, float FOV) : DummyCamera(position), m_FOVdeg(FOV)
	{
		if (position == cameraTarget) 
		{
			ORI_CORE_ASSERT(false, "Camera target and position are equal!");
		}
		m_CameraSpaceAxisZ = -glm::normalize(position - cameraTarget);
		m_CameraSpaceAxisX = glm::normalize(glm::cross(m_CameraSpaceAxisZ, m_WorldSpaceAxisY));
		m_CameraSpaceAxisY = -glm::cross(m_CameraSpaceAxisZ, m_CameraSpaceAxisX);

		m_ViewMatrix = glm::lookAt(position, position + m_CameraSpaceAxisZ, m_CameraSpaceAxisY);

		m_CameraForward = m_CameraSpaceAxisZ;
	}

	void PerspectiveCamera::Update(Orion::Timestep dt)
	{

		UpdateCameraOrientation(dt);

		UpdateCameraDisplacement(dt);

		RecalculateViewProjection();


	}

	

	void PerspectiveCamera::UpdateCameraOrientation(Orion::Timestep dt)
	{



		static bool firstClick = true;
		Orion::Application* app;


		if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_RIGHT))
		{
			app = &Orion::Application::Get();
			const float& width = m_ScreenSize.x;
			const float& height = m_ScreenSize.y;
			const float& sensitivity = m_CameraSensitivity;


			app->GetWindow().DisableCursor(true);

			if (firstClick)
			{
				app->GetWindow().SetCursorPosition(width / 2, height / 2);
				firstClick = false;
			}


			auto& [mouseX, mouseY] = Orion::Input::GetMousePosition();

			float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
			float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

			glm::vec3 newOrientation = glm::rotate(m_CameraSpaceAxisZ, glm::radians(-rotX), glm::normalize(glm::cross(m_CameraSpaceAxisZ, m_CameraSpaceAxisY)));

			if (abs(glm::angle(newOrientation, m_CameraSpaceAxisY) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				m_CameraSpaceAxisZ = newOrientation;
			}

			m_CameraSpaceAxisZ = glm::rotate(m_CameraSpaceAxisZ, glm::radians(-rotY), m_CameraSpaceAxisY);

			m_CameraForward = m_CameraSpaceAxisZ;
			app->GetWindow().SetCursorPosition(width / 2, height / 2);
		}
		else if (Orion::Input::IsMouseButtonReleased(ORI_MOUSE_BUTTON_RIGHT))
		{
			app = &Orion::Application::Get();
			app->GetWindow().DisableCursor(false);
			firstClick = true;
		}

	}

	void PerspectiveCamera::UpdateCameraDisplacement(Orion::Timestep ts)
	{
		
		if (Orion::Input::IsKeyPressed(ORI_KEY_W))
			m_Position += ts * m_CameraTranslationSpeed * m_CameraSpaceAxisZ;

		if (Orion::Input::IsKeyPressed(ORI_KEY_S))
			m_Position -= ts * m_CameraTranslationSpeed * m_CameraSpaceAxisZ;

		if (Orion::Input::IsKeyPressed(ORI_KEY_A))
			m_Position -= glm::normalize(glm::cross(m_CameraSpaceAxisZ, m_CameraSpaceAxisY)) * (m_CameraTranslationSpeed * ts);

		if (Orion::Input::IsKeyPressed(ORI_KEY_D))
			m_Position += glm::normalize(glm::cross(m_CameraSpaceAxisZ, m_CameraSpaceAxisY)) * (m_CameraTranslationSpeed * ts);


		if (Orion::Input::IsKeyPressed(ORI_KEY_SPACE))
		{
			m_Position += ts * m_CameraTranslationSpeed * m_WorldSpaceAxisY;
		}
		else if (Orion::Input::IsKeyPressed(ORI_KEY_LEFT_CONTROL))
		{
			m_Position -= ts * m_CameraTranslationSpeed * m_WorldSpaceAxisY;
		}



		static bool Initial = true;
		static float InitialSpeed;
		if (Orion::Input::IsKeyPressed(ORI_KEY_LEFT_SHIFT))
		{
			if (Initial) 
			{
				InitialSpeed = m_CameraTranslationSpeed;
				Initial = false;
			}
			m_CameraTranslationSpeed =  (InitialSpeed * ts) + 5;
		}
		else
		{
			if (!Initial) 
			{
				Initial = true;
				m_CameraTranslationSpeed = InitialSpeed;
			}
		}
	}

	void PerspectiveCamera::RecalculateViewProjection()
	{
		RecalculateProjection();
		RecalculateView();
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	inline void PerspectiveCamera::RecalculateView()
	{
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_CameraSpaceAxisZ, m_CameraSpaceAxisY);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	inline void PerspectiveCamera::RecalculateProjection()
	{
		m_AspectRatio = m_ScreenSize.x / m_ScreenSize.y;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOVdeg), (float)m_AspectRatio, 0.1f, 1000.0f);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix; 
	}


}