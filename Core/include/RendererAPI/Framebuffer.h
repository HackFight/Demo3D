#pragma once

// Core
#include "RendererAPI/Texture.h"

namespace Core {

    struct RenderbufferInfo
    {
		uint32_t internalFormat;
        int width;
        int height;
        bool multisampled;
        int samples;
	};

    class Renderbuffer
    {
    public:
        Renderbuffer();
        Renderbuffer(uint32_t internalFormat, int width, int height, bool multisampled, int samples);
        ~Renderbuffer();

        void Bind() const;
		static void Unbind();

		uint32_t GetRendererID() { return m_RendererID; }
        RenderbufferInfo GetRenderbufferInfo() { return m_RenderbufferInfo; }

        void SetData(uint32_t internalFormat, int width, int height, bool multisampled, int samples);
		void Resize(int width, int height);
    
    private:
        uint32_t m_RendererID;
		RenderbufferInfo m_RenderbufferInfo;
    };

    class Framebuffer
    {
    public:
    	Framebuffer();
		~Framebuffer();

		void Bind() const;
		static void Unbind();

        void AttachTexture(Texture texture);
        void AttachRenderBuffer(Renderbuffer renderbuffer);
        void Blit();
        void Blit(Framebuffer destination);
        
    private:
        uint32_t m_RendererID;
		int m_Width, m_Height;
    };
}