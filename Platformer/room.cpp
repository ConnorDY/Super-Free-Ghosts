#include "room.h"
#include <algorithm>

#define M_PI 3.14159265358979323846

Room::Room(StateManager &stm, SoundManager &som, TextureManager &tm, const settings_t &settings)
	: State(stm), soundManager(som),
	  width(2048), height(VIEW_HEIGHT)
{
	setView(sf::View(sf::Vector2f(VIEW_WIDTH / 2.0, VIEW_HEIGHT / 2.0), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT)));
	dirtSprite.setTexture(tm.getRef("tiles"));
}

Room::~Room()
{
	end();
}

bool depthSort(Object* i, Object* j)
{
	return (i->getDepth() > j->getDepth());
}

void Room::ensureHeightmapWidth(size_t width)
{
	size_t oldsize = heightmap.size();
	if (oldsize < width)
		heightmap.insert(heightmap.end(), width - oldsize, 0);
}

void Room::fillHeightMap(size_t xleft, size_t width, int height)
{
	ensureHeightmapWidth(xleft + width);

	size_t xright = xleft + width;
	for (size_t i = xleft; i < xright; i++)
		heightmap[i] = height;
}

void Room::createSlope(size_t xleft, size_t width, int heightS, int heightE)
{
	ensureHeightmapWidth(xleft + width);
	double sin_phase = 0, height = abs(heightE - heightS), adjust = 0;

	if (heightS > heightE)
	{
		sin_phase = M_PI / 2.;
		adjust = height;
	}

	for (size_t i = 0; i < width; i++)
	{
		heightmap[xleft + i] = heightS + (int)(height * sin(sin_phase + ((double)i / (double)width) * (M_PI / 2.))) - (int)adjust;
	}
}

void Room::drawHeightMap(sf::RenderWindow &window)
{
	auto &view = window.getView();
	auto &center = view.getCenter(), &size = view.getSize();
	int viewLeft = (int)floor(center.x - size.x / 2);
	int viewRight = (int)ceil(center.x + size.x / 2);
	int heightmapStart = std::max<int>(0, viewLeft);
	int heightmapEnd = std::min<int>(heightmap.size(), viewRight);
	int spriteWidth = dirtSprite.getTexture()->getSize().x / 2;
	int spriteColumn = heightmapStart % spriteWidth;
	for (int i = heightmapStart; i < heightmapEnd; i++)
	{
		int height = heightmap[i];
		if (height != 0)
		{
			int y = this->height - height;
			// Top of ground with grass
			dirtSprite.setTextureRect(sf::IntRect(spriteColumn, 0, 1, 38));
			dirtSprite.setOrigin(sf::Vector2f(0.0f, 13.0f));
			dirtSprite.setPosition((float)i, (float)y);
			window.draw(dirtSprite);

			// Ground underneath
			dirtSprite.setTextureRect(sf::IntRect(spriteColumn, 38, 1, 16));
			dirtSprite.setOrigin(sf::Vector2f(0.0f, 0.0f));
			for (float j = 19; j < height; j += 16)
			{
				dirtSprite.setPosition((float)i, (float)(y + j));
				window.draw(dirtSprite);
			}
		}

		spriteColumn++;
		if (spriteColumn == spriteWidth)
			spriteColumn = 0;
	}
}

void Room::drawHeightMapBack(sf::RenderWindow &window)
{
	auto &view = window.getView();
	auto &center = view.getCenter(), &size = view.getSize();
	int viewLeft = (int)floor(center.x - size.x / 2);
	int viewRight = (int)ceil(center.x + size.x / 2);
	int heightmapStart = std::max<int>(0, viewLeft);
	int heightmapEnd = std::min<int>(heightmap.size(), viewRight);
	int spriteWidth = dirtSprite.getTexture()->getSize().x / 2;
	int spriteColumn = heightmapStart % spriteWidth;
	for (int i = heightmapStart; i < heightmapEnd; i++)
	{
		int height = heightmap[i];
		if (height != 0)
		{
			int y = this->height - height;
			// Grass
			dirtSprite.setTextureRect(sf::IntRect(spriteColumn + 16, 0, 1, 38));
			dirtSprite.setOrigin(sf::Vector2f(0.0f, 13.0f));
			dirtSprite.setPosition((float)i, (float)y);
			window.draw(dirtSprite);
		}

		spriteColumn++;
		if (spriteColumn == spriteWidth)
			spriteColumn = 0;
	}
}

