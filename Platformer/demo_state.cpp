#include "demo_state.h"
#include "globals.h"
#include "block.h"
#include "chest.h"
#include "handeye.h"
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
	objects.push_back(new Block(0, VIEW_HEIGHT - 32, 256, 32));
	objects.push_back(new Block(256, VIEW_HEIGHT - 64, 64, 64));
	objects.push_back(new Block(384, VIEW_HEIGHT - 128, 64, 16));
	objects.push_back(new Block(512, VIEW_HEIGHT - 128, 32, 16));
	objects.push_back(new Block(576, VIEW_HEIGHT - 48, 256, 48));

	// Chests
	objects.push_back(new Chest(textureManager, 110.0f, 256));

	// Enemies
	objects.push_back(new HandEye(textureManager, 650, 200));

	// Create player
	player = new Player(textureManager, 45.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;
}

bool Demo_State::shouldSpawnMoreZombies() const
{
	return false;
}
