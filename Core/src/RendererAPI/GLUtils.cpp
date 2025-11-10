#include "RendererAPI/GLUtils.h"

// std
#include <print>

namespace Renderer::Utils {

	const char* GLDebugSourceToString(GLenum source)
	{
		switch (source)
		{
			case GL_DEBUG_SOURCE_API:             return "API";
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   return "WINDOW SYSTEM";
			case GL_DEBUG_SOURCE_SHADER_COMPILER: return "SHADER COMPILER";
			case GL_DEBUG_SOURCE_THIRD_PARTY:     return "THIRD PARTY";
			case GL_DEBUG_SOURCE_APPLICATION:     return "APPLICATION";
			case GL_DEBUG_SOURCE_OTHER:
			default:                              return "UNKNOWN";
		}
	}

	const char* GLDebugTypeToString(GLenum type)
	{
		switch (type)
		{
			case GL_DEBUG_TYPE_ERROR:               return "ERROR";
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "DEPRECATED BEHAVIOR";
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "UDEFINED BEHAVIOR";
			case GL_DEBUG_TYPE_PORTABILITY: 	    return "PORTABILITY";
			case GL_DEBUG_TYPE_PERFORMANCE:         return "PERFORMANCE";
			case GL_DEBUG_TYPE_OTHER:               return "OTHER";
			case GL_DEBUG_TYPE_MARKER:              return "MARKER";
			default:                                return "UNKNOWN";
		}

	}

	const char* GLDebugSeverityToString(GLenum severity)
	{
		switch (severity) 
		{
			case GL_DEBUG_SEVERITY_HIGH:         return "HIGH";
			case GL_DEBUG_SEVERITY_MEDIUM:       return "MEDIUM";
			case GL_DEBUG_SEVERITY_LOW:          return "LOW";
			case GL_DEBUG_SEVERITY_NOTIFICATION: return "NOTIFICATION";
			default:                             return "UNKNOWN";
		}
	}

	static void GLDebugCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		// TODO: Custom filters
		if (severity != GL_DEBUG_SEVERITY_MEDIUM && severity != GL_DEBUG_SEVERITY_HIGH)
			return;

		const char* sourceStr = Utils::GLDebugSourceToString(source);
		const char* typeStr = Utils::GLDebugTypeToString(type);
		const char* severityStr = Utils::GLDebugSeverityToString(severity);

		std::println("[OpenGL] [{} - {} ({})]: [{}] {}", severityStr, typeStr, id, sourceStr, message);
	}

	void InitOpenGLDebugMessageCallback()
	{
		glDebugMessageCallback(GLDebugCallback, nullptr);
	}

}