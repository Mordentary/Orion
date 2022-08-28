#pragma once

#include"Orion/Core/Window.h"
#include"Orion/Renderer/GraphicsRendering/GraphicsContext.h"


#include <GLFW/glfw3.h>

namespace Orion {


	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate()  override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		 void DisableCursor(bool IsDisabled) override 
		{ 
			if (IsDisabled) 
			{
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			}
			else
			{
				glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

			}
		}
		inline void SetCursorPosition(float xPos, float yPos) override { glfwSetCursorPos(m_Window, xPos, yPos); }

		inline int32_t GetCurrentTimeInSec_Int() override
		{
			return static_cast<int32_t>(glfwGetTime());
		}
		inline float GetCurrentTimeInSec_Float() override
		{
			return static_cast<float>(glfwGetTime());
		}
		
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }
	
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;


		struct WindowData 
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;

	};
}

