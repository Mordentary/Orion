#pragma once



namespace Orion 
{
	class Input
	{
	public:
		 static bool IsKeyPressed(int keycode);
		 static bool IsMouseButtonPressed(int keycode);

		 static bool IsKeyReleased(int keycode);
		 static bool IsMouseButtonReleased(int keycode);

		 static std::pair<float, float> GetMousePosition();
		 static float GetMouseX();
		 static float GetMouseY();

		 static std::pair<float, float> Input::GetLocalWindowMousePosition();
		 static float GetLocalWindowMouseX();
		 static float GetLocalWindowMouseY();

	};
}
