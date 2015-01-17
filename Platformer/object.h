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
			Zombie,
			Player
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

		// Accessors
		bool isSolid() const;
		bool shouldDelete() const;
		bool getOutsideView() const;
		Type getType() const;

		sf::FloatRect getRect() const;
		bool placeFree(float xx, float yy, std::vector<Object*> const objects) const;
		Object* placeEmpty(float xx, float yy, std::vector<Object*> const objects) const;

		// Actions
		virtual void draw(sf::RenderWindow &window) = 0;
		virtual void update(sf::Time deltaTime, std::vector<Object*> const objects);
		void setDelete();
		void setDelete(int i);
	protected:
		sf::Sprite sprite;
		float x, y, dx, dy, width, height, gravity, maxFallSpeed;
		bool solid, del, outsideView = false;
		Type const _type;
};

#endif
