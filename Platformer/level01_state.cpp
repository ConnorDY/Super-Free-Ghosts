#include "level01_state.h"
#include "menu_state.h"

Level01_State::Level01_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings), restart(false)
{
	start(textureManager, settings);
	bg01.setTexture(textureManager.getRef("bg01"));
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
	createSlope(928, 160, 128, 64);
	createSlope(1088, 112, 64, 80);
	createSlope(1200, 112, 80, 64);
	createSlope(1312, 112, 64, 80);
	fillHeightMap(1424, 224, 80);


	// Create Objects
	Object *object = new Zombie(textureManager, 240.0f, 250.0f/*32.0f*/);
	objects.push_back(object);

	object = new Zombie(textureManager, 405.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 625.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 675.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 725.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 775.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 1275.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 1300.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 1325.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 1350.0f, 32.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 1475.0f, 32.0f);
	objects.push_back(object);

	objects.push_back(new Chest(textureManager, 287 + 5, 280));
	object = new Obelisk(textureManager, 287.0f, 280.0f, 54.0f);
	objects.push_back(object);

	object = new Obelisk(textureManager, 660.0f, 280.0f, 50.0f);
	objects.push_back(object);

	object = new Obelisk(textureManager, 840.0f, 280.0f, 50.0f);
	objects.push_back(object);

	object = new Obelisk(textureManager, 1500.0f, 280.0f, 60.0f);
	objects.push_back(object);

	object = new Chest(textureManager, 120.0f, 234.0f);
	objects.push_back(object);

	// Create player
	player = new Player(textureManager, 15.0f, 234.0f);
	objects.push_back(player);
	view_follow = player;

	// Test dialogue
	dialogue = new Dialogue(std::vector<std::string>({ "Welcome to our game demo! Press X to continue.", "This is another line of text!" }));
}

int Level01_State::countZombies()
{
	return count_if(objects.begin(), objects.end(), [&](Object* const &elem)
	{
		return dynamic_cast<Zombie*>(elem) != nullptr;
	});
}

void Level01_State::drawBackground(sf::RenderWindow &window)
{
	// TODO: hardcoded assumption of view width to background width ratio
	// TODO: hardcoded background width
	for (size_t i = 0; i < 4; i++)
	{
		bg01.setPosition(sf::Vector2f(getViewX() + (float)fmod((-getViewX() / 2), 192) + (i * 192), getViewY() + 98));
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

	dialogue->draw(window, getView());
}

void Level01_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
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

	if (countZombies() < 10) objects.push_back(new Zombie(textureManager, (float)((double)rand() / (RAND_MAX)) * 1248, 250));

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

		if (inputHandler.checkInput(InputHandler::Input::Up, event)) player->jump(moveH, soundManager, settings); // Jumping
		if (inputHandler.checkInput(InputHandler::Input::Action, event)) player->throwWeapon(objects, player->getDir(), soundManager, settings); // Throw Weapon
	}

	player->move(moveH);
	player->setCrouching(crouching);

	// Update dialogue
	dialogue->update(inputHandler);

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}
