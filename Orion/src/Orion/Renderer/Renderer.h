#pragma once

#include"RenderCommand.h"
#include"OrthographicCamera.h"
#include"Shader.h"

namespace Orion {




	class Renderer
	{
	public:

		static void BeginScene(const Ref<OrthographicCamera>& m_Camera);
		static void EndScene();
		static void Submit(const Ref<Shader>& shader, const  Ref<VertexArray>& vertexArray, const glm::mat4& modelMatrix);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};
		static Scope<SceneData> s_SceneData;
	};
}

