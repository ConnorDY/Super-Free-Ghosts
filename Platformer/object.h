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
			Projectile
		};

		explicit Object(
				Type type = Type::Obj,
				float x = 0, float y = 0,
				float width = 0, float height = 0,
				float dx = 0, float dy = 0,
				bool solid = true, bool gravity = true,
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
		void setSolid(bool s);
		void setDelete(bool d);
		void setType(Object::Type t);

		// Accessors
		float getX();
		float getY();
		float getDX();
		float getDY();
		float getWidth();
		float getHeight();
		bool isSolid();
		bool shouldDelete();
		Type getType() const;

		sf::FloatRect getRect();
		bool placeFree(float xx, float yy, std::vector<Object*> const objects) const;

		// Actions
		virtual void draw(sf::RenderWindow &window);
		virtual void update(sf::Time deltaTime, std::vector<Object*> objects);
	private:
		sf::Sprite sprite;
		float x, y, dx, dy, width, height, maxFallSpeed;
		bool solid, del, gravity;
		Type _type;
};

#endif
