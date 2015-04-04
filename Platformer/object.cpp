#include "object.h"

Object::Object(float x, float y, float width, float height, float dx, float dy, bool solid, float gravity, float maxFallSpeed)
	: x(x), y(y),
	  dx(dx), dy(dy),
	  width(width), height(height),
	  gravity(gravity),
	  maxFallSpeed(maxFallSpeed),
	  solid(solid),
	  del(false),
	  depth(0)
{
}

Object::~Object()
{

}


/* Accessors */
int Object::getDepth() const { return depth; }
bool Object::isSolid() const { return solid; }
bool Object::shouldDelete() const { return del; }

sf::FloatRect Object::getRect() const
{
	return sf::FloatRect(x, y, width, height);
}

bool Object::placeFree(float xx, float yy, Room const &room) const
{
	sf::FloatRect temp_rect(xx, yy, width, height);

	if (room.heightmapIntersects(temp_rect)) return false;
	if (room.exceedsHorizontalBounds(temp_rect)) return false;

	auto const &objects = room.getObjects();
	return std::none_of(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && elem->isSolid() && temp_rect.intersects(elem->getRect()) && this->canCollideWith(elem) && elem->canCollideWith(this);
	});
}

Object* Object::solidCollision(float xx, float yy, Room const &room) const
{
	sf::FloatRect temp_rect(xx, yy, width, height);

	auto const &objects = room.getObjects();
	auto collision = std::find_if(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && elem->isSolid() && temp_rect.intersects(elem->getRect());
	});
	if (collision == objects.end()) return NULL;
	else return *collision;
}

Object* Object::nonsolidCollision(float xx, float yy, Room const &room) const
{
	sf::FloatRect temp_rect(xx, yy, width, height);

	auto const &objects = room.getObjects();
	auto collision = std::find_if(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && !elem->isSolid() && temp_rect.intersects(elem->getRect());
	});
	if (collision == objects.end()) return NULL;
	else return *collision;
}

bool Object::canCollideWith(const Object *obj) const
{
	return true;
}


/* Mutators*/
void Object::setX(float xx)
{
	x = xx;
}

void Object::setY(float yy)
{
	y = yy;
}

void Object::setDepth(int d)
{
	depth = d;
}


/* Actions */
void Object::update(sf::Time deltaTime, Room const &room, const settings_t &settings)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;

	// Gravity
	if (placeFree(x, y + 1, room)) dy += gravity * (float)mstime;
	else if (dy > 0.0f) dy = 0.0f;

	if (dy > maxFallSpeed) dy = maxFallSpeed;

	// Update Y
	for (float i = fabs(dy) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dy);
		if (placeFree(x, y + j, room))
		{
			y += j;
			break;
		}
	}

	// Update X
	for (float i = fabs(dx) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dx);
		bool brk = false;

		float ks = 0;
		float ke = 0;

		if (dy == 0 && maxFallSpeed > 0)
		{
			ks = -4;
			ke = 4;
		}

		for (float k = ks; k <= ke; k++)
		{
			if (placeFree(x + j, y - k, room))
			{
				x += j;
				y -= k;

				brk = true;
				break;
			}
		}

		if (brk) break;
	}

	// Move out of heightmap if stuck within it
	while (room.heightmapIntersects(sf::FloatRect(x, y, width, height))) y -= .5;
}

void Object::kill(Room const &room, const settings_t &settings)
{
	onDeath(room, settings);
	del = true;
}

void Object::onDeath(Room const&, const settings_t &settings)
{
}
