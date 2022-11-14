#include "oripch.h"
#include "Orion/Core/Input.h"

#include "GLFW/glfw3.h"
#include"Orion/Core/Application.h"
namespace Orion {

	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);
		
		return state == GLFW_PRESS || state == GLFW_REPEAT;

	}
	bool Input::IsMouseButtonPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, keycode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsKeyReleased(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keycode);

		return state == GLFW_RELEASE;
	}
	bool Input::IsMouseButtonReleased(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, keycode);

		return state == GLFW_RELEASE;
	}
	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);

		return {xPos,yPos};
	}
	float Input::GetMouseX()
	{
		auto [x, y] = GetMousePosition();

		return (float)x;
	}
	float Input::GetMouseY()
	{
		auto [x, y] = GetMousePosition();

		return (float)y;
	}


	std::pair<float, float> Input::GetLocalWindowMousePosition()
	{
		
		auto& window = Application::Get().GetWindow();
		auto VP_Prop = window.GetSubWindowProp();

		glm::vec2 mouseA = VP_Prop.MousePosInAbs;
		glm::vec2 winA = VP_Prop.ScreenPosInAbs;

		glm::vec2 size = { VP_Prop.Width,VP_Prop.Height };


		mouseA.y = size.y - (mouseA.y - winA.y);
		mouseA.x = (mouseA.x - winA.x);

		if (mouseA.y > size.y) mouseA.y = size.y;
		if (mouseA.y < 0) mouseA.y = 0;

		if (mouseA.x > size.x) mouseA.x = size.x;
		if (mouseA.x < 0) mouseA.x = 0;


		return { mouseA.x,mouseA.y };
	}

	float Input::GetLocalWindowMouseX()
	{
		auto [x, y] = GetLocalWindowMousePosition();

		return (float)x;
	}
	float Input::GetLocalWindowMouseY() 
	{
		auto [x, y] = GetLocalWindowMousePosition();

		return (float)y;
	}
}
