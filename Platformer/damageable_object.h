#ifndef DAMAGEABLE_OBJECT_H
#define DAMAGEABLE_OBJECT_H

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
			Room &room,
			float x = 0, float y = 0,
			float width = 0, float height = 0,
			float dx = 0, float dy = 0,
			bool solid = true,
			float gravity = 0,
			float maxFallSpeed = 0
		);
		virtual ~DamageableObject();

		// Accessors
		int getHealth() const;

		// Mutators
		void setHealth(int h);

		// Actions
		virtual void damage(int dmg);
	private:
		int health;
		Team::Alignment team;
};

#endif
