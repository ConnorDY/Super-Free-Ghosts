#include "menu_state.h"
#include "globals.h"
#include "demo_state.h"
#include "level01_state.h"
#include "level_editor_state.h"
#include "options_state.h"

// TODO move this
#ifdef STATIC_RESOURCES
#include "static_resource.h"
#endif

Menu_State::Menu_State(StateManager &sM, TextureManager const &textureManager, settings_t &settings)
	: State(sM), textureManager(textureManager), settings(settings)
{
	// Set up logo sprite
	sprLogo.setTexture(textureManager.getRef("logo"));
	sprLogo.setOrigin(sf::Vector2f(102.f, 0.0f));

	// Load Font
#ifdef STATIC_RESOURCES
	if (!fnt.loadFromMemory(ARRAY_WITH_LENGTH(res_Tewi_normal_11_bdf))) std::cout << "Failed to load menu font!" << std::endl;
#else
	if (!fnt.loadFromFile("res/Tewi-normal-11.bdf")) std::cout << "Failed to load menu font!" << std::endl;
#endif
	const_cast<sf::Texture&>(fnt.getTexture(11)).setSmooth(false);

	// Menu Options
	menuOptions.push_back("Start");
#ifndef DEMO_MODE
	menuOptions.push_back("Level Editor");
#endif
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

	//sprLogo.setPosition(sf::Vector2f(201.0f, 40.0f));
	//window.draw(sprLogo);

	float xx = 200.0f, yy = 130.0f;

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

void Menu_State::update(sf::RenderWindow &window, SoundManager &soundManager, InputHandler &inputHandler)
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
					std::exit(0);
					return;

#ifdef DEMO_MODE
				// Load Demo State
				case 0:
					getStateManager().setState(std::make_unique<Demo_State>(getStateManager(), soundManager, textureManager, settings));
					return;

				// Options Menu
				case 1:
					getStateManager().setState(std::make_unique<Options_State>(getStateManager(), textureManager, settings));
					return;
#else
				// Load Level01 State
				case 0:
					getStateManager().setState(std::make_unique<Level01_State>(getStateManager(), soundManager, textureManager, settings));
					return;

				// Level editor
				case 1:
					getStateManager().setState(std::make_unique<Level_Editor_State>(getStateManager(), soundManager, textureManager, settings));
					return;

				// Options Menu
				case 2:
					getStateManager().setState(std::make_unique<Options_State>(getStateManager(), textureManager, settings));
					return;
#endif
			}
		}
	}
}
