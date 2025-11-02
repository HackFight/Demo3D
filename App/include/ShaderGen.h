#pragma once

#include "RendererAPI/Shader.h"

#include <memory>

enum BasicShader
{
	BlinnPhong
};

class ShaderGen
{
public:
	static void InitShaders();
	static std::shared_ptr<Core::Shader> GetShader(BasicShader shader);

private:
	static std::shared_ptr<Core::Shader> blinnPhongShader;
};