#pragma once

#include "base/Renderer.h"
#include "d3d11_render_base.h"
#include "d3d11_framebuffer.h"
#include "d3d11_ray_tracer.h"

namespace Echo
{
    class GPUBuffer;
	class D3D11Renderer: public Renderer
	{
		typedef vector<const char*>::type				Extensions;
		typedef vector<VkExtensionProperties>::type     ExtensionProperties;
        typedef vector<VkQueueFamilyProperties>::type   QueueFamilies;

	public:
        D3D11Renderer();
		virtual ~D3D11Renderer();

        // get
        static D3D11Renderer* instance();

		// initialize
        virtual bool initialize(const Settings& settings) override;

        // get type
        virtual Type getType() override { return Renderer::Type::Vulkan; }

        // create buffer
        virtual GPUBuffer*	createVertexBuffer(Dword usage, const Buffer& buff) override;
        virtual GPUBuffer*	createIndexBuffer(Dword usage, const Buffer& buff) override;

        // create texture
        virtual Texture*       createTexture2D(const String& name) override;
        virtual TextureCube*   createTextureCube(const String& name) override {return nullptr; }
        virtual TextureRenderTarget2D* createTextureRender(const String& name) override;

        // create views
        virtual FrameBufferOffScreen* createFrameBufferOffScreen(ui32 width, ui32 height) override { return EchoNew(VKFramebufferOffscreen(width, height));}
        virtual FrameBufferWindow* createFrameBufferWindow() override;

        // create states
        virtual RasterizerState* createRasterizerState() override;
        virtual DepthStencilState* createDepthStencilState()override;
        virtual BlendState* createBlendState() override;
        virtual MultisampleState* createMultisampleState() override;
        virtual SamplerState* createSamplerState() override;

        // create shaders
        virtual ShaderProgram* createShaderProgram() override;

		// create renderable
        virtual RenderProxy* createRenderProxy() override;
        virtual ComputeProxy* createComputeProxy() override;

        // convert matrix
        virtual void unproject(Vector3& worldPos, const Vector3& screenPos, const Matrix4& matVP, Viewport* pViewport = NULL) override;
        virtual void getDepthRange(Vector2& vec) override;
        virtual void convertMatView(Matrix4& mat) override;
        virtual void convertMatOrho(Matrix4& mat, const Matrix4& matOrth, Real zn, Real zf) override {}
        virtual void convertMatProj(Matrix4& mat, const Matrix4& matProj) override {}

		// scissor operate
		virtual void scissor(ui32 left, ui32 top, ui32 width, ui32 height) override {}
		virtual void endScissor() override {}

		// on size
        virtual void onSize(int width, int height) override;

		// draw
        virtual void draw(RenderProxy* renderable, FrameBufferPtr& frameBuffer) override;

		// present
        virtual bool present() override;

    public:
        // set texture
        virtual void setTexture(ui32 index, Texture* texture, bool needUpdate = false) override;
        VKTexture* getTexture(ui32 index);

    public:
        // get screen width and height
        virtual ui32 getWindowWidth() override { return m_screenWidth; }
        virtual ui32 getWindowHeight() override { return m_screenHeight; }

		// get view port
		virtual void getViewportReal(Viewport& pViewport) override {}

	public:
		VkInstance getVkInstance() { return m_vkInstance; }

		// get physical device
		VkPhysicalDevice getVkPhysicalDevice() { return m_vkPhysicalDevice; }

		// get logical device
		VkDevice getVkDevice() { return m_vkDevice; }

		// get queue family index
		const ui32 getGraphicsQueueFamilyIndex();
		const ui32 getPresentQueueFamilyIndex(VkSurfaceKHR vkSurface);

        // get queue
        VkQueue getVkGraphicsQueue() { return m_vkGraphicsQueue; }

        // get clear command buffer
        VkCommandPool getVkCommandPool() { return m_vkCommandPool; }

		// get vk descriptor pool
		VkDescriptorPool getVkDescriptorPool() { return m_vkDescriptorPool; }

        // find memory type
        ui32 findVkMemoryType(ui32 typeBits, VkMemoryPropertyFlags properties);

    public:
        // Vulkan command buffer
        VkCommandBuffer createVkCommandBuffer();
        void flushVkCommandBuffer(VkCommandBuffer commandBuffer, VkQueue queue, bool free);

        // Submit single time commands
        void submitSingleTimeCommands(const std::function<void(VkCommandBuffer)>& action);

    public:
        // Ray tracing
        VKRayTracer* getRayTracer() { return m_rayTracer; }
        void initRayTracer();

	private:
		// create vk instance
		void createVkInstance();

        // choose a physical device
        void pickVkPhysicalDevice();
        i32  calcVkPhysicalDeviceScore(const VkPhysicalDevice& device);

        // queue famalies
        void enumerateQueueFamalies();

        // create vk logical device
        void createVkLogicalDevice();

		// set up validation
		void createVkValidation();

		// create command pool
		void createVkCommandPool();

		// create vk descriptor pool
		void createVkDescriptorPool();

    private:
		ui32				m_screenWidth = 800;
		ui32				m_screenHeight = 600;
		Extensions			m_vkInstanceExtensions;
		VkInstance			m_vkInstance;
        VkPhysicalDevice    m_vkPhysicalDevice = nullptr;
        QueueFamilies       m_vkQueueFamilies;
        VkDevice            m_vkDevice = nullptr;
        VkQueue             m_vkGraphicsQueue = nullptr;
		VkCommandPool		m_vkCommandPool;
        VkDescriptorPool    m_vkDescriptorPool = VK_NULL_HANDLE;
        VKRayTracer*        m_rayTracer = nullptr;
        array<Texture*, 32> m_currentTextures = { nullptr };
	};
}
