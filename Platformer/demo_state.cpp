#include "demo_state.h"
#include "globals.h"
#include "block.h"
#include "chest.h"
#include "menu_state.h"

Demo_State::Demo_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: LevelState(sM, som, textureManager, settings)
{
	start(textureManager, settings);
}

Demo_State::~Demo_State()
{
}

void Demo_State::start(TextureManager &textureManager, const settings_t &)
{
	// Test dialogue
	dialogue = std::make_unique<Dialogue>(std::vector<std::string>({ "Welcome to our game demo! Press X to continue.", "This is another line of text!" }));

	// Blocks
	objects.push_back(new Block(0, VIEW_HEIGHT - 32, 256, 64));

	// Chests
	objects.push_back(new Chest(textureManager, 150.0f, 234.0f));

	// Create player
	player = new Player(textureManager, 45.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;
}

bool Demo_State::shouldSpawnMoreZombies() const
{
	return false;
}
