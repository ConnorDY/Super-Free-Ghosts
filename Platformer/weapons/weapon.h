#ifndef WEAPON_H
#define WEAPON_H

#include "object.h"

class Weapon : public Object {
	protected:
		sf::RectangleShape rectangle;
		bool isOutsideView(float leeway = 30) const;
		int dmg;
		float animationFrame, animationSpeed;
		std::vector<sf::IntRect> animationFrames;
		bool destroyedOnHit; // whether this dies when it hits an enemy. Nothing to do with solids
	public:
		Weapon(Room &room, float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, int damage);
		virtual ~Weapon();

		virtual void move(sf::Time deltaTime);
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void onDeath() override;
};

#endif
