#ifndef HANDEYE_H
#define HANDEYE_H

#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

#include "damageable_object.h"

class HandEye : public DamageableObject
{
	private:
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation;
		bool pulling, awake, waking;
		double frame;

		// Actions
		void setAnimation(std::string name);
		void updateAnimation(sf::Time deltaTime);
	public:
		HandEye(TextureManager &textureManager, float x, float y);

		// Accesors
		virtual bool canCollideWith(const Object* obj) const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, Room &room, const settings_t &settings);
		virtual void onDeath(Room &room, const settings_t &settings);
};

#endif