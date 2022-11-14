#pragma once

#include"oripch.h"

#include "Orion/Core/Core.h"
#include "Orion/Events/Event.h"

namespace Orion
{
	struct WindowProps 
	{
		std::string Title;
		uint32_t SamplesPerPixel;
		uint32_t Width, Height;


		WindowProps(
			const std::string& title = "Orion Engine",
			uint32_t samples = 1,
			unsigned int width = 1280,
			unsigned int height = 720) : Title(title), SamplesPerPixel(samples), Width(width), Height(height)
		{
		}
	};
	struct SubWindow
	{
		std::string Title;
		int32_t Width, Height;
		glm::vec2 ScreenPosInAbs{};
		glm::vec2 MousePosInAbs{};
	};
	class ORION_API Window 
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;


		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetSamplesNum() const = 0;
		virtual const SubWindow& GetSubWindowProp() = 0;

		//Window attributes
		virtual void DisableCursor(bool IsHidden) = 0;
		virtual void SetCursorPosition(float xPos, float yPos) = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;
		virtual void SetSubWindowProp(const SubWindow& window) = 0;

		virtual int32_t GetCurrentTimeInSec_Int() = 0;
		virtual float GetCurrentTimeInSec_Float() = 0;
	
		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());


	};
}