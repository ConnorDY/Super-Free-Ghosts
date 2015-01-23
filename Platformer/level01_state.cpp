#include "level01_state.h"
#include "menu_state.h"

Level01_State::Level01_State(StateManager &sM, TextureManager &textureManager)
	: Room(sM)
{
	start(textureManager);
	scanlines.setTexture(textureManager.getRef("scanlines"));
}

Level01_State::~Level01_State()
{
}


// Actions
void Level01_State::start(TextureManager &textureManager)
{
	// Ready Music
	if (music.openFromFile("res/01.ogg"))
	{
		music.play();
		music.setLoop(true);
		music.setVolume(50.0f);
	}

	// Create Objects
	Object *object = new Block(textureManager, 0.0f, 284.0f, 128.0f, 16.0f);
	objects.push_back(object);

	object = new Block(textureManager, 128.0f, 268.0f, 128.0f, 32.0f);
	objects.push_back(object);

	object = new Block(textureManager, 256.0f, 252.0f, 64.0f, 48.0f);
	objects.push_back(object);

	object = new Block(textureManager, 384.0f, 284.0f, 128.0f, 16.0f);
	objects.push_back(object);

	object = new Block(textureManager, 576.0f, 284.0f, 32.0f, 16.0f);
	objects.push_back(object);

	object = new Block(textureManager, 736.0f, 284.0f, 32.0f, 16.0f);
	objects.push_back(object);

	object = new Block(textureManager, 816.0f, 252.0f, 256.0f, 48.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 270.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 405.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 850.0f, 32.0f);
	objects.push_back(object);

	// Create player
	player = new Player(textureManager, 15.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;
}

void Level01_State::draw(sf::RenderWindow &window)
{
	Room::draw(window);

	//scanlines.setPosition(sf::Vector2f(getViewX(), getViewY()));
	//window.draw(scanlines);
}

void Level01_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{
	/* Restart Level if Player is Outside of the Room */
	if (player->getRect().top > VIEW_HEIGHT) reset(textureManager);

	/* Input */
	sf::Event event;

	int moveH = inputHandler.checkInput(InputHandler::Input::Right) - inputHandler.checkInput(InputHandler::Input::Left); // Horizontal Movement
	bool crouching = inputHandler.checkInput(InputHandler::Input::Down); // Crouching

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

		if (inputHandler.checkInput(InputHandler::Input::Up, event)) player->jump(moveH, soundManager); // Jumping
		if (inputHandler.checkInput(InputHandler::Input::Action, event)) player->throwWeapon(objects, player->getDir(), textureManager, soundManager); // Throw Weapon

		if (inputHandler.checkInput(InputHandler::Input::Debug0, event)) player->changeTexture(textureManager, "arthur0");
	}

	player->move(moveH);
	player->setCrouching(crouching);

	Room::update(window, textureManager, soundManager, inputHandler);
}
