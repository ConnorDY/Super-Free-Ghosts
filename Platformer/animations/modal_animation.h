#ifndef MODAL_ANIMATION_H
#define MODAL_ANIMATION_H
#include <SFML/Graphics.hpp>
#include "globals.h"
#include "settings.h"
class Room;
class Object;

class ModalAnimation
{
	protected:
		bool m_finished;
		float currentFrame, animationSpeed;
	public:
		Room const &room;

		ModalAnimation(Room const &room);
		virtual ~ModalAnimation();

		virtual bool finished();
		virtual bool shouldDraw(Object const* obj);

		virtual void draw(sf::RenderWindow &window) = 0;
		virtual void update(sf::Time deltaTime);
};

#endif
