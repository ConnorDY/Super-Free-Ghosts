#include "handeye.h"
#include "globals.h"
#include "level_state.h"
#include "obelisk.h"
#include "enemy_explosion.h"

HandEye::HandEye(LevelState &room, float x, float y)
	: DamageableObject(
		room, x, y, 53, 40,
		0.0f, 0.0f, false,
		0.00185f / 2.0f, 0.25f
	),
	animation("awake"),
	pulling(false), awake(false), waking(false),
	frame(0)
{
	for (unsigned int i = 0; i < 7; i++) animations["pull"].emplace_back(0, i * 56, 55, 56);
	for (unsigned int i = 7; i < 17; i++) animations["pulling"].emplace_back(0, i * 56, 55, 56);
	for (unsigned int i = 17; i < 19; i++) animations["turn"].emplace_back(0, i * 56, 55, 56);
	for (unsigned int i = 0; i < 20; i++) animations["awake"].emplace_back(55, i * 40, 53, 40);

	sprite.setTexture(room.textureManager.getRef("handeye"));
	setDepth(-2);
	setHealth(30);
}


/* Accessors */
bool HandEye::canCollideWith(const Object* obj) const
{
	return dynamic_cast<const Obelisk*>(obj) == nullptr;
}

sf::FloatRect HandEye::getRect() const
{
	return sf::FloatRect(x + 9, y + 2, width - 18, height - 2);
}


/* Actions */
void HandEye::draw(sf::RenderWindow &window)
{
	sf::FloatRect boundingRect = getRect();

	if (DEBUG_MODE)
	{
		rectangle.setSize(sf::Vector2f(boundingRect.width, boundingRect.height));
		rectangle.setPosition(boundingRect.left, boundingRect.top);
		window.draw(rectangle);
	}

	float adjx = 0.0f, adjy = 0.0f;

	if (animation != "awake")
	{
		adjx = -2.0f;
		adjy = -10.0f;
	}

	if (sprite.getScale().x < 0.0f) adjx = width - adjx;

	sprite.setPosition(x + adjx, y + adjy);
	window.draw(sprite);
}

void HandEye::update(sf::Time deltaTime)
{
	Object::update(deltaTime);

	auto plyr = static_cast<LevelState&>(room).getPlayer();
	if (plyr != nullptr)
	{
		auto myRect = getRect(), playerRect = plyr->getRect();
		auto myCentre = myRect.left + myRect.width / 2;
		auto playerCentre = playerRect.left + playerRect.width / 2;
		float dist = myCentre - playerCentre;

		int dir = 1;
		if (dist < 0) dir = -1;
		sprite.setScale(sf::Vector2f(dir, 1));

		if (!awake && !waking && std::abs(dist) <= 200.) waking = true;

		if (pulling)
		{
			float maxPull = 1 / 16.2;
			float vel = 1.5 / dist;

			if (std::abs(vel) < maxPull)
				plyr->applyVelocityOnce(deltaTime, vel, 0);
		}
	}

	for (auto col : allCollisions(x, y))
	{
		Player* player = dynamic_cast<Player*>(col);
		if (player != nullptr) player->damage(this, 1);
	}

	if (pulling) setAnimation("pulling");
	else if (awake) setAnimation("pull");
	else setAnimation("awake");

	updateAnimation(deltaTime);
}

void HandEye::setAnimation(std::string name)
{
	if (animation != name)
	{
		animation = name;
		frame = 0.0f;
	}
}

void HandEye::updateAnimation(sf::Time deltaTime)
{
	// Get current animation
	std::vector<sf::IntRect> &anim = animations.at(animation);
	int frames = anim.size();

	// Adjust frame
	if (frames > 1)
	{
		double speed = 12.;

		if (awake || waking) frame += deltaTime.asSeconds() * speed;

		if (frame >= frames - 1)
		{
			if (awake && !pulling)
			{
				pulling = true;
			}
			if (waking)
			{
				waking = false;
				awake = true;
				frame = frames - 1;
			}
		}

		frame = fmodf(frame, (float)frames);
	}

	// Set TextureRect
	sprite.setTextureRect(anim[(int)frame]);
}

void HandEye::onDeath()
{
	room.soundManager.playSound("enemy_die");
	room.spawn(new EnemyExplosion(room, x + 4, y + 4));
}
