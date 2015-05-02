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
		sf::Music music_s, music_l;
	protected:
		virtual bool shouldSpawnMoreZombies() const;
	public:
		Demo_State(StateManager &sM, SoundManager &som, TextureManager const &textureManager, settings_t &settings);
		virtual ~Demo_State();

		// Actions
		void start() override;
		virtual void drawBackground(sf::RenderWindow &window) override;
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler) override;
};

#endif
