#include "room.h"
#include "player.h" // TODO remove
#include "projectile.h" // TODO remove

Room::Room(StateManager &stm, SoundManager &som) : State(stm), soundManager(som)
{
	setView(sf::View(sf::Vector2f(VIEW_WIDTH / 2.0, VIEW_HEIGHT / 2.0), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT)));
}

Room::~Room()
{
	end();
}

SoundManager& Room::getSoundManager() const
{
	return soundManager;
}

std::vector<Object*> const Room::getObjects() const
{
	return objects;
}

void Room::end()
{
	view_follow = nullptr;
	for (Object* i : objects) delete i;
	objects.clear();
}

void Room::reset(TextureManager &textureManager)
{
	end();
	start(textureManager);
}

void Room::draw(sf::RenderWindow &window)
{
	if (view_follow)
	{
		// Update view
		auto rect = view_follow->getRect();
		float vx = rect.left + rect.width/2;
		float vy = rect.top + rect.height/2;
		auto view_size = getView().getSize();
		float vw = view_size.x;
		float vh = view_size.y;

		// Don't let the view go past the top or left of the room
		if (vx < vw / 2.0f) vx = vw / 2.0f;
		if (vy < vh / 2.0f) vy = vh / 2.0f;

		getView().setCenter(vx, vh / 2.0f);
		window.setView(getView());
	}

	for (auto object : objects)
		object->draw(window);
}

void Room::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{
	deltaTime = restartClock();

	// Update objects and player
	auto iter = objects.begin();
	auto end = objects.end();
	while (iter != end)
	{
		if ((*iter)->shouldDelete())
		{
			delete *iter;
			iter = objects.erase(iter);
			end = objects.end();
			continue;
		}

		(*iter)->update(deltaTime, *this);
		iter++;
	}

	// Output time it took to render this frame
	std::cout << "Time: " << (deltaTime.asMicroseconds() / 1000.0) << std::endl;
}
