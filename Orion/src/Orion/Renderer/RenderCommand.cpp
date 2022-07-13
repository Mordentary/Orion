#include"oripch.h"
#include"RenderCommand.h"


#include"Platform/OpenGL/OpenGLRendererAPI.h"
namespace Orion
{	
	Scope<RendererAPI> RenderCommand::s_RendererAPI = std::make_unique<OpenGLRendererAPI>();;

}