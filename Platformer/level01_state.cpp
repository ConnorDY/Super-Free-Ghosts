#include "level01_state.h"
#include "menu_state.h"

Level01_State::Level01_State(StateManager &sM, SoundManager &som, TextureManager &textureManager)
	: Room(sM, som, textureManager)
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

	fillHeightMap(0, 128, 16);
	fillHeightMap(128, 128, 32);
	fillHeightMap(256, 64, 48);
	fillHeightMap(384, 128, 16);
	fillHeightMap(576, 32, 16);
	fillHeightMap(736, 32, 16);

	for (double i = 816; i < 1072; i += .25)
	{
		fillHeightMap((size_t)i, 1, 48 + (int)(4 * sin((i - 816.0) / (3.14 * 2))));
	}

	// Create Objects
	Object *object = new Zombie(textureManager, 270.0f, 32.0f);
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
