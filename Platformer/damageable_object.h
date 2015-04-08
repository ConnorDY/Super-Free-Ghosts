#ifndef DAMAGEABLE_OBJECT_H
#define DAMAGEABLE_OBJECT_H

#include <SFML/Graphics.hpp>
#include "texture_manager.h"
#include "object.h"
#include "settings.h"

namespace Team {
	enum Alignment
	{
		Player, Enemy, Neutral
	};
}

class DamageableObject : public Object
{
	public:
		explicit DamageableObject(
			float x = 0, float y = 0,
			float width = 0, float height = 0,
			float dx = 0, float dy = 0,
			bool solid = true,
			float gravity = 0,
			float maxFallSpeed = 0
			);
		virtual ~DamageableObject();

		// Mutators
		void setHealth(int h);

		// Actions
		virtual void damage(Room &room, const settings_t &settings);
		virtual void draw(sf::RenderWindow &window) = 0;
		virtual void update(sf::Time deltaTime, Room const &room, const settings_t &settings);

	private:
		int health;
		Team::Alignment team;
};

#endif
