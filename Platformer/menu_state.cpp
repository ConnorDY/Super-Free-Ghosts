#include "menu_state.h"

Menu_State::Menu_State(StateManager &sM, TextureManager &textureManager)
	: State(sM)
{
	// Set up logo sprite
	sprLogo.setTexture(textureManager.getRef("logo"));
	sprLogo.setOrigin(sf::Vector2f(102.f, 0.0f));

	// Load Font
	if (!fnt.loadFromFile("res/Tewi-normal-11.bdf")) std::cout << "Failed to load menu font!" << std::endl;
	const_cast<sf::Texture&>(fnt.getTexture(11)).setSmooth(false);

	// Menu Options
	menuOptions.push_back("Start");
	menuOptions.push_back("Level Editor");
	menuOptions.push_back("Options");
	menuOptions.push_back("Exit");
}

Menu_State::~Menu_State()
{

}


/* Actions */
void Menu_State::draw(sf::RenderWindow &window)
{
	sf::View temp = sf::View(sf::Vector2f((float)VIEW_WIDTH / 2.0f, (float)VIEW_HEIGHT / 2.0f), sf::Vector2f((float)VIEW_WIDTH, (float)VIEW_HEIGHT));
	window.setView(temp);

	sprLogo.setPosition(sf::Vector2f(201.0f, 40.0f));
	window.draw(sprLogo);

	float xx = 200.0f, yy = 180.0f;

	for (unsigned int i = 0; i < menuOptions.size(); i++)
	{
		// Create text
		sf::Text text;
		text.setFont(fnt);
		text.setCharacterSize(11);
		text.setString(menuOptions.at(i));

		// Text colour
		if (currentOption == i) text.setColor(sf::Color::Yellow);
		else text.setColor(sf::Color::White);

		// Center text
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(round(textRect.left + textRect.width / 2.0f), round(textRect.top + textRect.height / 2.0f));

		// Set position
		text.setPosition(sf::Vector2f(round(xx), round(yy)));
		yy += 16;

		// Draw text
		window.draw(text);
	}
}

void Menu_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
{
	restartClock();

	// Get Input
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

		// Escape
		if (inputHandler.checkInput(InputHandler::Input::Exit, event))
		{
			window.close();
			return;
		}

		// Up
		if (inputHandler.checkInput(InputHandler::Input::Up, event))
		{
			if (currentOption == 0) currentOption = menuOptions.size() - 1;
			else currentOption--;
		}

		// Down
		if (inputHandler.checkInput(InputHandler::Input::Down, event))
		{
			if (currentOption == menuOptions.size() - 1) currentOption = 0;
			else currentOption++;
		}

		// Enter
		if (inputHandler.checkInput(InputHandler::Input::Action, event) || inputHandler.checkInput(InputHandler::Input::Start, event))
		{
			switch (currentOption)
			{
				default:
					break;

				// Load Level 01
				case 0:
					getStateManager().setState(std::unique_ptr<State>(new Level01_State(getStateManager(), soundManager, textureManager, settings)));
					break;

				// Options Menu
				case 1:
					getStateManager().setState(std::unique_ptr<State>(new Level_Editor_State(getStateManager(), soundManager, textureManager, settings)));
					break;

				// Options Menu
				case 2:
					getStateManager().setState(std::unique_ptr<State>(new Options_State(getStateManager(), settings)));
					break;

				// Exit
				case 3:
					std::exit(0);
					break;
			}
		}
	}
}
