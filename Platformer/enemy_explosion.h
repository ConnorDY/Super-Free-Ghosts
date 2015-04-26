#ifndef ENEMY_EXPLOSION_H
#define ENEMY_EXPLOSION_H

#include "object.h"

class EnemyExplosion : public Object
{
	private:
		sf::Sprite sprite;
		std::vector<sf::IntRect> animation;
		double frame;
	public:
		EnemyExplosion(Room &room, float x, float y);

		// Actions
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif