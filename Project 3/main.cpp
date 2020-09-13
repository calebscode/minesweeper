#include <cmath>
#include "TextureManager.h"
#include "Board.h"
#include "Cell.h"
#include "Button.h"

int main()
{
	sf::RenderWindow window(sf::VideoMode(800, 600), "Minesweeper");

	bool gameOver = false;
	int numFlagged = 0;
	int initialMines = 50;
	int minesLeft = initialMines;
	Board board(25, 16, initialMines);

	// background color
	sf::RectangleShape rectangle(sf::Vector2f((float)window.getSize().x, (float)window.getSize().y));
	rectangle.setFillColor(sf::Color(200, 200, 200));

	// create menu buttons
	Button smiley("face_happy");
	smiley.setPosition(400.0f - (smiley.getTextureRect().width / 2.0f), 512.0f);

	Button debug("debug");
	debug.setPosition(400.0f + (2.0f * debug.getTextureRect().width) - (smiley.getTextureRect().width / 2.0f), 512.0f);

	Button test1("test_1");
	test1.setPosition(400.0f + (3.0f * test1.getTextureRect().width) - (smiley.getTextureRect().width / 2.0f), 512.0f);

	Button test2("test_2");
	test2.setPosition(400.0f + (4.0f * test2.getTextureRect().width) - (smiley.getTextureRect().width / 2.0f), 512.0f);

	Button test3("test_3");
	test3.setPosition(400.0f + (5.0f * test3.getTextureRect().width) - (smiley.getTextureRect().width / 2.0f), 512.0f);

	// scoreboard digits
	sf::Sprite digit1;
	digit1.setTexture(TextureManager::GetTexture("digits"));
	digit1.setPosition(0.0f, 512.0f);

	sf::Sprite digit2;
	digit2.setTexture(TextureManager::GetTexture("digits"));
	digit2.setPosition(21.0f, 512.0f);

	sf::Sprite digit3;
	digit3.setTexture(TextureManager::GetTexture("digits"));
	digit3.setPosition(42.0f, 512.0f);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			// close the window
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			// mouse pressed
			else if (event.type == sf::Event::MouseButtonPressed) {

				// get mouse positions
				unsigned int mouseX = event.mouseButton.x;
				unsigned int mouseY = event.mouseButton.y;

				// game board row/column indices
				unsigned int boardX = mouseX / board.GetCells().at(0).at(0)->tile.getTextureRect().width;
				unsigned int boardY = mouseY / board.GetCells().at(0).at(0)->tile.getTextureRect().height;

				if (event.mouseButton.button == sf::Mouse::Left) { // LEFT MOUSE BUTTON

					// CLICKING ON CELLS
					if (!gameOver && boardY < board.GetCells().size() && boardX < board.GetCells().at(0).size()) {
						if (!board.GetCells()[boardY][boardX]->flagged) {

							board.GetCells()[boardY][boardX]->LeftClicked(); // click dat bih

							if (board.GetCells()[boardY][boardX]->isMine) {  // GAME OVER
								gameOver = true;
								smiley.setTexture(TextureManager::GetTexture("face_lose"));
								board.ShowAllMines();
								board.debugMode = false;
								numFlagged = initialMines;
							}
						}
					}
					else if (smiley.isClicked(mouseX, mouseY)) {
						initialMines = 50;
						board.Reset(minesLeft);
						smiley.setTexture(TextureManager::GetTexture("face_happy"));
						minesLeft = board.GetMines();
						numFlagged = 0;
						gameOver = false;
						board.debugMode = false;
					}
					if (!gameOver && debug.isClicked(mouseX, mouseY)) {
						if (!board.debugMode) {
							board.ShowAllMines();
						}
						else {
							board.HideAllMines();
						}
					}
					// board 1
					if (test1.isClicked(mouseX, mouseY)) {
						board.LoadBoard("testBoard1");
						board.debugMode = false;
						initialMines = board.GetMines();
						smiley.setTexture(TextureManager::GetTexture("face_happy"));
						gameOver = false;
					}

					// board 2
					else if (test2.isClicked(mouseX, mouseY)) {
						board.LoadBoard("testBoard2");
						board.debugMode = false;
						initialMines = board.GetMines();
						smiley.setTexture(TextureManager::GetTexture("face_happy"));
						gameOver = false;
					}

					// board 3
					else if (test3.isClicked(mouseX, mouseY)) {
						board.LoadBoard("testBoard3");
						board.debugMode = false;
						initialMines = board.GetMines();
						smiley.setTexture(TextureManager::GetTexture("face_happy"));
						gameOver = false;
					}

				}
				// RIGHT MOUSE BUTTON
				else if (event.mouseButton.button == sf::Mouse::Right) {

					if (!gameOver && boardY < board.GetCells().size() && boardX < board.GetCells().at(0).size()) {
						board.GetCells()[boardY][boardX]->ToggleFlag();
						if (board.GetCells()[boardY][boardX]->flagged) {
							numFlagged++;
						}
						else {
							numFlagged--;
						}
					}

				}
			}

		}
		// UPDATE MINE COUNTER
		minesLeft = initialMines - board.CountFlagged();
		// sign
		if (minesLeft < 0) {
			digit1.setTextureRect(sf::IntRect(210, 0, 21, 32)); // negative sign
		}
		else {
			digit1.setTextureRect(sf::IntRect(0, 0, 21, 32));
		}

		// tens/ones digits
		if (abs(minesLeft) > 9) {
			int tens = abs(minesLeft) / 10;
			digit2.setTextureRect(sf::IntRect(tens * 21, 0, 21, 32)); // set tens place

			tens = abs(minesLeft) % 10;
			digit3.setTextureRect(sf::IntRect(tens * 21, 0, 21, 32));
		}
		else if (abs(minesLeft) < 10) {
			digit2.setTextureRect(sf::IntRect(0, 0, 21, 32));
			digit3.setTextureRect(sf::IntRect(abs(minesLeft) * 21, 0, 21, 32));
		}

		// check is game has been won
		if (board.GameWon()) {
			smiley.setTexture(TextureManager::GetTexture("face_win"));
			board.FlagAllMines();
			gameOver = true;
		}

		window.clear(sf::Color::White);

		// ============== DRAW =================
		window.draw(rectangle);
		board.DrawBoard(window);
		window.draw(smiley);
		window.draw(debug);
		window.draw(test1);
		window.draw(test2);
		window.draw(test3);
		window.draw(digit1);
		window.draw(digit2);
		window.draw(digit3);
		// =====================================

		window.display();
	}

	//Cleanup
	TextureManager::Clear();
	board.Clear();

	return 0;
}
