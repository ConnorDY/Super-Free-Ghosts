#include "level_state.h"
#include "player.h"
#include "dialogue.h"
#include "globals.h"
#include "zombie.h"
#include "menu_state.h"

LevelState::LevelState(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings),
	  restart(false), dimmed(false),
	  overlayRect(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT))
{
}

LevelState::~LevelState()
{
}

void LevelState::drawForeground(sf::RenderWindow &window)
{
	Room::drawForeground(window);

	if (dialogue != nullptr)
		dialogue->draw(window, getView());
}

int LevelState::countZombies() const
{
	return count_if(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return dynamic_cast<Zombie*>(elem) != nullptr;
	});
}

bool LevelState::shouldSpawnMoreZombies() const
{
	return countZombies() < 10;
}

bool LevelState::shouldUpdate(Object const* object) const
{
	// When the screen is dimmed, update only the player
	if (dimmed && dynamic_cast<Player const*>(object) == nullptr)
		return false;
	return Room::shouldUpdate(object);
}

bool LevelState::shouldDraw(Object const* object) const
{
	// When the screen is dimmed, skip the player for drawing (we are going to draw him on another depth)
	if (dimmed && dynamic_cast<Player const*>(object) != nullptr)
		return false;
	return Room::shouldDraw(object);
}

void LevelState::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
{
	/* Restart Level if Player is Outside of the Room */
	if (player->getRect().top > VIEW_HEIGHT)
	{
		reset(textureManager, settings);
		return;
	}

	/* Restart level if the player dies */
	if (!restart && !player->isAlive())
	{
		restartTimer.restart();
		restart = true;
	}
	else if (restart && restartTimer.getElapsedTime().asSeconds() >= 5)
	{
		restart = false;
		reset(textureManager, settings);
		return;
	}

	if (shouldSpawnMoreZombies()) objects.push_back(new Zombie(textureManager, (float)((double)rand() / (RAND_MAX)) * 1248, 250));

	int moveH = inputHandler.checkInput(InputHandler::Input::Right) - inputHandler.checkInput(InputHandler::Input::Left); // Horizontal Movement
	bool crouching = inputHandler.checkInput(InputHandler::Input::Down); // Crouching

	dimmed = player->isTransforming();

	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		default:
			break;

		case sf::Event::Closed:
			window.close();
			break;
		}

		if (inputHandler.checkInput(InputHandler::Input::Exit, event))
		{
			getStateManager().setState(std::make_unique<Menu_State>(getStateManager(), textureManager));
			return;
		}

		if (inputHandler.checkInput(InputHandler::Input::Up, event))
			player->jump(moveH, soundManager, settings); // Jumping
		if (inputHandler.checkInput(InputHandler::Input::Action, event))
			player->throwWeapon(*this, player->getDir(), soundManager, settings); // Throw Weapon
	}

	player->move(moveH);
	player->setCrouching(crouching);

	if (dialogue != nullptr)
	{
		dialogue->update(inputHandler);
		if (dialogue->isDone())
			dialogue.reset();
	}

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}

void LevelState::draw(sf::RenderWindow &window)
{
	Room::draw(window);

	if (dimmed)
	{
		double fadeTime = 400.0;
		double fadeLength = 400.0;

		if (player != nullptr)
		{
			if (player->isTransforming()) fadeTime = player->getFadeTime();
			if (player->isFadingOut()) fadeLength /= 2.0;

			if (fadeTime < fadeLength)
			{
				overlayRect.setFillColor(sf::Color(0, 0, 0, 100.0 * (fadeLength - fadeTime) / fadeLength));
				overlayRect.setPosition(getViewX(), getViewY());
				window.draw(overlayRect);
			}

			player->draw(window);
		}
	}
}
