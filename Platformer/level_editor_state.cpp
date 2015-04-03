#include "level_editor_state.h"
#include "menu_state.h"

#define GRID_SCALE 8

Level_Editor_State::Level_Editor_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	cursor = sf::Vector2i(0, 0);
	point = cursor;
	clickedL = false;
	clickedR = false;

	shapeCursor.setSize(sf::Vector2f(2, 2));
	shapeCursor.setOrigin(sf::Vector2f(1, 1));
	shapeCursor.setFillColor(sf::Color(255, 0, 0, 180));

	shapeGrid.setSize(sf::Vector2f(GRID_SCALE - 2, GRID_SCALE - 2));
	shapeGrid.setOrigin(sf::Vector2f(-1, -1));
	shapeGrid.setFillColor(sf::Color(255, 255, 255, 10));
	shapeGrid.setOutlineThickness(1);
}

Level_Editor_State::~Level_Editor_State()
{
}

void Level_Editor_State::start(TextureManager &textureManager, const settings_t &settings)
{

}

void Level_Editor_State::drawForeground(sf::RenderWindow &window)
{
	// Grid
	for (unsigned int i = 0; i <= ceil(VIEW_WIDTH / GRID_SCALE); i++)
	{
		for (unsigned int j = 0; j <= ceil(VIEW_HEIGHT / GRID_SCALE); j++)
		{
			shapeGrid.setOutlineColor(sf::Color(255, 255, 255, 50));
			shapeGrid.setPosition(sf::Vector2f((float)i * GRID_SCALE, (float)j * GRID_SCALE));
			window.draw(shapeGrid);
		}
	}

	// Heightmap
	Room::drawForeground(window);

	// Highlighted Grid Part
	shapeGrid.setOutlineColor(sf::Color(0, 216, 216, 100));
	shapeGrid.setPosition(sf::Vector2f((float)gridCursor.x * GRID_SCALE, (float)gridCursor.y * GRID_SCALE));
	window.draw(shapeGrid);
}

void Level_Editor_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
{
	restartClock();

	// Get Input
	sf::Event event;

	while (window.pollEvent(event))
	{
		switch (event.type)
		{
			default:
				break;

			case sf::Event::Closed:
				window.close();
				break;
		}

		if (inputHandler.checkInput(InputHandler::Input::Exit, event))
		{
			getStateManager().setState(std::unique_ptr<State>(new Menu_State(getStateManager(), textureManager, settings)));
			return;
		}

		/* Left Clicking */
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!clickedL)
			{
				point = gridCursor;
				clickedL = true;
			}
		}
		else if (clickedL)
		{
			if (point.y == gridCursor.y) fillHeightMap(point.x * GRID_SCALE, (gridCursor.x + 1 - point.x) * GRID_SCALE, abs(VIEW_HEIGHT - (point.y * GRID_SCALE)));
			else
			{
				createSlope(point.x * GRID_SCALE, (gridCursor.x + 1 - point.x) * GRID_SCALE, abs(VIEW_HEIGHT - (point.y * GRID_SCALE)), abs(VIEW_HEIGHT - (gridCursor.y * GRID_SCALE)));
			}

			clickedL = false;
		}

		/* Right Clicking */
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (!clickedR)
			{
				point = gridCursor;
				clickedR = true;
			}
		}
		else if (clickedR)
		{
			if (point.y == gridCursor.y) fillHeightMap(point.x * GRID_SCALE, (gridCursor.x + 1 - point.x) * GRID_SCALE, 0);

			clickedR = false;
		}
	}

	// Get mouse position
	sf::Vector2i m = sf::Mouse::getPosition(window);
	cursor = sf::Vector2i((int)floor(m.x / (float)settings.window_scale), (int)floor(m.y / (float)settings.window_scale));
	gridCursor = sf::Vector2i((int)floor(cursor.x / GRID_SCALE), (int)floor(cursor.y / GRID_SCALE));

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}