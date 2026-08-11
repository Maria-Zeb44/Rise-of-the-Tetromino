#include"Game.h"
using namespace std;

Game::Game() : window(sf::VideoMode(800, 700), "Tetris Project OOP"),
gen(rd()), dist(0, 6), score(0), level(1), linesCleared(0),
fallSpeed(INITIAL_FALL_SPEED), isGameOver(false), isPaused(false)
{
	well.resize(HEIGHT_OF_WELL, vector<sf::Color>(WIDTH_OF_WELL, sf::Color::Black));
	if (!font.loadFromFile("arial.ttf")) {
		cerr << "Failed to load font" << endl;
	}
	spawnNewShape();
}

void Game::run() {
	while (window.isOpen()) {
		handleInput();
		if (!isGameOver) {
			update();
		}
		render();
	}
}
void Game::spawnNewShape() {
	int shapeType = dist(gen);
	int startX = WIDTH_OF_WELL / 2 - 2;// initial position along x axis from where the tetrimon will be dropped
	int startY = 0;// initial position along y axis from where the tetrimon will be dropped

	switch (shapeType) {
	case 0: currentShape = make_unique<IShape>(startX, startY); break;
	case 1: currentShape = make_unique<JShape>(startX, startY); break;
	case 2: currentShape = make_unique<LShape>(startX, startY); break;
	case 3: currentShape = make_unique<OShape>(startX, startY); break;
	case 4: currentShape = make_unique<SShape>(startX, startY); break;
	case 5: currentShape = make_unique<TShape>(startX, startY); break;
	case 6: currentShape = make_unique<ZShape>(startX, startY); break;
	default: currentShape = make_unique<IShape>(startX, startY); break;
	}

	// Check if game over
	if (checkCollision()) {
		isGameOver = true;
	}
}
bool Game::checkCollision() {
	const vector<sf::Vector2i>& coords = currentShape->getCoordinates();
	for (size_t i = 0; i < coords.size(); ++i) {
		int wellX = currentShape->getX() + coords[i].x;
		int wellY = currentShape->getY() + coords[i].y;
		if (wellX < 0 || wellX >= WIDTH_OF_WELL || wellY >= HEIGHT_OF_WELL) {
			return true;
		}
		if (wellY >= 0 && well[wellY][wellX] != sf::Color::Black) {
			return true;
		}
	}
	return false;
}

bool Game::checkPosition(const vector<sf::Vector2i>& coords, int tempX, int tempY) {
	for (size_t i = 0; i < coords.size(); ++i) {
		int wellX = currentShape->getX() + coords[i].x + tempX;
		int wellY = currentShape->getY() + coords[i].y + tempY;
		if (wellX < 0 || wellX >= WIDTH_OF_WELL || wellY >= HEIGHT_OF_WELL) {
			return false;
		}
		if (wellY >= 0 && well[wellY][wellX] != sf::Color::Black) {
			return false;
		}
	}
	return true;
}

void Game::mergeShape() {
	const vector<sf::Vector2i>& coords = currentShape->getCoordinates();
	for (size_t i = 0; i < coords.size(); ++i) {
		int wellX = currentShape->getX() + coords[i].x;
		int wellY = currentShape->getY() + coords[i].y;

		if (wellY >= 0) {
			well[wellY][wellX] = currentShape->getColor();
		}
	}
}

void Game::clearLines() {
	int linesToClear = 0;
	for (int y = HEIGHT_OF_WELL - 1; y >= 0; --y) {
		bool lineComplete = true;
		for (int x = 0; x < WIDTH_OF_WELL; ++x) {
			if (well[y][x] == sf::Color::Black) {
				lineComplete = false;
				break;
			}
		}

		if (lineComplete) {
			well.erase(well.begin() + y);
			well.insert(well.begin(), vector<sf::Color>(WIDTH_OF_WELL, sf::Color::Black));
			linesToClear++;
			y++; 
		}
	}

	if (linesToClear > 0) {
	
		score += 100 * linesToClear;// INCREMENT OF 100 PER CLEARING 1 LINE

	
		int prevLinesCleared = linesCleared; 	// Track total lines cleared
		linesCleared += linesToClear;

		
		int prevThreshold = prevLinesCleared / 10;// Check if we passed a 10-line threshold
		int newThreshold = linesCleared / 10;

		if (newThreshold > prevThreshold) {
			level++;
			if (level > 8) {
				level = 1; // wrap to level 1
			}

			// Update fall speed
			fallSpeed = INITIAL_FALL_SPEED * pow(1.1f, level - 1);// INC SPEED BY 10 PERCENT
		}
	}
}





