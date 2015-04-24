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
		Level_Editor_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, settings_t &settings);
		virtual ~Level_Editor_State();

		// Actions
		void start() override;
		virtual void drawForeground(sf::RenderWindow &window) override;
		virtual void updateView(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif
