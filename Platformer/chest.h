#ifndef CHEST_H
#define CHEST_H

#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>

#include "damageable_object.h"

class Chest : public DamageableObject
{
	private:
		sf::RectangleShape rect;
		sf::Sprite spr;
		std::vector<std::vector<sf::IntRect> > animations;
		std::vector<sf::IntRect> frames;
		int animation;
		float slideUpFraction, // How far it is in its slide between underground and visible
		      frame;
		double leaveTimer;
		bool slidingUp, leaving;
	public:
		explicit Chest(Room &room, float x = -128, float y = -128);

		virtual sf::FloatRect getRect() const override;
		bool isLeaving() const;

		// Actions
		virtual void damage(int dmg) override;
		virtual void update(sf::Time deltaTime) override;
		virtual void draw(sf::RenderWindow &window) override;
		virtual void onDoubleJumpedOver() override;
		void setAnimation(int a);
		void updateAnimation(sf::Time deltaTime);
};

#endif
