#include "options_state.h"
#include "menu_state.h"

Options_State::Options_State(StateManager &sM, TextureManager &textureManager, const settings_t &settings)
	: State(sM)
{
	// Load Font
	if (!fnt.loadFromFile("res/Tewi-normal-11.bdf")) std::cout << "Failed to load menu font!" << std::endl;
	const_cast<sf::Texture&>(fnt.getTexture(11)).setSmooth(false);

	// Menu Options
	menuOptions.push_back("Window Scale");
	optionChoices.push_back({ "1", "2", "3" });

	menuOptions.push_back("Music");
	optionChoices.push_back({ "Off", "On" });

	menuOptions.push_back("Back");
	optionChoices.push_back({});

	// Default values
	currentValue = { 1, 1, 0 };

	// Load values
	currentValue[0] = settings.window_scale - 1;
	if (settings.music_on) currentValue[1] = 1; else currentValue[1] = 0;
}

Options_State::~Options_State()
{

}


/* Actions */
void Options_State::draw(sf::RenderWindow &window)
{
	sf::View temp = sf::View(sf::Vector2f((float)VIEW_WIDTH / 2.0f, (float)VIEW_HEIGHT / 2.0f), sf::Vector2f((float)VIEW_WIDTH, (float)VIEW_HEIGHT));
	window.setView(temp);

	sf::Text text;
	float xx = 200.0f, yy = 50.0f;

	for (unsigned int i = 0; i < menuOptions.size(); i++)
	{
		// Set text string
		text.setString(menuOptions.at(i));

		// Set text font and size
		text.setFont(fnt);
		text.setCharacterSize(11);

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

		// Option Choices
		if (optionChoices.at(i).size() > 0)
		{
			// Set text string
			text.setString(optionChoices.at(i).at(currentValue.at(i)));

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
}

void Options_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
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

				// Back
				case 2:
					getStateManager().setState(std::unique_ptr<State>(new Menu_State(getStateManager(), textureManager, settings)));
					break;
			}
		}

		if (inputHandler.checkInput(InputHandler::Input::Left, event))
		{
			switch (currentOption)
			{
				default:
					break;

				// Decrease window scale
				case 0:
					if (settings.window_scale > 1)
					{
						settings.window_scale--;
						currentValue[0]--;
						window.setSize(sf::Vector2u(VIEW_WIDTH * settings.window_scale, VIEW_HEIGHT * settings.window_scale));
					}
					break;

				// Disable music
				case 1:
					settings.music_on = false;
					currentValue[1] = 0;
					break;
			}
		}

		if (inputHandler.checkInput(InputHandler::Input::Right, event))
		{
			switch (currentOption)
			{
				default:
					break;

				// Increase window scale
				case 0:
					if (settings.window_scale < 3)
					{
						settings.window_scale++;
						currentValue[0]++;
						window.setSize(sf::Vector2u(VIEW_WIDTH * settings.window_scale, VIEW_HEIGHT * settings.window_scale));
					}
					break;

				// Enable music
				case 1:
					settings.music_on = true;
					currentValue[1] = 1;
					break;
			}
		}
	}
}
