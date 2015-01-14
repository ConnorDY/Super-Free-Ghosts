#include "menu_state.h"
#include "level01_state.h"

Menu_State::Menu_State(StateManager &sM, TextureManager &textureManager)
	: State(sM)
{
	// Set up logo sprite
	sprLogo.setTexture(textureManager.getRef("logo"));
	sprLogo.setScale(sf::Vector2f(2.0f, 2.0f));
	sprLogo.setOrigin(sf::Vector2f(102.f, 0.0f));

	// Load Font
	if (!fnt.loadFromFile("res/pixelmix.ttf")) std::cout << "Failed to load menu font!" << std::endl;

	// Menu Options
	menuOptions.push_back("Start");
	menuOptions.push_back("Exit");
}

Menu_State::~Menu_State()
{

}


/* Actions */
void Menu_State::draw(sf::RenderWindow &window)
{
	window.setView(window.getDefaultView());
	sprLogo.setPosition(sf::Vector2f(402.0f, 80.0f));
	window.draw(sprLogo);

	float xx = 400.0f, yy = 360.0f;

	for (unsigned int i = 0; i < menuOptions.size(); i++)
	{
		// Create text
		sf::Text text;
		text.setFont(fnt);
		text.setCharacterSize(18);
		text.setString(menuOptions.at(i));

		// Text colour
		if (currentOption == i) text.setColor(sf::Color::Yellow);
		else text.setColor(sf::Color::White);

		// Center text
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(round(textRect.left + textRect.width / 2.0f), round(textRect.top + textRect.height / 2.0f));

		// Set position
		text.setPosition(sf::Vector2f(round(xx), round(yy)));
		yy += 32;

		// Draw text
		window.draw(text);
	}
}

void Menu_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler)
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

		if (inputHandler.checkInput(InputHandler::Input::Up, event))
		{
			if (currentOption == 0) currentOption = menuOptions.size() - 1;
			else currentOption--;
		}

		if (inputHandler.checkInput(InputHandler::Input::Down, event))
		{
			if (currentOption == menuOptions.size() - 1) currentOption = 0;
			else currentOption++;
		}

		if (inputHandler.checkInput(InputHandler::Input::Action, event) || inputHandler.checkInput(InputHandler::Input::Start, event))
		{
			switch (currentOption)
			{
				default:
					break;

				// Load Level 01
				case 0:
					getStateManager().setState(std::unique_ptr<State>(new Level01_State(getStateManager(), textureManager)));
					break;

				// Exit
				case 1:
					std::exit(0);
					break;
			}
		}
	}
}
