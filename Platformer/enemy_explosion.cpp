#include "enemy_explosion.h"
#include "room.h"

EnemyExplosion::EnemyExplosion(Room &room, float x, float y)
	: Object(
		room, x, y, width, height
	),
	animation("1"), frame(0)
{
	for (unsigned int i = 0; i < 8; i++) animations["1"].emplace_back(i * 33, 0, 33, 33);
	for (unsigned int i = 0; i < 11; i++) animations["2"].emplace_back(i * 33, 33, 33, 33);

	sprite.setTexture(room.textureManager.getRef("enemydie"));
	setDepth(-3);
}

/* Actions */
void EnemyExplosion::draw(sf::RenderWindow &window)
{
	sprite.setPosition(x, y);
	window.draw(sprite);
}

void EnemyExplosion::update(sf::Time deltaTime)
{
	auto anim = animations.at(animation);
	int frames = anim.size();
	frame += deltaTime.asSeconds() * 12.;

	if (frame >= frames)
	{
		frame = frames - 1;
		kill();
	}

	sprite.setTextureRect(anim[(int)frame]);
}