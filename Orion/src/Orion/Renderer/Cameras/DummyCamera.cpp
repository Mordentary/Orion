#include "oripch.h"
#include "DummyCamera.h"
#include"Orion/Core/Application.h"
#include"Orion/Renderer/GraphicsRendering/Renderer2D.h"
#include"Orion/Core/Input.h"
#include"Orion/Core/KeyCodes.h"
#include"Orion/Core/MouseButtonCodes.h"
#include "Orion/Renderer/GraphicsObjects/Model.h"
#include "Orion/Core/AdvanceCamerasFamily/CamerasController.h"

#include <glm/gtx/string_cast.hpp>


namespace Orion 
{
	DummyCamera::DummyCamera(
		const glm::vec3& position,
		const glm::mat4& viewMatrix,
		const glm::mat4& projectionMatrix,
		bool rotation
		)
		:
		m_Position(position),
		m_ViewMatrix(viewMatrix),
		m_ProjectionMatrix(projectionMatrix),
		m_ProjectionViewMatrix(m_ViewMatrix * m_ProjectionMatrix),
		m_Rotation(rotation)
	{
		auto& app = Orion::Application::Get();
		m_ScreenSize.x = app.GetWindow().GetWidth();
		m_ScreenSize.y = app.GetWindow().GetHeight();
		m_AspectRatio = m_ScreenSize.x / m_ScreenSize.y;

	}

	const CameraRay& DummyCamera::Raycast(float xCoord, float yCoord)
	{
		float mouse_x = xCoord;
		float mouse_y = yCoord;

		//ORI_INFO("X: {0}, Y: {1}", mouse_x, mouse_y);
		float x = (2.0f * mouse_x) / m_ScreenSize.x - 1.0f;
		float y = 1.0f - (2.0f * mouse_y) / m_ScreenSize.y;
		float z = 1.0f;
		glm::vec3 ray_nds = glm::vec3(x, y, z);
		glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0, 1.0);
		glm::vec4 ray_eye = glm::inverse(m_ProjectionMatrix) * ray_clip;
		glm::vec3 ray_wor = glm::vec3((inverse(m_ViewMatrix) * glm::vec4(ray_eye.x, -ray_eye.y, -1.0, 0.0f)));
		//std::cout << glm::to_string(ray_wor) << "\n";

		m_Ray.Update(glm::normalize(ray_wor),m_Position,10.f);

		return m_Ray;
	}

	void DummyCamera::DragObjectAlongCameraPlane(Shared<Model>&model)
	{
		if (Orion::Input::IsMouseButtonPressed(ORI_MOUSE_BUTTON_1) && !Orion::CamerasController::GetBlockUpdate())
		{	


			float t = PlaneVsRay(Plane{ -m_CameraForward, model->GetPosition() }, m_Ray);

			static glm::vec3 pointIntersection{};
			if (t != -1) 
			{
				pointIntersection = m_Ray.GetOrigin() + (m_Ray.GetDirection() * t);
			}

			model->SetPosition(pointIntersection);

		}
	}

	float DummyCamera::PlaneVsRay(const Plane& pl, const CameraRay& ray)
	{
		float denom = glm::dot(pl.Normal, ray.GetDirection());

		if (abs(denom) > 1e-6)
		{
			glm::vec3 p0l0 = pl.Point - ray.GetOrigin();
			float t = glm::dot(p0l0, pl.Normal) / denom;
			if (t >= 0)
			{
				return t;
			}
		}

		return -1.f;
	}


	bool DummyCamera::AABBVsFrustum(const glm::vec3& min, const glm::vec3& max)
	{
		Plane* currentPlane = &m_Frustum.Top;
		bool ret = true;
		glm::vec3 vmin, vmax;

		for (int i = 0; i < 6; ++i) 
		{
			// X axis 
			if (currentPlane->Normal.x >= 0) {
				vmin.x = min.x;
				vmax.x = max.x;
			}
			else {
				vmin.x = max.x;
				vmax.x = min.x;
			}
			// Y axis 
			if (currentPlane->Normal.y >= 0) {
				vmin.y = min.y;
				vmax.y = max.y;
			}
			else {
				vmin.y = max.y;
				vmax.y = min.y;
			}
			// Z axis 
			if (currentPlane->Normal.z >= 0) {
				vmin.z = min.z;
				vmax.z = max.z;
			}
			else {
				vmin.z = max.z;
				vmax.z = min.z;
			}
			if (glm::dot(currentPlane->Normal, vmin) + glm::distance(currentPlane->Point, glm::vec3(0.f)) > 0)
				return false;
			if (glm::dot(currentPlane->Normal, vmax) + glm::distance(currentPlane->Point, glm::vec3(0.f)) >= 0)
				ret = true;

			++currentPlane;
		}
		return ret;
	}


	bool DummyCamera::PlaneVsAABB(const DummyCamera::Plane& pl, const glm::vec3& min, const glm::vec3& max)
	{
		// Convert AABB to center-extents representation
		glm::vec3 center = (min + max) * 0.5f; // Compute AABB center
		glm::vec3 extent = max - center; // Compute positive extents

		// Compute the projection interval radius of b onto L(t) = b.center + t * p.n
		float r = extent[0] * glm::abs(pl.Normal.x) + extent[1] * glm::abs(pl.Normal.y) + extent[2] * glm::abs(pl.Normal.z);

		// Compute distance of box center from plane
		float s = glm::dot(pl.Normal, center - pl.Point);

		// Intersection occurs when distance s falls within [-r,+r] interval
		return glm::abs(s) <= r;
	}

	CameraRay::CameraRay(glm::vec3 direction, glm::vec3 origin, float rayLength)
		: m_Direction(direction), m_Origin(origin), m_Length(rayLength)
	{
		
	}
	void CameraRay::Update(glm::vec3 direction, glm::vec3 origin, float rayLength)
	{
		m_Origin = origin;
		m_Direction = direction;
		m_Length = rayLength;
		


	}
	void CameraRay::DebugDraw() const
	{
		static glm::vec3 p1{ 0.0f };
		static glm::vec3 p2{ 0.0f };


		if(Orion::Input::IsKeyPressed(ORI_KEY_R))
		{
			p1 = GetOrigin();
			p2 = GetEndPoint();

		}
		Orion::Renderer2D::AddLine(p1, p2, glm::vec4(0.7f,0.2f,0.2f,1.0f));
	}

	glm::vec3 CameraRay::GetEndPoint() const
	{
		return m_Origin + (m_Direction * m_Length);
	}


}