#ifndef LEVEL_STATE_H
#define LEVEL_STATE_H

#include <SFML/Audio.hpp>
#include <algorithm>
#include <vector>
#include <memory>

struct settings_t;
class Player;
class Dialogue;
#include "room.h"

class LevelState : public Room
{
	private:
		sf::Clock restartTimer;
		bool restart;
	protected:
		std::unique_ptr<Dialogue> dialogue;
		Player *player;
		virtual bool shouldSpawnMoreZombies() const;
	public:
		LevelState(StateManager &sM, SoundManager &som, TextureManager &textureManager, settings_t &settings);
		virtual ~LevelState();

		int countZombies() const;

		// Actions
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler);
};

#endif
