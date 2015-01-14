#include "level01_state.h"

Level01_State::Level01_State(StateManager &sM, TextureManager &textureManager)
	: State(sM)
{
	start(textureManager);
}

Level01_State::~Level01_State()
{
	end();
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

	for (auto object : objects)
	{
		object->draw(window);
	}
}

void Level01_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{
	sf::Time deltaTime = restartClock();

	/* Restart Level if Player is Outside of the Room */
	if (player->getY() > WINDOW_HEIGHT + 74.0f) reset(textureManager);

	/* Input */
	sf::Event event;
	bool exit = false;

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

		if (inputHandler.checkInput("exit", event))
		{
			getStateManager().setState("menu");
			exit = true;
		}
		else
		{
			if (inputHandler.checkInput("jump", event)) player->jump(moveH, soundManager); // Jumping
			if (inputHandler.checkInput("throw", event)) player->throwWeapon(objects, player->getDir(), textureManager, soundManager); // Throw Weapon

			if (inputHandler.checkInput("debug0", event)) player->changeTexture(textureManager, "arthur0");
		}
	}

	if (!exit)
	{
		player->move(moveH);
		player->setCrouching(crouching);


		/* Update Stuff */

		// Update objects and player
		// TODO: Resolve by overriding virtually
		player->update(deltaTime, window, getView(), textureManager, soundManager, objects);

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

			switch ((*iter)->getType())
			{
			default:
				(*iter)->update(deltaTime, objects);
				break;

				// TODO: Resolve by overriding virtually
			case Object::Type::Projectile:
				((Projectile*)*iter)->update(deltaTime, getViewX(), objects);
				break;
			}
			iter++;
		}
	}
	std::cout << "Time: " << (deltaTime.asMicroseconds() / 1000.0) << std::endl;
}
