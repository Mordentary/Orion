#pragma once
#include"glm/glm.hpp"

namespace Orion
{
	class Ray
	{
	public:
		Ray() = default;
		Ray(glm::vec3 direction, glm::vec3 origin, float rayLength);
		void Update(glm::vec3 direction, glm::vec3 origin, float rayLength);
		void DebugDraw() const;


		const glm::vec3& GetDirection() const { return m_Direction; }
		const glm::vec3& GetOrigin() const { return m_Origin; }
		float GetLength() const { return m_Length; }
		glm::vec3 GetEndPoint() const;




		Ray& operator=(const Ray& r) {
			if (&r != this) {
				m_Origin = r.m_Origin;
				m_Direction = r.m_Direction;
				m_Length = r.m_Length;

			}
			return *this;
		};

	private:
		glm::vec3 m_Origin{ 1.0f };
		glm::vec3 m_Direction{ 1.0f };
		glm::vec3 m_LastHitedPoint{};
		float m_Length = 0.0f;

	};

	struct Plane
	{
		// unit vector
		glm::vec3 Normal{};
		// point on plane
		glm::vec3 Point{};

		float Distance(const glm::vec3& p) const 
		{
			return glm::dot(p - Point, Normal);
		}
	};

	struct FrustumShape
	{
		Plane Top;
		Plane Bottom;

		Plane Right;
		Plane Left;

		Plane Far;
		Plane Near;

	};

	class CollisionHandler
	{
	public:
		static float PlaneVsRay(const Plane& pl, const Ray& ray);
		static bool AABBVsFrustum(const FrustumShape& frustum, const glm::vec3& min, const glm::vec3& max);
		static bool PointVsFrustum(const FrustumShape& frustum, const glm::vec3& point);
	};
}
