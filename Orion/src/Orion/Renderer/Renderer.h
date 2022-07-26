#pragma once

#include"RenderCommand.h"
#include"OrthographicCamera.h"
#include"Shader.h"
namespace Orion {




	class Renderer
	{
	public:
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void Init();

		static void BeginScene(const Shared<OrthographicCamera>& m_Camera);
		static void EndScene();
		static void Submit(const Shared<Shader>& shader, const  Shared<VertexArray>& vertexArray, const glm::mat4& modelMatrix);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Scoped<SceneData> s_SceneData;
	};
}

