#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

using std::string;
using std::map;

class TextureManager
{
	static map<string, sf::Texture> textures;
public:
	static void LoadTexture(string textureName);
	static sf::Texture& GetTexture(string textureName);
	static void Clear();
};