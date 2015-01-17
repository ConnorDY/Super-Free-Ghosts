#include "level01_state.h"
#include "menu_state.h"

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
	Object *object = new Block(textureManager, 0.0f, 568.0f, 256.0f, 32.0f);
	objects.push_back(object);

	object = new Block(textureManager, 256.0f, 536.0f, 256.0f, 64.0f);
	objects.push_back(object);

	object = new Block(textureManager, 512.0f, 504.0f, 128.0f, 96.0f);
	objects.push_back(object);

	object = new Block(textureManager, 768.0f, 568.0f, 256.0f, 32.0f);
	objects.push_back(object);

	object = new Block(textureManager, 1152.0f, 568.0f, 64.0f, 32.0f);
	objects.push_back(object);

	object = new Block(textureManager, 1472.0f, 568.0f, 64.0f, 32.0f);
	objects.push_back(object);

	object = new Block(textureManager, 1632.0f, 504.0f, 512.0f, 96.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 540.0f, 64.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 810.0f, 64.0f);
	objects.push_back(object);

	object = new Zombie(textureManager, 1700.0f, 64.0f);
	objects.push_back(object);

	// Create player
	player = new Player(textureManager, 30.0f, 468.0f);
	objects.push_back(player);
	
	// Create the view
	setView(sf::View(sf::Vector2f((float)WINDOW_WIDTH / 2.0f, (float)WINDOW_HEIGHT / 2.0f), sf::Vector2f((float)WINDOW_WIDTH, (float)WINDOW_HEIGHT)));
}

void Level01_State::end()
{
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
	for (auto object : objects)
	{
		object->draw(window);
	}
}

void Level01_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
{
	sf::Time deltaTime = restartClock();

	/* Restart Level if Player is Outside of the Room */
	if (player->getRect().top > WINDOW_HEIGHT + 74.0f) reset(textureManager);

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


	// Update objects and player
	auto iter = objects.begin();
	auto end = objects.end();
	while (iter != end)
	{
		if ((*iter)->shouldDelete())
		{
			if ((*iter)->getType() == Object::Type::Zombie) soundManager.playSound("enemy_die");
			else if ((*iter)->getType() == Object::Type::Projectile && !(*iter)->getOutsideView()) soundManager.playSound("hit");

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

			// TODO: Resolve by overriding virtually
			case Object::Type::Player:
				((Player*)*iter)->update(deltaTime, window, getView(), textureManager, soundManager, objects);
				break;
		}
		iter++;
	}

	std::cout << "Time: " << (deltaTime.asMicroseconds() / 1000.0) << std::endl;
}
