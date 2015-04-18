#ifndef SPRITE_UTIL_H
#define SPRITE_UTIL_H
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

namespace SpriteUtil
{
	/** Sets the given sprite's origin to one of the corners of the BB, depending on how it has been flipped. This allows the BB to remain valid despite flipping. It then sets the scale to perform that flipping. */
	void setOriginAndScale(sf::Sprite &sprite, sf::Vector2f scale, sf::IntRect BB);
}

#endif
