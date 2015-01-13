#include <SFML/Graphics.hpp>
#include <vector>

#include "globals.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "level01_state.h"
#include "projectile.h"
#include "player.h"
#include "block.h"


/* Load Game's Resources */
void loadRes(TextureManager &textureManager, SoundManager &soundManager)
{
	textureManager.loadTexture("arthur0", "res/arthur0.png");
	textureManager.loadTexture("arthur1", "res/arthur1.png");
	textureManager.loadTexture("weapons", "res/weapons.png");

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

	// Load Level01
	Level01_State* state = new Level01_State(textureManager);
	
	// Create Window
	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Platformer", sf::Style::Close | sf::Style::Titlebar);
	//window.setVerticalSyncEnabled(true);

	// Game Loop
	while (window.isOpen())
	{
		// Update State
		state->update(window, textureManager, soundManager);

		// Draw the State
		window.clear();
		state->draw(window);
		window.display();
	}

	delete state;

	// Exit
	return 0;
}
