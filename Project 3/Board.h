#pragma once

#include <vector>
#include "Cell.h"

using std::string;
using std::vector;

class Board
{
	int cols;
	int rows;
	int mines;
	vector<vector<Cell*>> cells;

public:
	bool debugMode;

	Board(const int cols = 25, const int rows = 16, int mines = 50);
	vector<vector<Cell*>>& GetCells() { return cells; }
	int GetMines() { return mines; }
	void CalculateNeighbors();
	void FillMines(int mines);
	void Reset(int mines);
	void ClickAllCells();
	void DrawBoard(sf::RenderWindow& window);
	void LoadBoard(string board);
	void Clear();
	void ShowAllMines();
	void HideAllMines();
	bool GameWon();
	int CountFlagged();
	void FlagAllMines();
};

