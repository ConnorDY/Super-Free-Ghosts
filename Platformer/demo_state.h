#ifndef DEMO_STATE_H
#define DEMO_STATE_H

#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>

#include "settings.h"
#include "player.h"
#include "dialogue.h"
#include "level_state.h"

class Demo_State : public LevelState
{
	private:
		sf::RectangleShape shapeGrid;
	protected:
		virtual bool shouldSpawnMoreZombies() const;
	public:
		Demo_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings);
		virtual ~Demo_State();

		// Actions
		void start(TextureManager &textureManager, const settings_t &settings);
		virtual void drawBackground(sf::RenderWindow &window);
};

#endif