bool Room::heightmapIntersects(sf::FloatRect const &rect) const
{
	int left = std::max<int>(0, floor(rect.left));
	int right = std::min<int>(heightmap.size(), 1 + ceil(rect.left + rect.width));
	if (left >= right) return false;

	auto maxHeight = *std::max_element(heightmap.begin() + left, heightmap.begin() + right);
	if (maxHeight == 0) return false; // 0 is a special case (no solid ground)
	// Here we are negating because it's height up from the ground (-y)
	return height - maxHeight <= rect.top + rect.height;
}

bool Room::exceedsHorizontalBounds(sf::FloatRect const &rect) const
{
	return rect.left < 0 || rect.left + rect.width >= width;
}

SoundManager& Room::getSoundManager() const
{
	return soundManager;
}

std::vector<Object*> const Room::getObjects() const
{
	return objects;
}

void Room::end()
{
	view_follow = nullptr;
	for (Object* i : objects) delete i;
	objects.clear();
}

void Room::reset(TextureManager &textureManager, const settings_t &settings)
{
	end();
	start(textureManager, settings);
}

void Room::updateView(sf::RenderWindow &window)
{
	if (view_follow)
	{
		// Update view
		auto rect = view_follow->getRect();
		float vx = rect.left + rect.width/2;
		float vy = rect.top + rect.height/2;

		auto view_size = getView().getSize();
		float vw = view_size.x;
		float vh = view_size.y;

		auto viewLeftMax = vw / 2;
		auto viewTopMax = vh / 2;
		auto viewRightMax = width - viewLeftMax;
		auto viewBottomMax = height - viewTopMax;

		// Don't let the view go past the bottom or right of the room
		if (vx > viewRightMax) vx = viewRightMax;
		if (vy > viewBottomMax) vy = viewBottomMax;

		// Don't let the view go past the top or left of the room
		if (vx < vw / 2.0f) vx = vw / 2.0f;
		if (vy < vh / 2.0f) vy = vh / 2.0f;

		getView().setCenter(vx, vh / 2.0f);
		window.setView(getView());
	}
}

void Room::drawBackground(sf::RenderWindow &window)
{
	drawHeightMapBack(window);
}

void Room::drawSprites(sf::RenderWindow &window)
{
	for (auto object : objects)
		object->draw(window);
}

void Room::drawForeground(sf::RenderWindow &window)
{
	drawHeightMap(window);
}

void Room::draw(sf::RenderWindow &window)
{
	updateView(window);
	std::sort(objects.begin(), objects.end(), depthSort);
	drawBackground(window);
	drawSprites(window);
	drawForeground(window);
}

void Room::update(sf::RenderWindow&, TextureManager&, SoundManager&, InputHandler&, const settings_t &settings)
{
	deltaTime = restartClock();

	// Update objects and player
	auto iter = objects.begin();
	auto end = objects.end();
	while (iter != end)
	{
		if ((*iter)->shouldDelete())
		{
			delete *iter;
			iter = objects.erase(iter);
			end = objects.end();
			continue;
		}

		(*iter)->update(deltaTime, *this, settings);
		iter++;
	}

	// Output time it took to render this frame
	if (DEBUG_MODE) std::cout << "Time: " << (deltaTime.asMicroseconds() / 1000.0) << std::endl;
}
