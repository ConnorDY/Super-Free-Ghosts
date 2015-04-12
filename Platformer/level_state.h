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
		bool dimmed;
		sf::RectangleShape overlayRect;
	protected:
		std::unique_ptr<Dialogue> dialogue;
		Player *player;
		virtual bool shouldSpawnMoreZombies() const;
		virtual bool shouldUpdate(Object const* object) const;
		virtual bool shouldDraw(Object const* object) const;
	public:
		LevelState(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings);
		virtual ~LevelState();

		int countZombies() const;

		// Actions
		virtual void drawForeground(sf::RenderWindow &window);
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings);
};

#endif
