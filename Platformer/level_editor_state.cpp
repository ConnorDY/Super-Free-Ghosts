#include "level_editor_state.h"
#include "menu_state.h"

#define GRID_SCALE 8

Level_Editor_State::Level_Editor_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	cursor = sf::Vector2i(-1, -1);
	point = cursor;
	clickedL = false;

	shapeCursor.setSize(sf::Vector2f(2, 2));
	shapeCursor.setOrigin(sf::Vector2f(1, 1));
	shapeCursor.setFillColor(sf::Color(255, 0, 0, 180));

	shapeGrid.setSize(sf::Vector2f(GRID_SCALE - 2, GRID_SCALE - 2));
	shapeGrid.setOrigin(sf::Vector2f(-1, -1));
	shapeGrid.setFillColor(sf::Color(255, 255, 255, 10));
	shapeGrid.setOutlineThickness(1);

	// Create player
	player = new Player(textureManager, -50.0f, 500.0f);
	objects.push_back(player);
}

Level_Editor_State::~Level_Editor_State()
{
}

void Level_Editor_State::start(TextureManager&, settings_t const&)
{

}

static double gridYToWindowY(double gridY) { return gridY * GRID_SCALE; }
static double gridXToWindowX(double gridX) { return gridX * GRID_SCALE; }

void Level_Editor_State::drawForeground(sf::RenderWindow &window)
{
	auto gridXOffset = gridXToWindowX(floor(getViewX() / GRID_SCALE));
	// Grid
	for (int j = 0; j <= ceil(VIEW_HEIGHT / GRID_SCALE); j++)
	{
		for (int i = 0; i <= ceil(VIEW_WIDTH / GRID_SCALE); i++)
		{
			sf::Color col;

			if (i + gridXOffset / GRID_SCALE == point.x && j == point.y) col = sf::Color(255, 0, 0, 150);
			else if (j == gridCursor.y) col = sf::Color(255, 255, 255, 150);
			else if (j == point.y) col = sf::Color(255, 216, 0, 150);
			else col = sf::Color(255, 255, 255, 50);

			shapeGrid.setOutlineColor(col);
			shapeGrid.setPosition(sf::Vector2f(gridXToWindowX(i) + gridXOffset, gridYToWindowY(j)));
			window.draw(shapeGrid);
		}
	}

	// Heightmap
	Room::drawForeground(window);

	// Highlighted Grid Part
	shapeGrid.setOutlineColor(sf::Color(0, 255, 33, 150));
	shapeGrid.setPosition(sf::Vector2f(gridXToWindowX(gridCursor.x), gridYToWindowY(gridCursor.y)));
	window.draw(shapeGrid);
}

void Level_Editor_State::updateView(sf::RenderWindow &window)
{
	sf::Vector2f viewRect(getView().getCenter());
	double shift = deltaTime.asMicroseconds() / 10000.;

	if (cursor.x - getViewX() < 20) viewRect.x -= shift;
	else if (cursor.x - getViewX() > VIEW_WIDTH - 20) viewRect.x += shift;
	viewRect.x = std::max<double>(viewRect.x, VIEW_WIDTH / 2);

	getView().setCenter(viewRect);
	window.setView(getView());
}

class LineEndpoints {
		sf::Vector2i leftpoint, rightpoint;
	public:
		LineEndpoints(sf::Vector2i leftpoint, sf::Vector2i rightpoint)
			: leftpoint(leftpoint), rightpoint(rightpoint)
		{}
		double getLeft() { return gridXToWindowX(leftpoint.x); }
		double getWidth() { return (rightpoint.x - leftpoint.x + 1) * GRID_SCALE; }
		double getLeftY() { return VIEW_HEIGHT - gridYToWindowY(leftpoint.y); }
		double getRightY() { return VIEW_HEIGHT - gridYToWindowY(rightpoint.y); }
};

static LineEndpoints getLineFromPoints(sf::Vector2i const &point1, sf::Vector2i const &point2) {
	sf::Vector2i leftpoint, rightpoint;
	if (point1.x > point2.x)
		leftpoint = point2, rightpoint = point1;
	else
		leftpoint = point1, rightpoint = point2;

	if (leftpoint.x < 0) leftpoint.x = 0;
	if (rightpoint.x < 0) rightpoint.x = 0;
	return LineEndpoints( leftpoint, rightpoint);
}

void Level_Editor_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
{
	// Get Input
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

			case sf::Event::MouseButtonPressed:
				point = gridCursor;
				if (event.mouseButton.button == sf::Mouse::Left) clickedL = true;
				if (event.mouseButton.button == sf::Mouse::Middle)
				{
					player->setX(cursor.x);
					player->setY(cursor.y);
				}
				break;

			case sf::Event::MouseButtonReleased:
				auto line = getLineFromPoints(point, gridCursor);
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					clickedL = false;
					createSlope(line.getLeft(), line.getWidth(), line.getLeftY(), line.getRightY());
				}
				else if (event.mouseButton.button ==  sf::Mouse::Right)
				{
					fillHeightMap(line.getLeft(), line.getWidth(), 0);
				}

				point = sf::Vector2i(-1, -1);
				break;
		}

		if (inputHandler.checkInput(InputHandler::Input::Exit, event))
		{
			getStateManager().setState(std::unique_ptr<State>(new Menu_State(getStateManager(), textureManager)));
			return;
		}
		
		if (inputHandler.checkInput(InputHandler::Input::Up, event)) player->jump(moveH, soundManager, settings); // Jumping
		if (inputHandler.checkInput(InputHandler::Input::Action, event)) player->throwWeapon(objects, player->getDir(), soundManager, settings); // Throw Weapon
	}

	player->move(moveH);
	player->setCrouching(crouching);

	// Get mouse position
	sf::Vector2i m = sf::Mouse::getPosition(window);
	cursor = sf::Vector2i((int)floor(m.x / (float)settings.window_scale) + getViewX(), (int)floor(m.y / (float)settings.window_scale) + getViewY());
	gridCursor = sf::Vector2i((int)floor(cursor.x / GRID_SCALE), (int)floor(cursor.y / GRID_SCALE));

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}
