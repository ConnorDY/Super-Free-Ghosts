#include "options_state.h"
#include "globals.h"
#include "menu_state.h"

// TODO move this
#ifdef STATIC_RESOURCES
#include "static_resource.h"
#endif

Options_State::Options_State(StateManager &sM, TextureManager const &tm, settings_t &settings)
	: State(sM), textureManager(tm), settings(settings)
{
	// Load Font
#ifdef STATIC_RESOURCES
	if (!fnt.loadFromMemory(ARRAY_WITH_LENGTH(res_Tewi_normal_11_bdf))) std::cout << "Failed to load menu font!" << std::endl;
#else
	if (!fnt.loadFromFile("res/Tewi-normal-11.bdf")) std::cout << "Failed to load menu font!" << std::endl;
#endif
	const_cast<sf::Texture&>(fnt.getTexture(11)).setSmooth(false);

	// Menu Options
	menuOptions.push_back("Window Scale");
	optionChoices.push_back({ "1", "2", "3" });

	menuOptions.push_back("Vsync Enabled");
	optionChoices.push_back({ "Off", "On" });

	menuOptions.push_back("Music");
	optionChoices.push_back({ "Off", "On" });

	menuOptions.push_back("Sound Effects");
	optionChoices.push_back({ "Off", "On" });

	menuOptions.push_back("Back");
	optionChoices.push_back({});

	// Default values
	currentValue = { 1, 1, 1, 1, 0 };

	// Load values
	currentValue[0] = settings.window_scale - 1;
	if (settings.vsync_on) currentValue[1] = 1; else currentValue[1] = 0;
	if (settings.music_on) currentValue[2] = 1; else currentValue[2] = 0;
	if (settings.sound_on) currentValue[3] = 1; else currentValue[3] = 0;
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
	float xx = 200.0f, yy = 75.0f;

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

void Options_State::toggleVsync(sf::RenderWindow &window)
{
	settings.vsync_on = !settings.vsync_on;
	if (currentValue[1] == 0) currentValue[1] = 1; else currentValue[1] = 0;

	window.setVerticalSyncEnabled(settings.vsync_on);
}

void Options_State::toggleMusic()
{
	settings.music_on = !settings.music_on;
	if (currentValue[2] == 0) currentValue[2] = 1; else currentValue[2] = 0;
}

void Options_State::toggleSound()
{
	settings.sound_on = !settings.sound_on;
	if (currentValue[3] == 0) currentValue[3] = 1; else currentValue[3] = 0;
}

void Options_State::update(sf::RenderWindow &window, SoundManager&, InputHandler &inputHandler)
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
			getStateManager().setState(std::make_unique<Menu_State>(getStateManager(), textureManager, settings));
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

				// Back
				case 4:
					getStateManager().setState(std::make_unique<Menu_State>(getStateManager(), textureManager, settings));
					break;
			}
		}

		// Left
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

				// Vsync
				case 1:
					toggleVsync(window);
					break;

				// Music
				case 2:
					toggleMusic();
					break;

				// Sound
				case 3:
					toggleSound();
					break;
			}
		}

		// Right
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

				// Vsync
				case 1:
					toggleVsync(window);
					break;

				// Music
				case 2:
					toggleMusic();
					break;

				// Sound
				case 3:
					toggleSound();
					break;
			}
		}
	}
}
