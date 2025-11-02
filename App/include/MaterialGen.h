#pragma once

#include "RendererAPI/Shader.h"

#include <memory>

enum BlinnPhongMaterial
{
    Emerald,
    Jade,
	Obsidian,
	Pearl,
	Ruby,
	Turquoise,
	Brass,
	Bronze,
	Chrome,
	Copper,
	Gold,
	Silver,
	BlackPlastic,
	CyanPlastic,
	GreenPlastic,
	RedPlastic,
	WhitePlastic,
	YellowPlastic,
	BlackRubber,
	CyanRubber,
	GreenRubber,
	RedRubber,
	WhiteRubber,
	YellowRubber
};

class MaterialGen
{
public:
    static void setBlinnPhongMaterial(std::shared_ptr<Core::Shader> shader, BlinnPhongMaterial material);
};