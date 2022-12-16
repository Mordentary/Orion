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

	void OrthographicCamera::UpdateFrustum()
	{
		
		// Calculate the inverse of the projection-view matrix
		glm::mat4 invProjView = glm::inverse(m_ProjectionViewMatrix);

		std::array<glm::vec3, 8> corners;
		// Calculate the corner points by transforming the eight points (-1, -1, -1), (1, -1, -1), (1, 1, -1), (-1, 1, -1),
		// (-1, -1, 1), (1, -1, 1), (1, 1, 1), and (-1, 1, 1) through the inverse projection-view matrix


		corners[0] = glm::vec3(invProjView * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
		corners[1] = glm::vec3(invProjView * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
		corners[2] = glm::vec3(invProjView * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
		corners[3] = glm::vec3(invProjView * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));


		corners[4] = glm::vec3(invProjView * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
		corners[5] = glm::vec3(invProjView * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
		corners[6] = glm::vec3(invProjView * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		corners[7] = glm::vec3(invProjView * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));

		m_Frustum.Top.Normal = glm::normalize(glm::cross(corners[0] - corners[1], corners[4] - corners[0]));
		m_Frustum.Top.Point = corners[2]; // top

		m_Frustum.Bottom.Normal = glm::normalize(glm::cross(corners[6] - corners[2], corners[7] - corners[6]));
		m_Frustum.Bottom.Point = corners[0]; // bottom

		m_Frustum.Right.Normal = glm::normalize(glm::cross(corners[0] - corners[4], corners[3] - corners[0]));
		m_Frustum.Right.Point = corners[2]; // right

		m_Frustum.Left.Normal = -glm::normalize(glm::cross(corners[1] - corners[5], corners[2] - corners[5]));
		m_Frustum.Left.Point = corners[0]; // left

		m_Frustum.Near.Normal = glm::cross(m_Frustum.Left.Normal, m_Frustum.Top.Normal);
		m_Frustum.Near.Point = corners[0]; // near

		m_Frustum.Far.Normal = -glm::cross(m_Frustum.Left.Normal, m_Frustum.Top.Normal);
		m_Frustum.Far.Point = corners[4]; // far
	

	}
	void OrthographicCamera::RenderFrustum(const FrustumShape& frustum, const glm::mat4& projView)

	{
		glm::mat4 invProjView = glm::inverse(projView);

		std::array<glm::vec3, 8> corners;
		// Calculate the corner points by transforming the eight points (-1, -1, -1), (1, -1, -1), (1, 1, -1), (-1, 1, -1),
		// (-1, -1, 1), (1, -1, 1), (1, 1, 1), and (-1, 1, 1) through the inverse projection-view matrix
		corners[0] = glm::vec3(invProjView * glm::vec4(-1.0f, -1.0f, -1.0f, 1.0f));
		corners[1] = glm::vec3(invProjView * glm::vec4(1.0f, -1.0f, -1.0f, 1.0f));
		corners[2] = glm::vec3(invProjView * glm::vec4(1.0f, 1.0f, -1.0f, 1.0f));
		corners[3] = glm::vec3(invProjView * glm::vec4(-1.0f, 1.0f, -1.0f, 1.0f));
		corners[4] = glm::vec3(invProjView * glm::vec4(-1.0f, -1.0f, 1.0f, 1.0f));
		corners[5] = glm::vec3(invProjView * glm::vec4(1.0f, -1.0f, 1.0f, 1.0f));
		corners[6] = glm::vec3(invProjView * glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		corners[7] = glm::vec3(invProjView * glm::vec4(-1.0f, 1.0f, 1.0f, 1.0f));


		//Draw lines
		glm::vec4 color{ 0.7f,0.7f,0.3f,1.0f };

		// Connect the near plane corners
		Orion::Renderer2D::AddLine(corners[0], corners[1], color);
		Orion::Renderer2D::AddLine(corners[1], corners[2], color);
		Orion::Renderer2D::AddLine(corners[2], corners[3], color);
		Orion::Renderer2D::AddLine(corners[3], corners[0], color);

		// Connect the far plane corners
		Orion::Renderer2D::AddLine(corners[4], corners[5], color);
		Orion::Renderer2D::AddLine(corners[5], corners[6], color);
		Orion::Renderer2D::AddLine(corners[6], corners[7], color);
		Orion::Renderer2D::AddLine(corners[7], corners[4], color);


		// Connect the near and far planes
		Orion::Renderer2D::AddLine(corners[0], corners[4], color);
		Orion::Renderer2D::AddLine(corners[1], corners[5], color);
		Orion::Renderer2D::AddLine(corners[2], corners[6], color);
		Orion::Renderer2D::AddLine(corners[3], corners[7], color);

		// color = { 0.2f,0.2f,0.7f,1.0f };
		 //Draw normals
		/*Orion::Renderer2D::AddLine(m_Position, m_Position + m_Frustum.Bottom.Normal, color);
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_Frustum.Top.Normal, color);
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_Frustum.Right.Normal, color);
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_Frustum.Left.Normal, color);
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_Frustum.Near.Normal, color);
		Orion::Renderer2D::AddLine(m_Position, m_Position + m_Frustum.Far.Normal, color);*/


	}

	void OrthographicCamera::RenderFrustum()
	{
		RenderFrustum(m_Frustum, m_ProjectionViewMatrix);
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

