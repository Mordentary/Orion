#include "oripch.h"
#include"CollisionHandler.h"
#include"Orion/Renderer/GraphicsRendering/Renderer2D.h"
#include"Orion/Core/Input.h"
#include"Orion/Core/KeyCodes.h"



namespace Orion
{
	float CollisionHandler::PlaneVsRay(const Plane& pl, const Ray& ray)
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
	bool CollisionHandler::PointVsFrustum(const FrustumShape& frustum, const glm::vec3& point)
	{

		const Plane* currentPlane = &frustum.Top;
		for (int i = 0; i < 6; ++i)
		{
			if (currentPlane->Distance(point) <= 0)
				return false;

			++currentPlane;
		}
		return true;
	}


	bool CollisionHandler::AABBVsFrustum(const FrustumShape& frustum, const glm::vec3& min, const glm::vec3& max)
	{
		const Plane* currentPlane = &frustum.Top;

		for (int i = 0; i < 6; ++i)
		{
			glm::vec3 normal = currentPlane->Normal;
			glm::vec3 p = min;
			glm::vec3 n = max;

			if (normal.x >= 0)
			{
				p.x = max.x;
				n.x = min.x;
			}
			if (normal.y >= 0)
			{
				p.y = max.y;
				n.y = min.y;
			}
			if (normal.z >= 0)
			{
				p.z = max.z;
				n.z = min.z;
			}


			if (currentPlane->Distance(p) <= 0 && currentPlane->Distance(n) <= 0)
				return false;

			++currentPlane;
		}
		return true;
	}

	//Ray class
	Ray::Ray(glm::vec3 direction, glm::vec3 origin, float rayLength)
		: m_Direction(direction), m_Origin(origin), m_Length(rayLength)
	{

	}


	void Ray::Update(glm::vec3 direction, glm::vec3 origin, float rayLength)
	{
		m_Origin = origin;
		m_Direction = direction;
		m_Length = rayLength;



	}
	void Ray::DebugDraw() const
	{
		static glm::vec3 p1{ 0.0f };
		static glm::vec3 p2{ 0.0f };


		if (Orion::Input::IsKeyPressed(ORI_KEY_R))
		{
			p1 = GetOrigin();
			p2 = GetEndPoint();

		}
		Orion::Renderer2D::AddLine(p1, p2, glm::vec4(0.7f, 0.2f, 0.2f, 1.0f));
	}

	glm::vec3 Ray::GetEndPoint() const
	{
		return m_Origin + (m_Direction * m_Length);
	}

}



