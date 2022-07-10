#include"oripch.h"

#include"OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Orion
{
	
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		ORI_CORE_ASSERT(windowHandle, "Window handle is null!");
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		ORI_CORE_ASSERT(status, "Failder to initialize Glad!");

		
		ORI_CORE_INFO("Renderer {0}", glGetString(GL_RENDERER)); 
		ORI_CORE_INFO("Vendor {0}", glGetString(GL_VENDOR));
		ORI_CORE_INFO("Version {0}", glGetString(GL_VERSION));

	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}