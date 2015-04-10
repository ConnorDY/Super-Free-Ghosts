#include "dialogue.h"
#include "globals.h"

Dialogue::Dialogue(const std::vector<std::string> lines)
{
	textLines = lines;
	ipShow = true;
	done = false;
	index = 0;
	line = 0;
	
	if (!fnt.loadFromFile("res/Tewi-normal-11.bdf")) std::cout << "Failed to load dialogue font!" << std::endl;
	const_cast<sf::Texture&>(fnt.getTexture(11)).setSmooth(false);

	txt.setFont(fnt);
	txt.setCharacterSize(11);

	rect.setSize(sf::Vector2f(VIEW_WIDTH - 8, 20));
	rect.setFillColor(sf::Color(0, 0, 0, 100));
	rect.setOutlineColor(sf::Color(0, 0, 0, 150));
	rect.setOutlineThickness(1);

	ip.setSize(sf::Vector2f(2, 11));
	ip.setFillColor(sf::Color(255, 255, 255, 175));
}

Dialogue::~Dialogue()
{

}

void Dialogue::draw(sf::RenderWindow &window, const sf::View &view)
{
	if (done) return;

	sf::Vector2f viewCoords = view.getCenter();
	viewCoords.x -= VIEW_WIDTH / 2;
	viewCoords.y -= VIEW_HEIGHT / 2;

	// Background
	rect.setPosition(viewCoords.x + 4, viewCoords.y + VIEW_HEIGHT - 24);
	window.draw(rect);
	
	// Text
	txt.setString(textLines.at(line).substr(0, index + 1));
	txt.setPosition(viewCoords.x + 8, viewCoords.y + VIEW_HEIGHT - 22);
	window.draw(txt);

	// Insertion Point
	if (!ipShow) return;
	sf::FloatRect rect = txt.getLocalBounds();
	ip.setPosition(viewCoords.x + rect.width + 12, viewCoords.y + VIEW_HEIGHT - 20);
	window.draw(ip);
}

void Dialogue::update(InputHandler &inputHandler)
{
	sf::Time deltaTime = timer.getElapsedTime();

	bool held = inputHandler.checkInput(InputHandler::Input::Dialogue);
	double speed = 50.;
	if (held) speed = 10;

	if (deltaTime.asMilliseconds() >= speed)
	{
		index++;
		timer.restart();
	}

	if (index >= textLines.at(line).length()) index = textLines.at(line).length() - 1;
	
	if (index == textLines.at(line).length() - 1 && held)
	{
		if (line < textLines.size() - 1)
		{
			index = 0;
			line++;
		}
		else done = true;
	}

	if (ipTimer.getElapsedTime().asMilliseconds() > 200)
	{
		ipShow = !ipShow;
		ipTimer.restart();
	}
}