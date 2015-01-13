#include "level01_state.h"

Level01_State::Level01_State(StateManager *sM, TextureManager &textureManager)
{
	setStateManager(sM);
	start(textureManager);
}

Level01_State::~Level01_State()
{
	end();
}


// Actions
void Level01_State::start(TextureManager &textureManager)
{
	// Reserve space in objects vector
	objects.reserve(256);

	// Ready Music
	if (music.openFromFile("res/01.ogg"))
	{
		music.play();
		music.setLoop(true);
		music.setVolume(50.0f);
	}

	// Create Objects
	Object *object = new Block(0.0f, 568.0f, 640.0f, 32.0f);
	objects.push_back(object);

	object = new Block(256.0f, 536.0f, 256.0f, 32.0f);
	objects.push_back(object);

	object = new Block(512.0f, 504.0f, 128.0f, 64.0f);
	objects.push_back(object);

	object = new Block(768.0f, 568.0f, 256.0f, 32.0f);
	objects.push_back(object);

	object = new Block(1152.0f, 568.0f, 64.0f, 32.0f);
	objects.push_back(object);

	object = new Block(1472.0f, 568.0f, 64.0f, 32.0f);
	objects.push_back(object);

	object = new Block(1632.0f, 504.0f, 512.0f, 96.0f);
	objects.push_back(object);

	// Create player
	player = new Player(textureManager, 30.0f, 468.0f);
	
	// Create the view
	setView(sf::View(sf::Vector2f((float)WINDOW_WIDTH / 2.0f, (float)WINDOW_HEIGHT / 2.0f), sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT)));
}

void Level01_State::end()
{
	delete player;
	for (Object* i : objects) delete i;
	objects.clear();
}

void Level01_State::reset(TextureManager &textureManager)
{
	end();
	start(textureManager);
}

void Level01_State::draw(sf::RenderWindow &window)
{
	player->draw(window);
	
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		objects[i]->draw(window);
	}
}

void Level01_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{
	sf::Time deltaTime = restartClock();

	/* Restart Level if Player is Outside of the Room */
	if (player->getY() > WINDOW_HEIGHT + 74.0f) reset(textureManager);

	/* Input */
	sf::Event event;

	int moveH = inputHandler.checkInput("right") - inputHandler.checkInput("left"); // Horizontal Movement
	bool crouching = inputHandler.checkInput("down"); // Crouching

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

		if (inputHandler.checkInput("exit", event)) reset(textureManager); // Reset Level
		if (inputHandler.checkInput("jump", event)) player->jump(moveH, soundManager); // Jumping
		if (inputHandler.checkInput("throw", event)) player->throwWeapon(objects, player->getDir(), textureManager, soundManager); // Throw Weapon

		if (inputHandler.checkInput("debug0", event)) player->changeTexture(textureManager, "arthur0");
	}

	player->move(moveH);
	player->setCrouching(crouching);


	/* Update Stuff */

	// Update objects and player
	player->update(deltaTime, window, getView(), textureManager, soundManager, objects);

	for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (objects[i]->shouldDelete())
		{
			objects.erase(objects.begin() + i);
			i--;
		}
		else switch (objects[i]->getType())
		{
			default:
				objects[i]->update(deltaTime, objects);
				break;

			case Object::Type::Projectile:
				((Projectile*)objects[i])->update(deltaTime, getViewX(), objects);
				break;
		}
	}

	std::cout << "Time: " << (deltaTime.asMicroseconds() / 1000.0) << std::endl;
}
