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
#include "damageable_object.h"
#include "player.h"
#include "obelisk.h"

class Zombie : public DamageableObject
{
	private:
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation;
		float moveSpeed, frame, spawnX, spawnY, angle;
		bool inCasket, opening, turning, spawned, under, visible;
		sf::Clock casketTimer, turnTimer, openTimer, flashTimer;

		// Actions
		void setAnimation(std::string name);
		void updateAnimation(sf::Time deltaTime);
	public:
		Zombie(TextureManager &textureManager, float x, float y);

		// Accesors
		int getDir() const;
		bool getInCasket() const;
		virtual bool canCollideWith(const Object* obj) const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, const settings_t &settings);
		virtual void onDeath(Room const &room, const settings_t &settings);
};

#endif
