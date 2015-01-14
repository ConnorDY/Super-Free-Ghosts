#include "menu_state.h"

Menu_State::Menu_State(StateManager &sM, TextureManager &textureManager)
	: State(sM)
{
	//sM.setState("level01");

	// Set up logo sprite
	sprLogo.setTexture(textureManager.getRef("logo"));
	sprLogo.setScale(sf::Vector2f(2.0f, 2.0f));
	sprLogo.setOrigin(sf::Vector2f(102.f, 0.0f));

	// Load Font
	if (!fnt.loadFromFile("res/pixelmix.ttf")) std::cout << "Failed to load menu font!" << std::endl;

	// Menu Options
	menuOptions.push_back("Start");
}

Menu_State::~Menu_State()
{

}


/* Actions */
void Menu_State::draw(sf::RenderWindow &window)
{
	sprLogo.setPosition(sf::Vector2f(402.0f, 80.0f));
	window.draw(sprLogo);

	float xx = 400.0f, yy = 360.0f;

	for (std::string s : menuOptions)
	{
		// Create text
		sf::Text text;
		text.setFont(fnt);
		text.setCharacterSize(18);
		text.setString(s);

		// Center text
		sf::FloatRect textRect = text.getLocalBounds();
		text.setOrigin(round(textRect.left + textRect.width / 2.0f), round(textRect.top + textRect.height / 2.0f));

		// Set position
		text.setPosition(sf::Vector2f(round(xx), round(yy)));

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

		if (inputHandler.checkInput("throw", event)) getStateManager().setState("level01"); // Load Level 01
	}
}
