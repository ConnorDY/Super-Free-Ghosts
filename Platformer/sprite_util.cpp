#include "sprite_util.h"
#include "assert.h"

namespace SpriteUtil
{
	void setOriginAndScale(sf::Sprite &sprite, sf::Vector2f scale, sf::IntRect BB)
	{
		assert(fabs(scale.x) == 1);
		assert(fabs(scale.y) == 1);

		int originX = BB.left;
		int originY = BB.top;

		if (scale.x < 0) originX += BB.width;
		if (scale.y < 0) originY += BB.height;

		sprite.setOrigin(originX, originY);
		sprite.setScale(scale);
	}
}
