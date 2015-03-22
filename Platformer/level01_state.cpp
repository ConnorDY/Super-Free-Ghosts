#include "level01_state.h"
#include "menu_state.h"

Level01_State::Level01_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	start(textureManager, settings);
	bg01.setTexture(textureManager.getRef("bg01"));
	bg02.setTexture(textureManager.getRef("bg02"));
	bg03.setTexture(textureManager.getRef("bg03"));
	under01.setTexture(textureManager.getRef("under01"));
}

Level01_State::~Level01_State()
{
}


// Actions
void Level01_State::start(TextureManager &textureManager, const settings_t &settings)
{
	// Ready Music
	if (settings.music_on && music.openFromFile("res/01.ogg"))
	{
		music.play();
		music.setLoop(true);
		music.setVolume(50.0f);
	}

	fillHeightMap(0, 512, 64);
	createSlope(256, 96, 64, 96);
	createSlope(352, 96, 96, 64);
	fillHeightMap(448, 96, 64);
	createSlope(544, 128, 64, 96);
	fillHeightMap(672, 64, 96);
	createSlope(736, 128, 96, 128);
	fillHeightMap(864, 64, 128);
	createSlope(928, 192, 128, 64);
	createSlope(1120, 128, 64, 80);
	createSlope(1248, 128, 80, 64);


	// Create Objects
	Object *object = new Zombie(textureManager, 240.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 405.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 850.0f, 32.0f);
	objects.push_back(object);

	object = new Obelisk(textureManager, 287.0f, 280.0f, 54.0f);
	objects.push_back(object);

	object = new Obelisk(textureManager, 660.0f, 280.0f, 50.0f);
	objects.push_back(object);

	// Create player
	player = new Player(textureManager, 15.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;
}

void Level01_State::drawTree(int x, int y, sf::RenderWindow &window)
{
	bg02.setPosition(sf::Vector2f(x, y));
	window.draw(bg02);
}

void Level01_State::drawDecor(int x, int y, int type, sf::RenderWindow &window)
{
	bg03.setPosition(x, y);

	switch (type)
	{
		default:
			break;

		// Skull and Casket #1
		case 0:
			bg03.setTextureRect(sf::IntRect(96, 0, 48, 63));
			break;

		// Skull and Casket #2
		case 1:
			bg03.setTextureRect(sf::IntRect(0, 0, 96, 63));
			break;

		// Casket on Skulls
		case 2:
			bg03.setTextureRect(sf::IntRect(144, 0, 56, 63));
			break;
		
		// Caskets and Banner
		case 3:
			bg03.setTextureRect(sf::IntRect(200, 0, 96, 63));
			break;

		// Banner
		case 4:
			bg03.setTextureRect(sf::IntRect(296, 0, 40, 63));
			break;
	}

	window.draw(bg03);
}

void Level01_State::drawBackground(sf::RenderWindow &window)
{
	// TODO: hardcoded assumption of view width to background width ratio
	// TODO: hardcoded background width
	for (size_t i = 0; i < 4; i++)
	{
		bg01.setPosition(sf::Vector2f(getViewX() + fmod((-getViewX() / 2), 192) + (i * 192), getViewY() + 98));
		window.draw(bg01);
	}

	// Decorations
	drawDecor(10, 171, 0, window);
	drawTree(-4, 50, window);
	drawDecor(100, 171, 4, window);
	drawDecor(160, 171, 1, window);
	drawDecor(340, 151, 2, window);
	drawTree(430, 50, window);
	drawDecor(470, 171, 1, window);
	drawDecor(590, 151, 4, window);
	drawDecor(690, 147, 3, window);
	
	// Background
	Room::drawBackground(window);
}

void Level01_State::drawForeground(sf::RenderWindow &window)
{
	Room::drawForeground(window);

	under01.setPosition(sf::Vector2f(320, 275));
	window.draw(under01);
}

void Level01_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
{
	/* Restart Level if Player is Outside of the Room */
	if (player->getRect().top > VIEW_HEIGHT) reset(textureManager, settings);

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
			getStateManager().setState(std::unique_ptr<State>(new Menu_State(getStateManager(), textureManager, settings)));
			return;
		}

		if (inputHandler.checkInput(InputHandler::Input::Up, event)) player->jump(moveH, soundManager, settings); // Jumping
		if (inputHandler.checkInput(InputHandler::Input::Action, event)) player->throwWeapon(objects, player->getDir(), textureManager, soundManager, settings); // Throw Weapon

		if (inputHandler.checkInput(InputHandler::Input::Debug0, event)) player->changeTexture(textureManager, "arthur0");
	}

	player->move(moveH);
	player->setCrouching(crouching);

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}
