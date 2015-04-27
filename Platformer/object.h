#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>

class Room;
class TextureManager;

class Object
{
	public:
		explicit Object(
				Room &room,
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
		bool placeFree(float xx, float yy) const;
		std::vector<Object*> allCollisions(float xx, float yy) const;

		// Mutators
		void setX(float xx);
		void setY(float yy);
		void setDepth(int depth);

		// Actions
		virtual void draw(sf::RenderWindow &window) = 0;
		virtual void update(sf::Time deltaTime);
		virtual void kill();
		virtual void onDeath();
		virtual void onDoubleJumpedOver();
		void applyGravityOnce(sf::Time deltaTime);
		void applyVelocityOnce(sf::Time deltaTime, float velX, float velY);
		void pushOutOfHeightmap();
		void pushOutOfSolids();
	protected:
		Room &room;
		sf::Sprite sprite;
		float x, y, dx, dy, width, height, gravity, maxFallSpeed, type;
		int depth;
		bool solid, del;
};

#endif
