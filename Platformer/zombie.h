#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>

#include "globals.h"
#include "texture_manager.h"
#include "sound_manager.h"
#include "object.h"

class Zombie : public Object
{
	private:
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation;
		float moveSpeed, frame, spawnX, spawnY, angle;
		bool inCasket, opening, turning, spawned;
		sf::Clock casketTimer, turnTimer, openTimer;

		// Actions
		void setAnimation(std::string name);
		void updateAnimation(sf::Time deltaTime);
	public:
		Zombie(TextureManager &textureManager, float x, float y);

		// Accesors
		int getDir() const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room const &room, const settings_t &settings);
		virtual void onDeath(Room const &room, const settings_t &settings);
};

#endif
