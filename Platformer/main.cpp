#include <SFML/Graphics.hpp>
#include <vector>

#include "globals.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "state_manager.h"


/* Load Game's Resources */
void loadRes(TextureManager &textureManager, SoundManager &soundManager)
{
	textureManager.loadTexture("arthur0", "res/arthur0.png");
	textureManager.loadTexture("arthur1", "res/arthur1.png");
	textureManager.loadTexture("weapons", "res/weapons.png");
	textureManager.loadTexture("tiles", "res/tiles.png");
	textureManager.loadTexture("logo", "res/logo.png");

	soundManager.loadSound("jump", "res/jump.wav");
	soundManager.loadSound("land", "res/land.wav");
	soundManager.loadSound("throw", "res/throw.wav");
}


/* Program Entry Point */
int main()
{
	// Load Textures and Sounds
	TextureManager textureManager;
	SoundManager soundManager;
	loadRes(textureManager, soundManager);

	// Create State Manager
	StateManager stateManager(textureManager, soundManager);
	
	// Create Window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Super Free Ghosts", sf::Style::Close | sf::Style::Titlebar);
	//window.setVerticalSyncEnabled(true);

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
