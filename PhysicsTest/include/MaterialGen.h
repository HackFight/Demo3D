#pragma once

#include <stdint.h>

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
	YellowRubber,
	CyanLight,
	GreenLight,
	RedLight,
	WhiteLight,
	YellowLight,
	FLASHBANG
};

class MaterialGen
{
public:
    static void setBlinnPhongMaterial(uint32_t shader, BlinnPhongMaterial material);
};