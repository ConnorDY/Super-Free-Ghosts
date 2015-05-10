#ifndef HANDEYE_H
#define HANDEYE_H

#include <SFML/Audio.hpp>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

#include "damageable_object.h"
#include "player.h"
#include "level_state.h"

class HandEye : public DamageableObject
{
	private:
		sf::Sprite sprite;
		sf::RectangleShape rectangle;
		std::map<std::string, std::vector<sf::IntRect> > animations;
		std::string animation;
		bool pulling, awake, waking, turning;
		double frame;

		// Actions
		void setAnimation(std::string name);
		void updateAnimation(sf::Time deltaTime);
	public:
		HandEye(LevelState &room, float x, float y);

		// Accesors
		virtual bool canCollideWith(const Object* obj) const override;
		virtual sf::FloatRect getRect() const override;

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif
