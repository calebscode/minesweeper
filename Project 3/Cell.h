#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using std::string;
using std::map;

class Cell
{
	int row;
	int col;
	int adjMines;

	map<string, Cell*> neighbors;

public:
	sf::Sprite tile;
	sf::Sprite mineNum;
	sf::Sprite flag;

	bool isMine;
	bool flagged;
	bool hidden;

	Cell(int row = 0, int col = 0);
	void SetNeighbor(string key, Cell* cell) { neighbors[key] = cell; }
	int GetRow() { return row; }
	int GetCol() { return col; }
	void SetMine();
	void CountMines();
	void Reveal();
	void Hide();
	void LeftClicked();
	void ToggleFlag();
	void DrawCell(sf::RenderWindow& window);
};

