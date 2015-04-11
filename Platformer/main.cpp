#include <SFML/Graphics.hpp>
#include <vector>

#include "globals.h"
#include "settings.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "state_manager.h"


/* Load Game's Resources */
void loadRes(TextureManager &textureManager, SoundManager &soundManager)
{
	auto textures = std::vector<std::pair<std::string, std::string>>
	{
		{ "player0", "res/plyr/plyr_dead.png" },
		{ "player1", "res/plyr/plyr_naked.png" },
		{ "player2", "res/plyr/plyr_silver.png" },
		{ "player3", "res/plyr/plyr_gold.png" },
		{ "transform1", "res/plyr/transform_silver.png" },
		{ "chest1", "res/chest/01.png" },
		{ "weapons", "res/weapons.png" },
		{ "zombie", "res/zombie.png" },
		{ "tiles", "res/tiles.png" },
		{ "obelisk", "res/obelisk.png" },
		{ "bg01", "res/bg01.png" },
		{ "bg02", "res/bg02.png" },
		{ "bg03", "res/bg03.png" },
		{ "under01", "res/under01.png" },
		{ "logo", "res/logo.png" },
	};

	auto sounds = std::vector<std::pair<std::string, std::string>>
	{
		{ "jump", "res/jump.wav" },
		{ "land", "res/land.wav" },
		{ "throw", "res/throw.wav" },
		{ "hit", "res/hit.wav" },
		{ "enemy_die", "res/enemy_die.wav" },
	};

	for (auto &texture : textures) textureManager.loadTexture(texture.first, texture.second);
	for (auto &sound : sounds) soundManager.loadSound(sound.first, sound.second);
}


/* Program Entry Point */
int main()
{
	// Default settings
	settings_t settings;
	settings.window_scale = 2;
	settings.vsync_on = true;
	settings.music_on = true;
	settings.sound_on = true;

	// Load Textures and Sounds and Shaders
	TextureManager textureManager;
	SoundManager soundManager;
	loadRes(textureManager, soundManager);

	// Create State Manager
	StateManager stateManager(textureManager, soundManager, settings);
	
	// Create Window
	sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH * settings.window_scale, VIEW_HEIGHT * settings.window_scale), "Super Free Ghosts", sf::Style::Close | sf::Style::Titlebar);
	window.setVerticalSyncEnabled(settings.vsync_on);

	// Game Loop
	while (window.isOpen())
	{
		// Update State
		stateManager.update(window);

		// Draw the State
		window.clear();
		stateManager.draw(window);
		window.display();
	}

	// Exit
	return 0;
}
