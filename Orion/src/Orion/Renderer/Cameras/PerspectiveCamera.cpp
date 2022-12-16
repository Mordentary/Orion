#include "oripch.h"
#include "PerspectiveCamera.h"
#include"Orion/Core/Application.h"
#include"Orion/Core/Input.h"
#include"Orion/Core/KeyCodes.h"
#include"Orion/Core/MouseButtonCodes.h"
#include"Orion/Renderer/GraphicsRendering/Renderer2D.h"

#include<glm/gtc/matrix_transform.hpp>
#include<glm/glm.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>
namespace Orion
{
	PerspectiveCamera::PerspectiveCamera(const glm::vec3& position, const glm::vec3& cameraTarget, float FOV, const glm::vec2& nearFar) : DummyCamera(position), m_FOVdeg(FOV), m_NearFar(nearFar)
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

		UpdateFrustum();
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


			/*if (rotY > 89.0f)
				rotY = 89.0f;
			if (rotY < -89.0f)
				rotY = -89.0f;*/
			glm::vec3 newOrientation = glm::rotate(m_CameraSpaceAxisZ, glm::radians(-rotX), glm::normalize(glm::cross(m_CameraSpaceAxisZ, m_WorldSpaceAxisY)));

			if (abs(glm::angle(newOrientation, m_WorldSpaceAxisY) - glm::radians(90.0f)) <= glm::radians(85.0f))
			{
				m_CameraSpaceAxisZ = newOrientation;
			}
			
			m_CameraSpaceAxisZ = glm::normalize(glm::rotate(m_CameraSpaceAxisZ, glm::radians(-rotY), m_WorldSpaceAxisY));

			m_CameraSpaceAxisX = glm::normalize(glm::cross(m_CameraSpaceAxisZ, m_WorldSpaceAxisY));
			m_CameraSpaceAxisY = -glm::cross(m_CameraSpaceAxisZ, m_CameraSpaceAxisX);

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
			m_CameraTranslationSpeed = (InitialSpeed * ts) + 5;
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




	void PerspectiveCamera::UpdateFrustum()
	{

		//Old code
		/*	float halfHeightNear = tan(m_FOVdeg / 2) * m_NearFar.x;
		float halfWidthNear = halfHeightNear * m_AspectRatio;

		float halfHeightFar = tanf(m_FOVdeg / 2) * m_NearFar.y;
		float halfWidthFar = halfHeightFar * m_AspectRatio;

		glm::vec3 nearPointCenter = m_Position + m_NearFar.x * m_CameraForward;
		glm::vec3 farPointCenter = m_Position + m_NearFar.y * m_CameraForward;

		FrustumShape frustum;


		frustum.Near = { m_CameraForward ,nearPointCenter };
		frustum.Far = { -m_CameraForward ,farPointCenter };

		glm::vec3 rightBorder = (nearPointCenter + m_CameraSpaceAxisX * halfWidthNear) - m_Position;
		glm::vec3 normalRight = glm::cross(m_CameraSpaceAxisY, glm::normalize(rightBorder));

		frustum.Right = { normalRight, m_Position };

		glm::vec3 leftBorder = (nearPointCenter - m_CameraSpaceAxisX * halfWidthNear) - m_Position;
		glm::vec3 normalLeft = -glm::cross(m_CameraSpaceAxisY, glm::normalize(leftBorder));

		frustum.Left = { normalLeft ,m_Position };


		glm::vec3 topBorder = (nearPointCenter + m_CameraSpaceAxisY * halfHeightNear) - m_Position;
		glm::vec3 normalTop = -glm::cross(m_CameraSpaceAxisX, glm::normalize(topBorder));

		frustum.Top = { normalTop ,m_Position };


		glm::vec3 bottomBorder = (nearPointCenter - m_CameraSpaceAxisY * halfHeightNear) - m_Position;
		glm::vec3 normalBottom = glm::cross(m_CameraSpaceAxisX, glm::normalize(bottomBorder));

		frustum.Bottom = { normalBottom ,m_Position };*/

		// Pre-compute some values
		float tanFov = tan(m_FOVdeg / 2);
		float halfHeightNear = tanFov * m_NearFar.x;
		float halfWidthNear = halfHeightNear * m_AspectRatio;
		float halfHeightFar = tanFov * m_NearFar.y;
		float halfWidthFar = halfHeightFar * m_AspectRatio;

		glm::vec3 nearPointCenter = m_Position + m_NearFar.x * m_CameraForward;
		glm::vec3 farPointCenter = m_Position + m_NearFar.y * m_CameraForward;


		// Extract the near and far planes
		m_Frustum.Near = { m_CameraForward ,nearPointCenter };
		m_Frustum.Far = { -m_CameraForward ,farPointCenter };

		// Extract the right plane
		glm::vec3 rightBorder = (nearPointCenter + m_CameraSpaceAxisX * halfWidthNear) - m_Position;
		m_Frustum.Right = { glm::cross(m_CameraSpaceAxisY, glm::normalize(rightBorder)), m_Position };

		// Extract the left plane
		glm::vec3 leftBorder = (nearPointCenter - m_CameraSpaceAxisX * halfWidthNear) - m_Position;
		m_Frustum.Left = { -glm::cross(m_CameraSpaceAxisY, glm::normalize(leftBorder)), m_Position };

		// Extract the top plane
		m_Frustum.Top = { -glm::cross(glm::normalize(rightBorder), m_CameraSpaceAxisY), m_Position };

		// Extract the bottom plane
		glm::vec3 bottomBorder = (nearPointCenter - m_CameraSpaceAxisY * halfHeightNear) - m_Position;
		m_Frustum.Bottom = { -glm::cross(glm::normalize(bottomBorder), m_CameraSpaceAxisX), m_Position };

	}


