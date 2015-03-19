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
	textureManager.loadTexture("arthur0", "res/arthur0.png");
	textureManager.loadTexture("arthur1", "res/arthur1.png");
	textureManager.loadTexture("weapons", "res/weapons.png");
	textureManager.loadTexture("zombie", "res/zombie.png");
	textureManager.loadTexture("tiles", "res/tiles.png");
	textureManager.loadTexture("logo", "res/logo.png");
	textureManager.loadTexture("scanlines", "res/scanlines.png");

	soundManager.loadSound("jump", "res/jump.wav");
	soundManager.loadSound("land", "res/land.wav");
	soundManager.loadSound("throw", "res/throw.wav");
	soundManager.loadSound("hit", "res/hit.wav");
	soundManager.loadSound("enemy_die", "res/enemy_die.wav");
}


/* Program Entry Point */
int main()
{
	// Default settings
	settings_t settings;
	settings.window_scale = 2;

	// Load Textures and Sounds and Shaders
	TextureManager textureManager;
	SoundManager soundManager;
	loadRes(textureManager, soundManager);

	// Create State Manager
	StateManager stateManager(textureManager, soundManager, settings);
	
	// Create Window
	sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH * settings.window_scale, VIEW_HEIGHT * settings.window_scale), "Super Free Ghosts", sf::Style::Close | sf::Style::Titlebar);
	window.setVerticalSyncEnabled(VSYNC_ON);

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
