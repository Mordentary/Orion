#pragma once
#include"Orion/Renderer/GraphicsCore/Buffer.h"


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





	class OpenGLUniformBuffer : public UniformBuffer 
	{
	public:
		OpenGLUniformBuffer(const BufferLayout& layout, const std::string& name, uint32_t numOfLayouts); // Initiliase and allocate memory
		OpenGLUniformBuffer(uint32_t size, const std::string& name); // Initiliase and allocate memory


		virtual ~OpenGLUniformBuffer();

		virtual void Bind(uint32_t slot) override;
		virtual void Unbind() const override;

		virtual void SetData(const void* data, uint32_t size) override;
		virtual void SetData(const void* data, uint32_t offset, uint32_t size) override;

		virtual void SetDataUsingLayout(uint32_t indexOfElement,const void* data) override;


		virtual const BufferLayout& GetLayout() const override { return m_Layout; }
		virtual void SetLayout(const BufferLayout& layout) override { m_Layout = layout; };

		virtual const uint32_t& GetBindingSlot() const override { return m_BindingSlot; };
		virtual void SetBindingSlot(const uint32_t slot) override { m_BindingSlot = slot; };

		virtual const std::string& GetBufferName() const override { return m_BufferName; };


	private:
		BufferLayout m_Layout;

		std::string m_BufferName;
		uint32_t m_BindingSlot = 0;

		uint32_t m_RendererID = 0;

	};





}