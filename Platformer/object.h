#ifndef OBJECT_H
#define OBJECT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "globals.h"
#include "texture_manager.h"

class Object
{
	public:
		enum class Type
		{
			Obj,
			Block,
			Projectile,
			Zombie
		};

		explicit Object(
				Type type = Type::Obj,
				float x = 0, float y = 0,
				float width = 0, float height = 0,
				float dx = 0, float dy = 0,
				bool solid = true,
				float gravity = 0,
				float maxFallSpeed = 0
		);
		virtual ~Object();

		// Mutators
		void setX(float xx);
		void setY(float yy);
		void setDX(float dxx);
		void setDY(float dyy);
		virtual void setWidth(float w);
		virtual void setHeight(float h);
		void setGravity(float g);
		void setSolid(bool s);
		void setDelete(bool d);
		void setType(Object::Type t);

		// Accessors
		float getX() const;
		float getY() const;
		float getDX() const;
		float getDY() const;
		float getWidth() const;
		float getHeight() const;
		float getGravity() const;
		bool isSolid() const;
		bool shouldDelete() const;
		Type getType() const;

		virtual sf::FloatRect getRect() const;
		bool placeFree(float xx, float yy, std::vector<Object*> const objects) const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, std::vector<Object*> const objects);
	private:
		sf::Sprite sprite;
		float x, y, dx, dy, width, height, gravity, maxFallSpeed;
		bool solid, del;
		Type _type;
};

#endif
