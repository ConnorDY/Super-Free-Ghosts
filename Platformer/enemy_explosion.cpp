#include "enemy_explosion.h"
#include "room.h"

EnemyExplosion::EnemyExplosion(Room &room, float x, float y)
	: Object(
		room, x, y, width, height
	),
	frame(0)
{
	for (unsigned int i = 0; i < 11; i++) animation.emplace_back(i * 33, 33, 33, 33);

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
	int frames = animation.size();
	frame += deltaTime.asSeconds() * 12.;

	if (frame >= frames)
	{
		frame = frames - 1;
		kill();
	}

	sprite.setTextureRect(animation[(int)frame]);
}