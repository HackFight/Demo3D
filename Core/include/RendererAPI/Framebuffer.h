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
        Renderbuffer(AttachementType type, int width, int height);
        ~Renderbuffer();

        void Bind() const;
		static void Unbind();

        uint32_t GetRendererID() {return m_RendererID;}
        AttachementType GetType() {return type;}

        void SetData(int width, int height);

        static std::shared_ptr<Renderbuffer> Create(AttachementType type, int width, int height);
    
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

        void AttachTexture(AttachementType type, std::shared_ptr<Texture> texture);
		void AttachTexture(AttachementType type, int width, int height);
        void AttachRenderBuffer(std::shared_ptr<Renderbuffer> renderbuffer);

		static std::shared_ptr<Framebuffer> Create();
        
    private:
        uint32_t m_RendererID;
    };
}