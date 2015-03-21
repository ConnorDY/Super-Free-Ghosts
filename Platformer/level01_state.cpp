#include "level01_state.h"
#include "menu_state.h"

Level01_State::Level01_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	start(textureManager, settings);
	bg.setTexture(textureManager.getRef("bg01"));
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
	createSlope(256, 128, 64, 96);
	createSlope(384, 128, 96, 64);
	fillHeightMap(512, 96, 64);
	createSlope(608, 128, 64, 96);
	fillHeightMap(736, 64, 96);
	createSlope(800, 128, 96, 128);
	fillHeightMap(928, 64, 128);
	createSlope(992, 192, 128, 64);

	// Create Objects
	Object *object = new Zombie(textureManager, 270.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 405.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 850.0f, 32.0f);
	objects.push_back(object);

	object = new Obelisk(textureManager, 280.0f, 280.0f, 50.0f);
	objects.push_back(object);

	// Create player
	player = new Player(textureManager, 15.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;
}

void Level01_State::draw(sf::RenderWindow &window)
{
	for (size_t i = 0; i < 4; i++)
	{
		bg.setPosition(sf::Vector2f(getViewX() + fmod((-getViewX() / 2), 192) + (i * 192), getViewY() + 98));
		window.draw(bg);
	}

	Room::draw(window);
}

void Level01_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
{
	static sf::Clock clock;

	/* Restart Level if Player is Outside of the Room */
	if (player->getRect().top > VIEW_HEIGHT) reset(textureManager, settings);

	auto time = clock.getElapsedTime();

	/*ensureHeightmapWidth(816 + 256);
	for (size_t i = 0; i < 256; i++)
	{
		heightmap[i + 816] = 30 + 20 * sin((i + time.asMilliseconds()) / 200.);
	}*/

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
