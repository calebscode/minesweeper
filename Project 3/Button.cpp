#include "Button.h"

Button::Button(string texture)
{
	setTexture(TextureManager::GetTexture(texture));
}

bool Button::isClicked(int mouseX, int mouseY)
{
	if (mouseX > getPosition().x && mouseX < getPosition().x + getTextureRect().width) {
		if (mouseY > getPosition().y&& mouseY < getPosition().y + getTextureRect().height) {
			return true;
		}
	}

	return false;
}
