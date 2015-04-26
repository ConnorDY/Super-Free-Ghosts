#ifndef ZOMBIE_H
#define ZOMBIE_H

#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <vector>
#include <map>

#include "damageable_object.h"

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
		Zombie(Room &room, float x, float y);

		// Accesors
		int getDir() const;
		bool getInCasket() const;
		virtual bool canCollideWith(const Object* obj) const override;

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
		virtual void damage(int dmg) override;
};

#endif
