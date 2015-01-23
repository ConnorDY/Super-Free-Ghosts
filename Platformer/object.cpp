#include "object.h"

Object::Object(float x, float y, float width, float height, float dx, float dy, bool solid, float gravity, float maxFallSpeed)
	: x(x), y(y),
	  dx(dx), dy(dy),
	  width(width), height(height),
	  gravity(gravity),
	  maxFallSpeed(maxFallSpeed),
	  solid(solid),
	  del(false)
{
}

Object::~Object()
{

}


/* Accessors */
bool Object::isSolid() const { return solid; }
bool Object::shouldDelete() const { return del; }

sf::FloatRect Object::getRect() const
{
	return sf::FloatRect(x, y, width, height);
}

bool Object::placeFree(float xx, float yy, std::vector<Object*> const objects) const
{
	sf::FloatRect temp_rect(xx, yy, width, height);

	return std::none_of(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && elem->isSolid() && temp_rect.intersects(elem->getRect());
	});
}

Object* Object::nonsolidCollision(float xx, float yy, std::vector<Object*> const objects) const
{
	sf::FloatRect temp_rect(xx, yy, width, height);

	auto collision = std::find_if(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return this != elem && !elem->isSolid() && temp_rect.intersects(elem->getRect());
	});
	if (collision == objects.end()) return NULL;
	else return *collision;
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


/* Actions */
void Object::update(sf::Time deltaTime, Room const &room)
{
	double mstime = deltaTime.asMicroseconds() / 1000.0;

	// Gravity
	auto const objects = room.getObjects();
	if (placeFree(x, y + 1, objects)) dy += gravity * (float)mstime;
	else if (dy > 0.0f) dy = 0.0f;

	if (dy > maxFallSpeed) dy = maxFallSpeed;

	// Update Y
	for (float i = fabs(dy) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dy);
		if (placeFree(x, y + j, objects))
		{
			y += j;
			break;
		}
	}

	// Update X
	for (float i = fabs(dx) * (float)mstime; i > 0; i--)
	{
		float j = copysign(i, dx);
		if (placeFree(x + j, y, objects))
		{
			x += j;
			break;
		}
	}
}

void Object::kill(Room const &room)
{
	onDeath(room);
	del = true;
}

void Object::onDeath(Room const &room)
{
}
