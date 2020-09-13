#include <random>
#include <fstream>
#include <sstream>
#include "Board.h"

Board::Board(const int cols, const int rows, int mines)
{
	// member variables
	this->cols = cols;
	this->rows = rows;
	this->mines = mines;
	this->debugMode = false;

	// populate with cells
	cells.resize(rows);
	for (unsigned int i = 0; i < cells.size(); i++) {
		cells.at(i).resize(cols);
	}

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Cell* cell = new Cell(i, j);
			cells[i][j] = cell;
		}
	}

	FillMines(mines);
	CalculateNeighbors();

	// update cell textures to the right numbers
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cells[i][j]->CountMines();
		}
	}
}

void Board::CalculateNeighbors()
{
	for (unsigned int row = 0; row < cells.size(); row++) {
		for (unsigned int col = 0; col < cells.at(0).size(); col++) {

			if (row > 0) {
				cells[row][col]->SetNeighbor("top", cells[row - 1][col]);

				if (col > 0) {
					cells[row][col]->SetNeighbor("top left", cells[row - 1][col - 1]);
				}
				else {
					cells[row][col]->SetNeighbor("top left", nullptr);
				}
				if (col < cols - (unsigned int)1) {
					cells[row][col]->SetNeighbor("top right", cells[row - 1][col + 1]);
				}
				else {
					cells[row][col]->SetNeighbor("top right", nullptr);
				}
			}
			else {
				cells[row][col]->SetNeighbor("top", nullptr);
			}

			if (row < rows - (unsigned int)1) {
				cells[row][col]->SetNeighbor("bottom", cells[row + 1][col]);

				if (col > 0) {
					cells[row][col]->SetNeighbor("bottom left", cells[row + 1][col - 1]);
				}
				else {
					cells[row][col]->SetNeighbor("bottom left", nullptr);
				}
				if (col < cols - (unsigned int)1) {
					cells[row][col]->SetNeighbor("bottom right", cells[row + 1][col + 1]);
				}
				else {
					cells[row][col]->SetNeighbor("bottom right", nullptr);
				}
			}
			else {
				cells[row][col]->SetNeighbor("bottom", nullptr);
			}

			if (col > 0) {
				cells[row][col]->SetNeighbor("left", cells[row][col - 1]);
			}
			else {
				cells[row][col]->SetNeighbor("left", nullptr);
			}

			if (col < cols - (unsigned int)1) {
				cells[row][col]->SetNeighbor("right", cells[row][col + 1]);
			}
			else {
				cells[row][col]->SetNeighbor("right", nullptr);
			}
		}
	}
}

void Board::FillMines(int mines)
{
	this->mines = mines;

	std::random_device device;
	std::mt19937 generator(device());
	std::uniform_int_distribution<int> rowDist(0, rows - 1);
	std::uniform_int_distribution<int> colDist(0, cols - 1);


	int currentMines = 0;
	int xPos = colDist(generator);
	int yPos = rowDist(generator);
	while (currentMines < mines) {
		cells.at(yPos).at(xPos)->SetMine();
		currentMines++;
		while (cells[yPos][xPos]->isMine) {
			xPos = colDist(generator);
			yPos = rowDist(generator);
		}
	}
}

void Board::Reset(int mines)
{
	this->mines = mines;

	Clear();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Cell* cell = new Cell(i, j);
			cells[i][j] = cell;
		}
	}

	FillMines(mines);
	CalculateNeighbors();

	// update cell textures to the right numbers
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cells[i][j]->CountMines();
		}
	}
}

void Board::ClickAllCells()
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cells[i][j]->LeftClicked();
		}
	}
}

void Board::DrawBoard(sf::RenderWindow& window)
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cells[i][j]->DrawCell(window);
		}
	}
}

void Board::LoadBoard(string board)
{
	string path = "boards/";
	path += board;
	path += ".brd";

	Clear();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			Cell* cell = new Cell(i, j);
			cells[i][j] = cell;
		}
	}

	std::ifstream file(path);
	if (file.is_open()) {
		/*
		1. read in all the characters into a 2D array
		2. iterate through array
			2a. if its a 0, no mine
			2b. if its a 1, mine
		*/

		char space;
		int numMines = 0;
		for (int i = 0; i < 16; i++) {
			for (int j = 0; j < 25; j++) {
				file >> space;
				if (space == '1') {
					cells[i][j]->SetMine();
					numMines++;
				}
			}
		}

		this->mines = numMines;

		CalculateNeighbors();

		// update cell textures to the right numbers
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				cells[i][j]->CountMines();
			}
		}
	}
}

void Board::Clear()
{
	for (unsigned int i = 0; i < cells.size(); i++) {
		for (unsigned int j = 0; j < cells.at(0).size(); j++) {
			delete cells[i][j];
		}
	}

	mines = 0;
}

void Board::ShowAllMines()
{
	debugMode = true;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (cells[i][j]->isMine) {
				cells[i][j]->Reveal();
			}
		}
	}
}

void Board::HideAllMines()
{
	debugMode = false;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (cells[i][j]->isMine && !cells[i][j]->hidden) {
				cells[i][j]->Hide();
			}
		}
	}
}

bool Board::GameWon()
{
	/*if (debugMode) {
		return false;
	}
	else {*/
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (!cells[i][j]->isMine && cells[i][j]->hidden) {
					return false;
				}
			}
		}
	//}

	return true;
}

int Board::CountFlagged()
{
	int cellsFlagged = 0;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (cells[i][j]->hidden && cells[i][j]->flagged) {
				cellsFlagged++;
			}
		}
	}
	return cellsFlagged;
}

void Board::FlagAllMines()
{
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (cells[i][j]->isMine && !cells[i][j]->flagged) {
				cells[i][j]->ToggleFlag();
			}
		}
	}
}
