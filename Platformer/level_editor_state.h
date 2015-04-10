#ifndef LEVEL_EDITOR_STATE_H
#define LEVEL_EDITOR_STATE_H

#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>

#include "settings.h"
#include "player.h"
#include "room.h"

class Level_Editor_State : public Room
{
	private:
		sf::Clock restartTimer;
		sf::Vector2i cursor, gridCursor, point;
		sf::RectangleShape shapeCursor, shapeGrid;
		bool clickedL;

		Player *player;
	public:
		Level_Editor_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings);
		virtual ~Level_Editor_State();

		// Actions
		void start(TextureManager &textureManager, const settings_t &settings);
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void updateView(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings);
};

#endif