void Game::handleInput() {
	sf::Event event;
	while (window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window.close();
		}
		if (event.type == sf::Event::KeyPressed && isGameOver) {
			if (event.key.code == sf::Keyboard::R) {
				resetGame();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !shapeLocked) {
			//(Additional)->HARDDROP
			while (checkPosition(currentShape->getCoordinates(), 0, 1)) {
				currentShape->moveDown();
			}
			mergeShape();
			clearLines();
			spawnNewShape();
			gameClock.restart();      // Reset fall timer
			inputClock.restart();     // Reset input timer
			shapeLocked = true;       // Prevent update from running again
		}
		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::P) {
				isPaused = !isPaused; // Toggle pause state
			}
			if (event.key.code == sf::Keyboard::R && isGameOver) {
				resetGame();
			}
		}
	}

	if (isGameOver) return;

	// Handle continuous movement
	if (moveClock.getElapsedTime().asMilliseconds() > 100) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			if (checkPosition(currentShape->getCoordinates(), -1, 0)) {
				currentShape->moveLeft();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			if (checkPosition(currentShape->getCoordinates(), 1, 0)) {
				currentShape->moveRight();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			if (checkPosition(currentShape->getCoordinates(), 0, 1)) {
				currentShape->moveDown();
			}
			else {
				mergeShape();
				clearLines();
				spawnNewShape();
			}
		}
		moveClock.restart();
	}

	// Handle rotation (single press)
	if (inputClock.getElapsedTime().asMilliseconds() > 200) {
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			
			vector<sf::Vector2i> rotatedCoords = currentShape->getRotatedCoords();

			if (checkPosition(rotatedCoords, 0, 0)) {
				currentShape->rotate();
			}
			inputClock.restart();
		}
	}
}
void Game::update() {
	if (isPaused || isGameOver) return;

	if (shapeLocked) {
		shapeLocked = false;
		return;
	}

	if (gameClock.getElapsedTime().asSeconds() > fallSpeed) {
		gameClock.restart();
		if (checkPosition(currentShape->getCoordinates(), 0, 1)) {
			currentShape->moveDown();
		}
		else {
			mergeShape();
			clearLines();
			spawnNewShape();
		}
	}
}
void Game::resetGame() {
	well = vector<vector<sf::Color>>(HEIGHT_OF_WELL, vector<sf::Color>(WIDTH_OF_WELL, sf::Color::Black));
	score = 0;
	level = 1;
	linesCleared = 0;
	fallSpeed = INITIAL_FALL_SPEED;
	isGameOver = false;
	spawnNewShape();
}
void Game::render() {
	// Clear with white background
	window.clear(sf::Color::White);

	// Draw game well background
	sf::RectangleShape wellBackground(sf::Vector2f(
		WIDTH_OF_WELL * BLOCK_SIZE + 2,
		HEIGHT_OF_WELL * BLOCK_SIZE + 2
	));
	wellBackground.setPosition(SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW - 1, UPWARD_DISTANCE_OF_WELL_FROM_WINDOW - 1);
	wellBackground.setFillColor(sf::Color(240, 240, 240));
	wellBackground.setOutlineThickness(1);
	wellBackground.setOutlineColor(sf::Color(200, 200, 200));
	window.draw(wellBackground);

	// Draw well grid lines
	for (int x = 0; x <= WIDTH_OF_WELL; x++) {
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + x * BLOCK_SIZE, UPWARD_DISTANCE_OF_WELL_FROM_WINDOW), sf::Color(220, 220, 220)),
			sf::Vertex(sf::Vector2f(SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + x * BLOCK_SIZE, UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + HEIGHT_OF_WELL * BLOCK_SIZE), sf::Color(220, 220, 220))
		};
		window.draw(line, 2, sf::Lines);
	}
	for (int y = 0; y <= HEIGHT_OF_WELL; y++) {
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW, UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + y * BLOCK_SIZE), sf::Color(220, 220, 220)),
			sf::Vertex(sf::Vector2f(SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + WIDTH_OF_WELL * BLOCK_SIZE, UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + y * BLOCK_SIZE), sf::Color(220, 220, 220))
		};
		window.draw(line, 2, sf::Lines);
	}

	// Draw placed blocks
	sf::RectangleShape block(sf::Vector2f(BLOCK_SIZE - 1, BLOCK_SIZE - 1));
	for (int y = 0; y < HEIGHT_OF_WELL; ++y) {
		for (int x = 0; x < WIDTH_OF_WELL; ++x) {
			if (well[y][x] != sf::Color::Black) {
				block.setFillColor(well[y][x]);
				block.setPosition(
					SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + x * BLOCK_SIZE,
					UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + y * BLOCK_SIZE
				);
				window.draw(block);
			}
		}
	}

	// Draw current shape
	if (!isGameOver) {
		currentShape->draw(window);
	}

	// Draw INFO  BOXES on the right side
	const float width_of_blocks = 200;
	const float height_of_block = 50;
	const float upward_distance = UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + 5;
	const float boxSpacing = 1;

	// Level box (top)
	drawInfoBox("LEVEL:", to_string(level), 2 * upward_distance);

	// Score box (middle)
	drawInfoBox("SCORE:", to_string(score) + " points", upward_distance + (height_of_block + boxSpacing) + 125);

	// Lines box (bottom)
	drawInfoBox("LINES:", to_string(linesCleared), upward_distance + 6 * (height_of_block + boxSpacing));



	// Game over message (restart and pause functionality added additionally)
	if (isGameOver) {
		sf::RectangleShape overlay(sf::Vector2f(WIDTH_OF_WELL * BLOCK_SIZE, HEIGHT_OF_WELL * BLOCK_SIZE));
		overlay.setPosition(SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW, UPWARD_DISTANCE_OF_WELL_FROM_WINDOW);
		overlay.setFillColor(sf::Color(0, 0, 0, 150));
		window.draw(overlay);

		sf::Text gameOverText("GAME OVER", font, 36);
		gameOverText.setPosition(
			SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + WIDTH_OF_WELL * BLOCK_SIZE / 2 - gameOverText.getLocalBounds().width / 2,
			UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + HEIGHT_OF_WELL * BLOCK_SIZE / 2 - 30
		);
		gameOverText.setFillColor(sf::Color::White);
		window.draw(gameOverText);

		sf::Text restartText("Press \"R\" to restart", font, 24);
		restartText.setPosition(
			SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + WIDTH_OF_WELL * BLOCK_SIZE / 2 - restartText.getLocalBounds().width / 2,
			UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + HEIGHT_OF_WELL * BLOCK_SIZE / 2 + 20
		);
		restartText.setFillColor(sf::Color::White);
		window.draw(restartText);
	}
	if (isPaused) {
		sf::RectangleShape overlay(sf::Vector2f(WIDTH_OF_WELL * BLOCK_SIZE, HEIGHT_OF_WELL * BLOCK_SIZE));
		overlay.setPosition(SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW, UPWARD_DISTANCE_OF_WELL_FROM_WINDOW);
		overlay.setFillColor(sf::Color(0, 0, 0, 150));
		window.draw(overlay);

		sf::Text pauseText("PAUSED", font, 36);
		pauseText.setPosition(
			SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + WIDTH_OF_WELL * BLOCK_SIZE / 2 - pauseText.getLocalBounds().width / 2,
			UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + HEIGHT_OF_WELL * BLOCK_SIZE / 2 - 30
		);
		pauseText.setFillColor(sf::Color::White);
		window.draw(pauseText);

		sf::Text resumeText("Press \"P\" to resume", font, 24);
		resumeText.setPosition(
			SIDEWISE_DISTANCE_OF_WELL_FROM_WINDOW + WIDTH_OF_WELL * BLOCK_SIZE / 2 - resumeText.getLocalBounds().width / 2,
			UPWARD_DISTANCE_OF_WELL_FROM_WINDOW + HEIGHT_OF_WELL * BLOCK_SIZE / 2 + 20
		);
		resumeText.setFillColor(sf::Color::White);
		window.draw(resumeText);
	}
	window.display();
}
