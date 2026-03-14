#include "MaterialGen.h"

#include "RendererAPI/ShaderManager.h"
#include <cstdint>

void MaterialGen::setBlinnPhongMaterial(uint32_t shader, BlinnPhongMaterial material)
{
	switch (material)
	{
	case Emerald:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0215f, 0.1745f, 0.0215f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.07568f, 0.61424f, 0.07568f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.633f, 0.727811f, 0.633f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 76.8f);
		break;

	case Jade:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.135f, 0.2225f, 0.1575f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.54f, 0.89f, 0.63f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.316228f, 0.316228f, 0.316228f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 12.8f);
		break;

	case Obsidian:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.05375f, 0.05f, 0.06625f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.18275f, 0.17f, 0.22525f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.332741f, 0.328634f, 0.346435f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 38.4f);
		break;

	case Pearl:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.25f, 0.20725f, 0.20725f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 1.0f, 0.829f, 0.829f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.296648f, 0.296648f, 0.296648f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 11.264f);
		break;

	case Ruby:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.1745f, 0.01175f, 0.01175f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.61424f, 0.04136f, 0.04136f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.727811f, 0.626959f, 0.626959f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 76.8f);
		break;

	case Turquoise:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.1f, 0.18725f, 0.1745f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.396f, 0.74151f, 0.69102f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.297254f, 0.30829f, 0.306678f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 12.8f);
		break;

	case Brass:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.329412f, 0.223529f, 0.027451f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.780392f, 0.568627f, 0.113725f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.992157f, 0.941176f, 0.807843f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 27.8974f);
		break;

	case Bronze:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.2125f, 0.1275f, 0.054f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.714f, 0.4284f, 0.18144f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.393548f, 0.271906f, 0.166721f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 25.6f);
		break;

	case Chrome:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.25f, 0.25f, 0.25f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.4f, 0.4f, 0.4f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.774597f, 0.774597f, 0.774597f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 76.8f);
		break;

	case Copper:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.19125f, 0.0735f, 0.0225f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.7038f, 0.27048f, 0.0828f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.256777f, 0.137622f, 0.086014f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 12.8f);
		break;

	case Gold:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.24725f, 0.1995f, 0.0745f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.75164f, 0.60648f, 0.22648f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.628281f, 0.555802f, 0.366065f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 51.2f);
		break;

	case Silver:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.19225f, 0.19225f, 0.19225f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.50754f, 0.50754f, 0.50754f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.508273f, 0.508273f, 0.508273f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 51.2f);
		break;

	case BlackPlastic:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.01f, 0.01f, 0.01f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.50f, 0.50f, 0.50f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 32.0f);
		break;

	case CyanPlastic:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.1f, 0.06f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 32.0f);
		break;

	case GreenPlastic:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.1f, 0.35f, 0.1f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.45f, 0.55f, 0.45f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 32.0f);
		break;

	case RedPlastic:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.5f, 0.0f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.7f, 0.6f, 0.6f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 32.0f);
		break;

	case WhitePlastic:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.55f, 0.55f, 0.55f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.70f, 0.70f, 0.70f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 32.0f);
		break;

	case YellowPlastic:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.5f, 0.5f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.60f, 0.60f, 0.50f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 32.0f);
		break;

	case BlackRubber:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.02f, 0.02f, 0.02f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.01f, 0.01f, 0.01f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.4f, 0.4f, 0.4f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 10.0f);
		break;

	case CyanRubber:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.05f, 0.05f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.4f, 0.5f, 0.5f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.04f, 0.7f, 0.7f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 10.0f);
		break;

	case GreenRubber:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.0f, 0.05f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.4f, 0.5f, 0.4f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.04f, 0.7f, 0.04f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 10.0f);
		break;

	case RedRubber:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.05f, 0.0f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.5f, 0.4f, 0.4f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.7f, 0.04f, 0.04f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 10.0f);
		break;

	case WhiteRubber:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.05f, 0.05f, 0.05f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.5f, 0.5f, 0.5f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.7f, 0.7f, 0.7f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 10.0f);
		break;

	case YellowRubber:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.05f, 0.05f, 0.0f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.5f, 0.5f, 0.4f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.7f, 0.7f, 0.04f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 10.0f);
		break;

	case CyanLight:
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 1.0f, 1.0f);
		break;

	case GreenLight:
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 1.0f, 0.0f);
		break;

	case RedLight:
		Core::ShaderManager::set3f(shader, "material.emission", 1.0f, 0.0f, 0.0f);
		break;

	case WhiteLight:
		Core::ShaderManager::set3f(shader, "material.emission", 1.0f, 1.0f, 1.0f);
		break;

	case YellowLight:
		Core::ShaderManager::set3f(shader, "material.emission", 1.0f, 1.0f, 0.0f);
		break;

	case FLASHBANG:
		Core::ShaderManager::set3f(shader, "material.emission", 200.0f, 200.0f, 200.0f);
		break;

	default:
		Core::ShaderManager::set3f(shader, "material.ambient", 0.05f, 0.05f, 0.05f);
		Core::ShaderManager::set3f(shader, "material.diffuse", 0.5f, 0.5f, 0.5f);
		Core::ShaderManager::set3f(shader, "material.specular", 0.7f, 0.7f, 0.7f);
		Core::ShaderManager::set3f(shader, "material.emission", 0.0f, 0.0f, 0.0f);
		Core::ShaderManager::setFloat(shader, "material.shininess", 10.0f);
		break;
	}
}