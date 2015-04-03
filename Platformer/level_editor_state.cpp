#include "level_editor_state.h"
#include "menu_state.h"

#define GRID_SCALE 8

Level_Editor_State::Level_Editor_State(StateManager &sM, SoundManager &som, TextureManager &textureManager, const settings_t &settings)
	: Room(sM, som, textureManager, settings)
{
	cursor = sf::Vector2i(0, 0);
	point = cursor;
	clickedL = false;
	clickedR = false;

	shapeCursor.setSize(sf::Vector2f(2, 2));
	shapeCursor.setOrigin(sf::Vector2f(1, 1));
	shapeCursor.setFillColor(sf::Color(255, 0, 0, 180));

	shapeGrid.setSize(sf::Vector2f(GRID_SCALE - 2, GRID_SCALE - 2));
	shapeGrid.setOrigin(sf::Vector2f(-1, -1));
	shapeGrid.setFillColor(sf::Color(255, 255, 255, 10));
	shapeGrid.setOutlineThickness(1);
}

Level_Editor_State::~Level_Editor_State()
{
}

void Level_Editor_State::start(TextureManager &textureManager, const settings_t &settings)
{

}

void Level_Editor_State::drawForeground(sf::RenderWindow &window)
{
	// Grid
	for (unsigned int i = 0; i <= ceil(VIEW_WIDTH / GRID_SCALE); i++)
	{
		for (unsigned int j = 0; j <= ceil(VIEW_HEIGHT / GRID_SCALE); j++)
		{
			shapeGrid.setOutlineColor(sf::Color(255, 255, 255, 50));
			shapeGrid.setPosition(sf::Vector2f((float)i * GRID_SCALE, (float)j * GRID_SCALE));
			window.draw(shapeGrid);
		}
	}

	// Heightmap
	Room::drawForeground(window);

	// Highlighted Grid Part
	shapeGrid.setOutlineColor(sf::Color(0, 216, 216, 100));
	shapeGrid.setPosition(sf::Vector2f((float)gridCursor.x * GRID_SCALE, (float)gridCursor.y * GRID_SCALE));
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

static double gridYToRealY(double gridY) { return VIEW_HEIGHT - gridY * GRID_SCALE; }
static double gridXToRealX(double gridX) { return gridX * GRID_SCALE; }

class LineEndpoints {
		sf::Vector2i leftpoint, rightpoint;
		State const *state;
	public:
		LineEndpoints(State const *state, sf::Vector2i leftpoint, sf::Vector2i rightpoint)
			: leftpoint(leftpoint), rightpoint(rightpoint), state(state)
		{}
		double getLeft() { return gridXToRealX(leftpoint.x); }
		double getWidth() { return (rightpoint.x - leftpoint.x + 1) * GRID_SCALE; }
		double getLeftY() { return gridYToRealY(leftpoint.y); }
		double getRightY() { return gridYToRealY(rightpoint.y); }
};

static LineEndpoints getLineFromPoints(Level_Editor_State const *state, sf::Vector2i const &point1, sf::Vector2i const &point2) {
	sf::Vector2i leftpoint, rightpoint;
	if (point1.x > point2.x)
		leftpoint = point2, rightpoint = point1;
	else
		leftpoint = point1, rightpoint = point2;

	if (leftpoint.x < 0) leftpoint.x = 0;
	if (rightpoint.x < 0) rightpoint.x = 0;
	return LineEndpoints(state, leftpoint, rightpoint);
}

void Level_Editor_State::update(sf::RenderWindow &window, TextureManager &textureManager, SoundManager &soundManager, InputHandler &inputHandler, settings_t &settings)
{
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

		/* Left Clicking */
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			if (!clickedL)
			{
				point = gridCursor;
				clickedL = true;
			}
		}
		else if (clickedL)
		{
			clickedL = false;

			auto line = getLineFromPoints(this, point, gridCursor);
			createSlope(line.getLeft(), line.getWidth(), line.getLeftY(), line.getRightY());
		}

		/* Right Clicking */
		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			if (!clickedR)
			{
				point = gridCursor;
				clickedR = true;
			}
		}
		else if (clickedR)
		{
			auto line = getLineFromPoints(this, point, gridCursor);
			fillHeightMap(line.getLeft(), line.getWidth(), 0);
			clickedR = false;
		}
	}

	// Get mouse position
	sf::Vector2i m = sf::Mouse::getPosition(window);
	cursor = sf::Vector2i((int)floor(m.x / (float)settings.window_scale) + getViewX(), (int)floor(m.y / (float)settings.window_scale));
	gridCursor = sf::Vector2i((int)floor(cursor.x / GRID_SCALE), (int)floor(cursor.y / GRID_SCALE));

	Room::update(window, textureManager, soundManager, inputHandler, settings);
}
