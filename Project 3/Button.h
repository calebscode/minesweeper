#pragma once

#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using std::string;

struct Button : sf::Sprite
{
	Button(string texture);
	bool isClicked(int mouseX, int mouseY);
};

