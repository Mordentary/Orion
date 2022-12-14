#include "oripch.h"
#include "OrthographicCamera.h"
#include"Orion/Core/MathHelper.h"
#include "Orion/Core/Input.h"
#include "Orion/Core/KeyCodes.h"
#include"Orion/Renderer/GraphicsRendering/Renderer2D.h"


namespace Orion
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top, bool rotation)
		: DummyCamera(glm::vec3(0.0f), glm::mat4(1.0f), (glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), rotation)
	{
	}

	OrthographicCamera::OrthographicCamera(glm::vec3& position, glm::vec3& dir, glm::vec4& borders, glm::vec2& nearFarPlanes)
		: DummyCamera(position, glm::lookAt(position, position + glm::normalize(glm::vec3(dir.x, dir.y, dir.z)), glm::vec3(0.0f, 1.0f, 0.0f)), (glm::ortho(borders.x, borders.y, borders.z, borders.w, nearFarPlanes.x, nearFarPlanes.y)), false), m_Borders(borders), m_NearFar(nearFarPlanes)
	{
		m_CameraForward = glm::normalize(glm::vec3(dir.x, dir.y, dir.z));
	}


	OrthographicCamera::OrthographicCamera(glm::vec3& position, bool rotation, bool pixelByPixel)
		: DummyCamera(position, glm::mat4(1.0f), glm::mat4(1.0f), rotation), m_ProjMatchesPixelByPixel(pixelByPixel)
	{
		RecalculateProjection();

	}
	

	void OrthographicCamera::Update(Orion::Timestep ts)
	{

		glm::vec3 velocity(0.0f);
		if (Input::IsKeyPressed(ORI_KEY_A))
			velocity.x -= GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_D))
			velocity.x += GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_W))
			velocity.y += GetTranslationSpeed() * ts;
		if (Input::IsKeyPressed(ORI_KEY_S))
			velocity.y -= GetTranslationSpeed() * ts;


		if (IsRotation())
		{
			if (Input::IsKeyPressed(ORI_KEY_Q))
				m_Rotation += GetSensitivity() * ts;
			if (Input::IsKeyPressed(ORI_KEY_E))
				m_Rotation -= GetSensitivity() * ts;
		}
		if (velocity != glm::vec3(0.0f)) {

			m_Position += velocity * (m_ZoomLevel * 5);
			RecalculateView();
		}

		
		UpdateFrustum();


	}
	
	void OrthographicCamera::RecalculateViewProjection()
	{
		RecalculateView();
		RecalculateProjection();
	}

	inline void OrthographicCamera::RecalculateView()
	{
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_CameraRotation), glm::vec3(0, 0, 1));
		m_CameraForward = glm::vec3(transform[0][2], transform[1][2], transform[2][2]);
		m_ViewMatrix = glm::inverse(transform);
		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
	struct Plane
	{
		// unit vector
		glm::vec3 Normal{};
		// point on plane
		glm::vec3 Point{};

		float Distance(const glm::vec3& p)
		{
			return glm::dot(p - Point, Normal);
		}
	};
	struct CameraFrustum
	{
		Plane Top;
		Plane Bottom;

		Plane Right;
		Plane Left;

		Plane Far;
		Plane Near;
	};
	void OrthographicCamera::UpdateFrustum()
	{
		

		// Create a new frustum using the updated view-projection matrix
		CameraFrustum frustum{};

		// Calculate the planes of the frustum using the view-projection matrix
		glm::vec3 row1 = m_ProjectionViewMatrix[0];
		glm::vec3 row2 = m_ProjectionViewMatrix[1];
		glm::vec3 row3 = m_ProjectionViewMatrix[2];
		glm::vec3 row4 = m_ProjectionViewMatrix[3];

		// Calculate the normal vector and point on the near plane
		glm::vec3 nearNormal = row3;
		glm::vec3 nearPoint = row4 + row3;
		m_Frustum.Near = Plane{ glm::normalize(nearNormal), nearPoint };

		// Calculate the normal vector and point on the far plane
		glm::vec3 farNormal = -row3;
		glm::vec3 farPoint = row4 - row3;
		m_Frustum.Far = Plane{ glm::normalize(farNormal), farPoint };

		// Calculate the normal vector and point on the top plane
		glm::vec3 topNormal = row4 + row1;
		glm::vec3 topPoint = row4 - row1;
		m_Frustum.Top = Plane{ glm::normalize(topNormal), topPoint };

		// Calculate the normal vector and point on the bottom plane
		glm::vec3 bottomNormal = row4 - row1;
		glm::vec3 bottomPoint = row4 + row1;
		m_Frustum.Bottom = Plane{ glm::normalize(bottomNormal), bottomPoint };

		// Calculate the normal vector and point on the right plane
		glm::vec3 rightNormal = row4 - row2;
		glm::vec3 rightPoint = row4 + row2;
		m_Frustum.Right = Plane{ glm::normalize(rightNormal), rightPoint };

		// Calculate the normal vector and point on the left plane
		glm::vec3 leftNormal = row4 + row2;
		glm::vec3 leftPoint = row4 - row2;
		m_Frustum.Left = Plane{ glm::normalize(leftNormal), leftPoint };

	

	}
	void OrthographicCamera::RenderFrustum()
	{
		// Compute the center of the frustum
		glm::vec3 center = (m_Frustum.Right.Point + m_Frustum.Left.Point + m_Frustum.Top.Point + m_Frustum.Bottom.Point + m_Frustum.Near.Point + m_Frustum.Far.Point) / 6.0f;

		// Compute the distances from the center to each of the frustum planes
		float dRight = glm::length(m_Frustum.Right.Point - center);
		float dLeft = glm::length(m_Frustum.Left.Point - center);
		float dTop = glm::length(m_Frustum.Top.Point - center);
		float dBottom = glm::length(m_Frustum.Bottom.Point - center);
		float dNear = glm::length(m_Frustum.Near.Point - center);
		float dFar = glm::length(m_Frustum.Far.Point - center);

		// Compute the points on the right, left, top, and bottom frustum planes that are closest to the center
		glm::vec3 right = center + (m_Frustum.Right.Normal * dRight);
		glm::vec3 left = center + (m_Frustum.Left.Normal * dLeft);

		// Compute the top and bottom vectors using the dNear and dFar values
		glm::vec3 top = center + (m_Frustum.Top.Normal * dNear);
		glm::vec3 bottom = center + (m_Frustum.Bottom.Normal * dFar);

		// Compute the corners of the frustum
		glm::vec3 nearTopRight = top + (m_Frustum.Near.Normal * dNear);
		glm::vec3 nearTopLeft = nearTopRight + (m_Frustum.Left.Normal * dLeft);
		glm::vec3 nearBottomRight = bottom + (m_Frustum.Near.Normal * dNear);
		glm::vec3 nearBottomLeft = nearBottomRight + (m_Frustum.Left.Normal * dLeft);

		glm::vec3 farTopRight = top + (m_Frustum.Far.Normal * dFar);
		glm::vec3 farTopLeft = farTopRight + (m_Frustum.Left.Normal * dLeft);
		glm::vec3 farBottomRight = bottom + (m_Frustum.Far.Normal * dFar);
		glm::vec3 farBottomLeft = farBottomRight + (m_Frustum.Left.Normal * dLeft);




		//Draw lines
		glm::vec4 color{ 0.9f,0.f,0.2f,1.0f };

		// Connect the near plane corners
		Orion::Renderer2D::AddLine(nearTopRight, nearTopLeft, color);
		Orion::Renderer2D::AddLine(nearTopLeft, nearBottomLeft, color);
		Orion::Renderer2D::AddLine(nearBottomLeft, nearBottomRight, color);
		Orion::Renderer2D::AddLine(nearBottomRight, nearTopRight, color);

		// Connect the far plane corners
		Orion::Renderer2D::AddLine(farTopRight, farTopLeft, color);
		Orion::Renderer2D::AddLine(farTopLeft, farBottomLeft, color);
		Orion::Renderer2D::AddLine(farBottomLeft, farBottomRight, color);
		Orion::Renderer2D::AddLine(farBottomRight, farTopRight, color);

		// Connect the near and far planes
		Orion::Renderer2D::AddLine(nearTopRight, farTopRight, color);
		Orion::Renderer2D::AddLine(nearTopLeft, farTopLeft, color);
		Orion::Renderer2D::AddLine(nearBottomLeft, farBottomLeft, color);
		Orion::Renderer2D::AddLine(nearBottomRight, farBottomRight, color);


	}

	inline void OrthographicCamera::RecalculateProjection()
	{
		if (m_ProjMatchesPixelByPixel)
		{

			m_ProjectionMatrix = glm::ortho(0.f, m_ScreenSize.x, 0.f, m_ScreenSize.y, -1.0f, 1.0f);
			m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
			return;
		}


		m_AspectRatio = m_ScreenSize.x / m_ScreenSize.y;

		m_ProjectionMatrix = glm::ortho(-m_ZoomLevel * m_AspectRatio, m_ZoomLevel * m_AspectRatio, -m_ZoomLevel, m_ZoomLevel, -10.0f, 10.0f);


		//ORI_CORE_INFO("AR: {0}", m_AspectRatio);

		m_ProjectionViewMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	
}

