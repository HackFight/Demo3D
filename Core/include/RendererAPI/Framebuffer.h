#pragma once

// std
#include "RendererAPI/Texture.h"
#include <memory>
#include <stdint.h>

namespace Core {

    enum AttachementType
    {
        Color,
        Depth_Stencil,
        Depth
    };

    class Renderbuffer
    {
    public:
        Renderbuffer(AttachementType type, int width, int height, bool multisampled = false, int samples = 4);
        ~Renderbuffer();

        void Bind() const;
		static void Unbind();

        uint32_t GetRendererID() {return m_RendererID;}
        AttachementType GetType() {return type;}

        void SetData(int width, int height, bool multisampled = false, int samples = 4);

        static std::shared_ptr<Renderbuffer> Create(AttachementType type, int width, int height, bool multisampled = false, int samples = 4);
    
    private:
        uint32_t m_RendererID;
        AttachementType type;
    };

    class Framebuffer
    {
    public:
    	Framebuffer();
		~Framebuffer();

		void Bind() const;
		static void Unbind();

		uint16_t GetRendererID() { return m_RendererID; }

        void AttachTexture(AttachementType type, std::shared_ptr<Texture> texture, bool multisampled = false);
		void AttachTexture(AttachementType type, int width, int height, bool multisampled = false, int samples = 4);
        void AttachRenderBuffer(std::shared_ptr<Renderbuffer> renderbuffer);
        void Blit(std::shared_ptr<Framebuffer> destination, int width, int height);

		static std::shared_ptr<Framebuffer> Create();
        
    private:
        uint32_t m_RendererID;
    };
}