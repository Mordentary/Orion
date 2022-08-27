#pragma once
#include"Orion/Renderer/GraphicsObjects/Buffer.h"


namespace Orion
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		OpenGLVertexBuffer(uint32_t size);
		virtual ~OpenGLVertexBuffer();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual void SetData(const void* data, uint32_t offset, uint32_t size) override;

		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };

	private:
		BufferLayout m_Layout;
		uint32_t m_RendererID;
	};
	

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~OpenGLIndexBuffer();


		virtual inline uint32_t GetCount() const override {return m_Count;}
		virtual void Bind() const override;
		virtual void Unbind() const override;


	private:
		uint32_t m_Count;
		uint32_t m_RendererID;
	};
}