	static void RenderFrustum(FrustumShape& frustum) 
	{
		//TODO: Implement
	}

	void PerspectiveCamera::RenderFrustum()
	{
		float halfHeightNear = tan(m_FOVdeg / 2) * m_NearFar.x;
		float halfWidthNear = halfHeightNear * m_AspectRatio;

		float halfHeightFar = tanf(m_FOVdeg / 2) * m_NearFar.y;
		float halfWidthFar = halfHeightFar * m_AspectRatio;

		glm::vec3 nearPointCenter = m_Position + m_NearFar.x * m_CameraForward;
		glm::vec3 farPointCenter = m_Position + m_NearFar.y * m_CameraForward;

		glm::vec4 color{ 0.5f,0.7f,0.6f,1.0f };


		glm::vec3 ftl = farPointCenter + (m_CameraSpaceAxisY * halfHeightFar) - (m_CameraSpaceAxisX * halfWidthFar);
		glm::vec3 fbr = farPointCenter - (m_CameraSpaceAxisY * halfHeightFar) + (m_CameraSpaceAxisX * halfWidthFar);

		glm::vec3 ftr = farPointCenter + (m_CameraSpaceAxisY * halfHeightFar) + (m_CameraSpaceAxisX * halfWidthFar);
		glm::vec3 fbl = farPointCenter - (m_CameraSpaceAxisY * halfHeightFar) - (m_CameraSpaceAxisX * halfWidthFar);


		glm::vec3 ntl = nearPointCenter + (m_CameraSpaceAxisY * halfHeightNear) - (m_CameraSpaceAxisX * halfWidthNear);
		glm::vec3 ntr = nearPointCenter + (m_CameraSpaceAxisY * halfHeightNear) + (m_CameraSpaceAxisX * halfWidthNear);

		glm::vec3 nbr = nearPointCenter - (m_CameraSpaceAxisY * halfHeightNear) + (m_CameraSpaceAxisX * halfWidthNear);
		glm::vec3 nbl = nearPointCenter - (m_CameraSpaceAxisY * halfHeightNear) - (m_CameraSpaceAxisX * halfWidthNear);
		
		//NearPlane
		Orion::Renderer2D::AddLine(ntl, ntr, color);
		Orion::Renderer2D::AddLine(ntr, nbr, color);
		Orion::Renderer2D::AddLine(nbr, nbl, color);
		Orion::Renderer2D::AddLine(nbl, ntl , color);

		//FarPlane
		Orion::Renderer2D::AddLine(ftl, ftr, color);
		Orion::Renderer2D::AddLine(ftr, fbr, color);
		Orion::Renderer2D::AddLine(fbr, fbl, color);
		Orion::Renderer2D::AddLine(fbl, ftl, color);

		//Connections
		Orion::Renderer2D::AddLine(ftl, ntl, color);
		Orion::Renderer2D::AddLine(ftr, ntr, color);
		Orion::Renderer2D::AddLine(fbr, nbr, color);
		Orion::Renderer2D::AddLine(fbl, nbl, color);


		//CameraAxis
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_CameraSpaceAxisX , glm::vec4(0.8f,0.5f,0.f,1.0f));
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_CameraSpaceAxisY, glm::vec4(0.8f, 0.5f, 0.f, 1.0f));
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_CameraSpaceAxisZ, glm::vec4(0.8f, 0.5f, 0.f, 1.0f));
		

		glm::vec3 rightBorder = (nearPointCenter + m_CameraSpaceAxisX * halfWidthNear) - m_Position;


		glm::vec3 leftBorder = (nearPointCenter - m_CameraSpaceAxisX * halfWidthNear) - m_Position;



		glm::vec3 topBorder = (nearPointCenter + m_CameraSpaceAxisY * halfHeightNear) - m_Position;



		glm::vec3 bottomBorder = (nearPointCenter - m_CameraSpaceAxisY * halfHeightNear) - m_Position;



		float borderScale = m_NearFar.y / 2.f;
		float normalScale = glm::max(m_NearFar.x * 2.f,1.0f);


		//DrawNormals
		Orion::Renderer2D::AddLine(m_Frustum.Near.Point, m_Frustum.Near.Point + m_Frustum.Near.Normal * normalScale, glm::vec4(0.1f, 0.7f, 0.7f, 1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Far.Point, m_Frustum.Far.Point + m_Frustum.Far.Normal * normalScale, glm::vec4(0.1f, 0.7f, 0.7f, 1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Top.Point + topBorder * (borderScale / 2), m_Frustum.Top.Point + topBorder * (borderScale/2) + m_Frustum.Top.Normal * normalScale, glm::vec4(0.1f, 0.7f, 0.7f, 1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Bottom.Point + bottomBorder * (borderScale / 2), m_Frustum.Bottom.Point + bottomBorder * (borderScale / 2) + m_Frustum.Bottom.Normal * normalScale,  glm::vec4(0.1f, 0.7f, 0.7f, 1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Right.Point + rightBorder * (borderScale / 2), m_Frustum.Right.Point + rightBorder * (borderScale / 2) + m_Frustum.Right.Normal * normalScale, glm::vec4(0.1f, 0.7f, 0.7f, 1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Left.Point + leftBorder * (borderScale / 2), m_Frustum.Left.Point + leftBorder * (borderScale / 2) + m_Frustum.Left.Normal * normalScale,  glm::vec4(0.1f, 0.7f, 0.7f, 1.0f));


		//Borders
		Orion::Renderer2D::AddLine(m_Frustum.Top.Point, m_Frustum.Top.Point + topBorder * borderScale, glm::vec4(0.9f,0.1f,0.1f,1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Bottom.Point, m_Frustum.Bottom.Point + bottomBorder * borderScale, glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Left.Point, m_Frustum.Left.Point + leftBorder * borderScale, glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));
		Orion::Renderer2D::AddLine(m_Frustum.Right.Point, m_Frustum.Right.Point + rightBorder * borderScale, glm::vec4(0.9f, 0.1f, 0.1f, 1.0f));


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
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOVdeg), (float)m_AspectRatio, m_NearFar.x, m_NearFar.y);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}