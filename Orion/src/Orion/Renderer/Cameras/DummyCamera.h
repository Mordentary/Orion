#pragma once
#include <Orion/Core/TimeHelper.h>

namespace Orion 
{
	enum class Cameras
	{
		Uknown = 0,
		Perspective = 1,
		Orthographic = 2

	};


	class CameraRay
	{
	public:
		
	public:
		CameraRay() = default;
		CameraRay(glm::vec3 direction, glm::vec3 origin, float rayLength);
		void Update(glm::vec3 direction, glm::vec3 origin, float rayLength);
		void DebugDraw() const;


		const glm::vec3& GetDirection() const { return m_Direction; }
		const glm::vec3& GetOrigin() const { return m_Origin; }
		float GetLength() const { return m_Length; }
		glm::vec3 GetEndPoint() const;




		CameraRay& operator=(const CameraRay& r) {
			if (&r != this) {
				m_Origin = r.m_Origin;
				m_Direction = r.m_Direction;
				m_Length = r.m_Length;

			}
			return *this;
		};

	private:
		glm::vec3 m_Origin{1.0f};
		glm::vec3 m_Direction{1.0f};
		glm::vec3 m_LastHitedPoint{};
		float m_Length = 0.0f;

	};


	class Model;


	class DummyCamera
	{
	public:

		DummyCamera(
			const glm::vec3& position,
			const glm::mat4& viewMatrix = glm::mat4(1.0f),
			const glm::mat4& projectionMatrix = glm::mat4(1.0f),
			bool rotation = false
		);
		virtual ~DummyCamera() = default;



		virtual void Update(Orion::Timestep deltaTime) {}
		virtual void RecalculateView() = 0;
		virtual void RecalculateViewProjection() = 0;
		virtual void RecalculateProjection() = 0;

		virtual void RenderFrustum() = 0;
		virtual void UpdateFrustum() = 0;

		void DragObjectAlongCameraPlane(Shared<Model>& model);

		const inline glm::mat4& GetProjectionViewMatrix() const { return m_ProjectionViewMatrix; }
		const inline glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const inline glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		inline const glm::vec3& GetPosition() const { return m_Position; }
		inline const glm::vec3& GetCameraForwardDir() const { return m_CameraForward; }

		virtual const Cameras Get() = 0;


	    inline const float& GetAspectRatio() const { return m_AspectRatio; }
		inline const float& GetZoomLevel() const { return m_ZoomLevel; }

		inline const float& GetSensitivity() const { return m_CameraSensitivity; }
		inline const float& GetTranslationSpeed() { return m_CameraTranslationSpeed; }

		inline const CameraRay& GetCameraRay() { return m_Ray; }


		inline const float IsRotation() const { return m_Rotation; }
		
		inline void OnViewportResize(const glm::vec2& size) { SetScreenSize(size); RecalculateProjection();}
		inline void SetViewMatrix(const glm::mat4& view) { m_ViewMatrix = view; RecalculateProjection(); }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; }
		inline void SetScreenSize(const glm::vec2& size) { m_ScreenSize = size;}
		inline void SetZoomLevel(float zoom) { m_ZoomLevel = zoom;}
		inline void SetAspectRatio(float aspect) { m_AspectRatio = aspect; }
		inline void SetTraslationSpeed(float tranSpeed) { m_CameraTranslationSpeed = tranSpeed; }
		inline void SetSensitivity(float rotSpeed) { m_CameraSensitivity = rotSpeed; }

		const CameraRay& Raycast(float xCoord, float yCoord);
		bool AABBVsFrustum(const glm::vec3& min, const glm::vec3& max);
		bool PointVsFrustum(const glm::vec3& point);

	protected:
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
	protected: 
		float m_AspectRatio = 1.7f;
		float m_ZoomLevel = 1.0f;
		float m_CameraTranslationSpeed = 1.0f, m_CameraSensitivity = 180.0f;
		bool m_Rotation = false;

		glm::vec3 m_Position{};
		glm::vec3 m_CameraForward{};

		glm::mat4 m_ViewMatrix{};
		glm::mat4 m_ProjectionMatrix{};
		glm::mat4 m_ProjectionViewMatrix{};

		CameraRay m_Ray;
		CameraFrustum m_Frustum; 

		glm::vec2 m_ScreenSize{};
	protected:

	private:
		float PlaneVsRay(const DummyCamera::Plane& pl, const CameraRay& ray);

	};

}
