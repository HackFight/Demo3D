#include "ShaderGen.h"

std::shared_ptr<Core::Shader> ShaderGen::blinnPhongShader = nullptr;

void ShaderGen::InitShaders()
{
	blinnPhongShader = std::make_shared<Core::Shader>(RESOURCES_PATH "shaders/default.vert", RESOURCES_PATH "shaders/blinn-phong.frag");
}

std::shared_ptr<Core::Shader> ShaderGen::GetShader(BasicShader shader)
{
	switch (shader)
	{
	case BlinnPhong:
		return blinnPhongShader;

	default:
		return nullptr;
	}
}