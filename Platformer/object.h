#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "globals.h"
#include "texture_manager.h"
class Room;
#include "room.h"

class Object
{
	public:
		explicit Object(
				float x = 0, float y = 0,
				float width = 0, float height = 0,
				float dx = 0, float dy = 0,
				bool solid = true,
				float gravity = 0,
				float maxFallSpeed = 0
		);
		virtual ~Object();

		// Accessors
		int getDepth() const;
		bool isSolid() const;
		bool shouldDelete() const;
		virtual bool canCollideWith(const Object* obj) const;

		virtual sf::FloatRect getRect() const;
		bool placeFree(float xx, float yy, Room const &room) const;
		std::vector<Object*> allCollisions(float xx, float yy, Room const &room) const;

		// Mutators
		void setX(float xx);
		void setY(float yy);
		void setDepth(int depth);

		// Actions
		virtual void draw(sf::RenderWindow &window) = 0;
		virtual void update(sf::Time deltaTime, Room const &room, const settings_t &settings);
		virtual void kill(Room const &room, const settings_t &settings);
		virtual void onDeath(Room const &room, const settings_t &settings);
		virtual void onDoubleJumpedOver(Room const &room);
	protected:
		sf::Sprite sprite;
		float x, y, dx, dy, width, height, gravity, maxFallSpeed, type;
		int depth;
		bool solid, del;
};

#endif
