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

		inline uint32_t GetHeight() const override { return m_Data.Height; }
		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetSamplesNum() const override { return m_Data.SamplesPerPixel; }
		inline const SubWindow& GetSubWindowProp() override {	return m_ImGuiViewport;}

		inline int32_t GetCurrentTimeInSec_Int() override {return static_cast<int32_t>(glfwGetTime());}
		inline float GetCurrentTimeInSec_Float() override {return static_cast<float>(glfwGetTime());}

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
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }


		inline void SetSubWindowProp(const SubWindow& win) override { m_ImGuiViewport = win; }


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
			uint32_t SamplesPerPixel;
			uint32_t Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	private:
		SubWindow m_ImGuiViewport;
	
	};
}

