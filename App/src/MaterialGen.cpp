#include "MaterialGen.h"

void MaterialGen::setBlinnPhongMaterial(std::shared_ptr<Core::Shader> shader, BlinnPhongMaterial material)
{
	switch (material)
	{
	case Emerald:
		shader->set3f("material.ambient", 0.0215f, 0.1745f, 0.0215f);
		shader->set3f("material.diffuse", 0.07568f, 0.61424f, 0.07568f);
		shader->set3f("material.specular", 0.633f, 0.727811f, 0.633f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 76.8f);
		break;

	case Jade:
		shader->set3f("material.ambient", 0.135f, 0.2225f, 0.1575f);
		shader->set3f("material.diffuse", 0.54f, 0.89f, 0.63f);
		shader->set3f("material.specular", 0.316228f, 0.316228f, 0.316228f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 12.8f);
		break;

	case Obsidian:
		shader->set3f("material.ambient", 0.05375f, 0.05f, 0.06625f);
		shader->set3f("material.diffuse", 0.18275f, 0.17f, 0.22525f);
		shader->set3f("material.specular", 0.332741f, 0.328634f, 0.346435f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 38.4f);
		break;

	case Pearl:
		shader->set3f("material.ambient", 0.25f, 0.20725f, 0.20725f);
		shader->set3f("material.diffuse", 1.0f, 0.829f, 0.829f);
		shader->set3f("material.specular", 0.296648f, 0.296648f, 0.296648f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 11.264f);
		break;

	case Ruby:
		shader->set3f("material.ambient", 0.1745f, 0.01175f, 0.01175f);
		shader->set3f("material.diffuse", 0.61424f, 0.04136f, 0.04136f);
		shader->set3f("material.specular", 0.727811f, 0.626959f, 0.626959f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 76.8f);
		break;

	case Turquoise:
		shader->set3f("material.ambient", 0.1f, 0.18725f, 0.1745f);
		shader->set3f("material.diffuse", 0.396f, 0.74151f, 0.69102f);
		shader->set3f("material.specular", 0.297254f, 0.30829f, 0.306678f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 12.8f);
		break;

	case Brass:
		shader->set3f("material.ambient", 0.329412f, 0.223529f, 0.027451f);
		shader->set3f("material.diffuse", 0.780392f, 0.568627f, 0.113725f);
		shader->set3f("material.specular", 0.992157f, 0.941176f, 0.807843f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 27.8974f);
		break;

	case Bronze:
		shader->set3f("material.ambient", 0.2125f, 0.1275f, 0.054f);
		shader->set3f("material.diffuse", 0.714f, 0.4284f, 0.18144f);
		shader->set3f("material.specular", 0.393548f, 0.271906f, 0.166721f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 25.6f);
		break;

	case Chrome:
		shader->set3f("material.ambient", 0.25f, 0.25f, 0.25f);
		shader->set3f("material.diffuse", 0.4f, 0.4f, 0.4f);
		shader->set3f("material.specular", 0.774597f, 0.774597f, 0.774597f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 76.8f);
		break;

	case Copper:
		shader->set3f("material.ambient", 0.19125f, 0.0735f, 0.0225f);
		shader->set3f("material.diffuse", 0.7038f, 0.27048f, 0.0828f);
		shader->set3f("material.specular", 0.256777f, 0.137622f, 0.086014f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 12.8f);
		break;

	case Gold:
		shader->set3f("material.ambient", 0.24725f, 0.1995f, 0.0745f);
		shader->set3f("material.diffuse", 0.75164f, 0.60648f, 0.22648f);
		shader->set3f("material.specular", 0.628281f, 0.555802f, 0.366065f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 51.2f);
		break;

	case Silver:
		shader->set3f("material.ambient", 0.19225f, 0.19225f, 0.19225f);
		shader->set3f("material.diffuse", 0.50754f, 0.50754f, 0.50754f);
		shader->set3f("material.specular", 0.508273f, 0.508273f, 0.508273f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 51.2f);
		break;

	case BlackPlastic:
		shader->set3f("material.ambient", 0.0f, 0.0f, 0.0f);
		shader->set3f("material.diffuse", 0.01f, 0.01f, 0.01f);
		shader->set3f("material.specular", 0.50f, 0.50f, 0.50f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 32.0f);
		break;

	case CyanPlastic:
		shader->set3f("material.ambient", 0.0f, 0.1f, 0.06f);
		shader->set3f("material.diffuse", 0.0f, 0.50980392f, 0.50980392f);
		shader->set3f("material.specular", 0.50196078f, 0.50196078f, 0.50196078f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 32.0f);
		break;

	case GreenPlastic:
		shader->set3f("material.ambient", 0.0f, 0.0f, 0.0f);
		shader->set3f("material.diffuse", 0.1f, 0.35f, 0.1f);
		shader->set3f("material.specular", 0.45f, 0.55f, 0.45f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 32.0f);
		break;

	case RedPlastic:
		shader->set3f("material.ambient", 0.0f, 0.0f, 0.0f);
		shader->set3f("material.diffuse", 0.5f, 0.0f, 0.0f);
		shader->set3f("material.specular", 0.7f, 0.6f, 0.6f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 32.0f);
		break;

	case WhitePlastic:
		shader->set3f("material.ambient", 0.0f, 0.0f, 0.0f);
		shader->set3f("material.diffuse", 0.55f, 0.55f, 0.55f);
		shader->set3f("material.specular", 0.70f, 0.70f, 0.70f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 32.0f);
		break;

	case YellowPlastic:
		shader->set3f("material.ambient", 0.0f, 0.0f, 0.0f);
		shader->set3f("material.diffuse", 0.5f, 0.5f, 0.0f);
		shader->set3f("material.specular", 0.60f, 0.60f, 0.50f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 32.0f);
		break;

	case BlackRubber:
		shader->set3f("material.ambient", 0.02f, 0.02f, 0.02f);
		shader->set3f("material.diffuse", 0.01f, 0.01f, 0.01f);
		shader->set3f("material.specular", 0.4f, 0.4f, 0.4f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 10.0f);
		break;

	case CyanRubber:
		shader->set3f("material.ambient", 0.0f, 0.05f, 0.05f);
		shader->set3f("material.diffuse", 0.4f, 0.5f, 0.5f);
		shader->set3f("material.specular", 0.04f, 0.7f, 0.7f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 10.0f);
		break;

	case GreenRubber:
		shader->set3f("material.ambient", 0.0f, 0.05f, 0.0f);
		shader->set3f("material.diffuse", 0.4f, 0.5f, 0.4f);
		shader->set3f("material.specular", 0.04f, 0.7f, 0.04f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 10.0f);
		break;

	case RedRubber:
		shader->set3f("material.ambient", 0.05f, 0.0f, 0.0f);
		shader->set3f("material.diffuse", 0.5f, 0.4f, 0.4f);
		shader->set3f("material.specular", 0.7f, 0.04f, 0.04f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 10.0f);
		break;

	case WhiteRubber:
		shader->set3f("material.ambient", 0.05f, 0.05f, 0.05f);
		shader->set3f("material.diffuse", 0.5f, 0.5f, 0.5f);
		shader->set3f("material.specular", 0.7f, 0.7f, 0.7f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 10.0f);
		break;

	case YellowRubber:
		shader->set3f("material.ambient", 0.05f, 0.05f, 0.0f);
		shader->set3f("material.diffuse", 0.5f, 0.5f, 0.4f);
		shader->set3f("material.specular", 0.7f, 0.7f, 0.04f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 10.0f);
		break;

	case CyanLight:
		shader->set3f("material.emission", 0.0f, 1.0f, 1.0f);
		break;

	case GreenLight:
		shader->set3f("material.emission", 0.0f, 1.0f, 0.0f);
		break;

	case RedLight:
		shader->set3f("material.emission", 1.0f, 0.0f, 0.0f);
		break;

	case WhiteLight:
		shader->set3f("material.emission", 1.0f, 1.0f, 1.0f);
		break;

	case YellowLight:
		shader->set3f("material.emission", 1.0f, 1.0f, 0.0f);
		break;

	case FLASHBANG:
		shader->set3f("material.emission", 200.0f, 200.0f, 200.0f);
		break;

	default:
		shader->set3f("material.ambient", 0.05f, 0.05f, 0.05f);
		shader->set3f("material.diffuse", 0.5f, 0.5f, 0.5f);
		shader->set3f("material.specular", 0.7f, 0.7f, 0.7f);
		shader->set3f("material.emission", 0.0f, 0.0f, 0.0f);
		shader->setFloat("material.shininess", 10.0f);
		break;
	}
}