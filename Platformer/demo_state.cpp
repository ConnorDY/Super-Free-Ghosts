#include "demo_state.h"
#include "globals.h"
#include "block.h"
#include "chest.h"
#include "menu_state.h"

Demo_State::Demo_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings), restart(false)
{
	start(textureManager, settings);
}

Demo_State::~Demo_State()
{
}

void Demo_State::start(TextureManager &textureManager, const settings_t &)
{
	// Test dialogue
	dialogue = new Dialogue(std::vector<std::string>({ "Welcome to our game demo! Press X to continue.", "This is another line of text!" }));

	// Blocks
	objects.push_back(new Block(0, VIEW_HEIGHT - 32, 256, 64));

	// Chests
	objects.push_back(new Chest(textureManager, 150.0f, 234.0f));

	// Create player
	player = new Player(textureManager, 45.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;
}

void Demo_State::drawBackground(sf::RenderWindow &window)
{
	Room::drawBackground(window);
}

void Demo_State::drawForeground(sf::RenderWindow &window)
{
	Room::drawForeground(window);

	dialogue->draw(window, getView());
}

void Demo_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
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

	sf::Event event;

	int moveH = inputHandler.checkInput(InputHandler::Input::Right) - inputHandler.checkInput(InputHandler::Input::Left); // Horizontal Movement
	bool crouching = inputHandler.checkInput(InputHandler::Input::Down); // Crouching

	setDimmed(player->isTransforming());

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
			getStateManager().setState(std::unique_ptr<State>(new Menu_State(getStateManager(), textureManager)));
			return;
		}

		if (inputHandler.checkInput(InputHandler::Input::Up, event)) player->jump(moveH, soundManager, settings); // Jumping
		if (inputHandler.checkInput(InputHandler::Input::Action, event)) player->throwWeapon(*this, player->getDir(), soundManager, settings); // Throw Weapon
	}

	player->move(moveH);
	player->setCrouching(crouching);

	dialogue->update(inputHandler);

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}
