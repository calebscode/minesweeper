#include "Cell.h"

Cell::Cell(int row, int col)
{
	// Member variables
	this->row = row;
	this->col = col;
	this->isMine = false;
	this->flagged = false;
	this->hidden = true;
	this->adjMines = 0;

	// Sprites / Textures
	this->tile.setTexture(TextureManager::GetTexture("tile_hidden"));

	this->tile.setPosition((float)(col * tile.getTextureRect().width), (float)(row * tile.getTextureRect().height));
	this->mineNum.setPosition((float)(col * tile.getTextureRect().width), (float)(row * tile.getTextureRect().height));
	this->flag.setPosition((float)(col * tile.getTextureRect().width), (float)(row * tile.getTextureRect().height));
}

void Cell::SetMine()
{
	this->mineNum.setTexture(TextureManager::GetTexture("mine"));
	this->isMine = true;
}

void Cell::CountMines()
{
	if (!isMine) {
		for (auto iter = neighbors.begin(); iter != neighbors.end(); iter++) {
			if (iter->second != nullptr && iter->second->isMine) {
				adjMines++;
			}
		}
		if (adjMines > 0) {
			string path = "number_";
			path += std::to_string(adjMines);
			mineNum.setTexture(TextureManager::GetTexture(path));
		}
	}
	else {
		adjMines = -1;
	}
}

void Cell::Reveal() {
	tile.setTexture(TextureManager::GetTexture("tile_revealed"));
	hidden = false;
}

void Cell::Hide()
{
	tile.setTexture(TextureManager::GetTexture("tile_hidden"));
	hidden = true;
}

void Cell::LeftClicked()
{
	if (hidden) {
		Reveal();
		if (isMine) {
			return;
		}
		else if (adjMines == 0) {
			for (auto iter = neighbors.begin(); iter != neighbors.end(); iter++) {
				if (iter->second != nullptr) {
					if (iter->second->adjMines == 0) {
						iter->second->LeftClicked();
					}
					else if (!iter->second->isMine) {
						iter->second->Reveal();
					}
				}
			}
		}
	}
}

void Cell::ToggleFlag()
{
	if (hidden) {
		if (!flagged) {
			flag.setTexture(TextureManager::GetTexture("flag"));
			flagged = true;
		}
		else {
			flag.setTexture(TextureManager::GetTexture("tile_hidden"));
			flagged = false;
		}
	}
}

void Cell::DrawCell(sf::RenderWindow& window)
{
	if (hidden) {
		window.draw(mineNum);
		window.draw(tile);
		window.draw(flag);
	}
	else {
		window.draw(tile);
		window.draw(mineNum);
	}
}
