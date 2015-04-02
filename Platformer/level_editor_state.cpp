#include "level_editor_state.h"
#include "menu_state.h"

Level_Editor_State::Level_Editor_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	point = sf::Vector2i(0, 0);
	clicked = false;
}

Level_Editor_State::~Level_Editor_State()
{
}

void Level_Editor_State::start(TextureManager &textureManager, const settings_t &settings)
{

}

void Level_Editor_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
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

		if (inputHandler.checkInput(InputHandler::Input::Exit, event))
		{
			getStateManager().setState(std::unique_ptr<State>(new Menu_State(getStateManager(), textureManager, settings)));
			return;
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!clicked)
			{
				point = sf::Mouse::getPosition(window);
				clicked = true;
			}
		}
		else if (clicked)
		{
			fillHeightMap(point.x / settings.window_scale, (sf::Mouse::getPosition(window).x - point.x) / settings.window_scale, abs(VIEW_HEIGHT - (point.y / settings.window_scale)));
			clicked = false;
		}
	}

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}