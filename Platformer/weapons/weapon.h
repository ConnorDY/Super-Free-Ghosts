#ifndef WEAPON_H
#define WEAPON_H

#include <set>
#include "object.h"

class Weapon : public Object {
	private:
		void collisionCheck();
	protected:
		sf::RectangleShape rectangle;
		bool isOutsideView(float leeway = 30) const;
		int dmg;
		int animationCycle;
		float animationFrame, animationSpeed;
		std::vector<sf::IntRect> animationFrames;
		bool destroyedOnHit, destroyedOnSolidCollision, destroyedOnExitView;
		std::set<Object*> alreadyAttacked;

		virtual bool canDamage();
		virtual void onHit();
		virtual void onHitSolid();
	public:
		Weapon(Room &room, float x, float y, float width, float height, float dx, float dy, float gravity, float maxFallSpeed, int damage);
		virtual ~Weapon();

		void doNotDamage(Object *what);
		virtual void move(sf::Time deltaTime);
		virtual void draw(sf::RenderWindow &window) override;
		virtual void update(sf::Time deltaTime) override;
};

#endif
