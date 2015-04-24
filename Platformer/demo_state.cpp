#include "demo_state.h"
#include "globals.h"
#include "block.h"
#include "chest.h"
#include "handeye.h"
#include "menu_state.h"

#define GRID_SCALE 8

Demo_State::Demo_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, settings_t &settings)
	: LevelState(sM, som, textureManager, settings)
{
	start();
}

Demo_State::~Demo_State()
{
}

void Demo_State::start()
{
	// Background
	shapeGrid.setSize(sf::Vector2f(GRID_SCALE - 2, GRID_SCALE - 2));
	shapeGrid.setOrigin(sf::Vector2f(-1, -1));
	shapeGrid.setFillColor(sf::Color(255, 255, 255, 10));
	shapeGrid.setOutlineThickness(1);

	// Test dialogue
	dialogue = std::make_unique<Dialogue>(std::vector<std::string>({ "Welcome to our game demo! Press X to continue.", "This is another line of text!" }));

	// Blocks
	objects.push_back(new Block(*this, 0, VIEW_HEIGHT - 32, 256, 32));
	objects.push_back(new Block(*this, 256, VIEW_HEIGHT - 64, 64, 64));
	objects.push_back(new Block(*this, 384, VIEW_HEIGHT - 128, 64, 16));
	objects.push_back(new Block(*this, 512, VIEW_HEIGHT - 128, 32, 16));
	objects.push_back(new Block(*this, 576, VIEW_HEIGHT - 48, 256, 48));

	// Chests
	objects.push_back(new Chest(*this, 110.0f, 256));

	// Enemies
	objects.push_back(new HandEye(*this, 650, 200));

	// Create player
	player = new Player(*this, 45.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;
}

bool Demo_State::shouldSpawnMoreZombies() const
{
	return false;
}

static double gridYToWindowY(double gridY) { return gridY * GRID_SCALE; }
static double gridXToWindowX(double gridX) { return gridX * GRID_SCALE; }

void Demo_State::drawBackground(sf::RenderWindow &window)
{
	auto gridXOffset = gridXToWindowX(floor(getViewX() / GRID_SCALE));

	// Grid
	for (int j = 0; j <= ceil(VIEW_HEIGHT / GRID_SCALE); j++)
	{
		for (int i = 0; i <= ceil(VIEW_WIDTH / GRID_SCALE); i++)
		{
			sf::Color col;
			col = sf::Color(255, 255, 255, 50);

			shapeGrid.setOutlineColor(col);
			shapeGrid.setPosition(sf::Vector2f(gridXToWindowX(i) + gridXOffset, gridYToWindowY(j)));
			window.draw(shapeGrid);
		}
	}

	Room::drawBackground(window);
}
