#include "combined_animations.h"

namespace
{
	class LongestLifetimeAnimation : public ModalAnimation
	{
		private:
			std::unique_ptr<ModalAnimation> one, two;
		public:
			LongestLifetimeAnimation(std::unique_ptr<ModalAnimation> one, std::unique_ptr<ModalAnimation> two)
				: one(std::move(one)), two(std::move(two))
			{}
			~LongestLifetimeAnimation() {}

			virtual bool finished() { return one->finished() && two->finished(); }
			virtual bool shouldDraw(Object const* obj) { return one->shouldDraw(obj) && two->shouldDraw(obj); }

			virtual void draw(sf::RenderWindow &window)
			{
				one->draw(window);
				two->draw(window);
			}
			virtual void update(sf::Time deltaTime, Room &room, settings_t const &settings)
			{
				one->update(deltaTime, room, settings);
				two->update(deltaTime, room, settings);
			}
	};

	class SequentialAnimation : public ModalAnimation
	{
		private:
			std::vector<std::unique_ptr<ModalAnimation>> animations;
		public:
			SequentialAnimation(std::vector<std::unique_ptr<ModalAnimation>> animations)
				: animations(std::move(animations))
			{}
			~SequentialAnimation() {}

			virtual bool finished() { return animations.empty(); }
			virtual bool shouldDraw(Object const* obj)
			{
				if (animations.empty()) return true;
				return animations[0]->shouldDraw(obj);
			}

			virtual void draw(sf::RenderWindow &window)
			{
				if (!animations.empty()) animations[0]->draw(window);
			}
			virtual void update(sf::Time deltaTime, Room &room, settings_t const &settings)
			{
				while (!animations.empty())
				{
					animations[0]->update(deltaTime, room, settings);
					if (animations[0]->finished())
					{
						animations.erase(animations.begin());
					}
					else break;
				}
			}
	};
}

namespace CombinedAnimations
{
	std::unique_ptr<ModalAnimation> keepUntilBothFinish(std::unique_ptr<ModalAnimation> one, std::unique_ptr<ModalAnimation> two)
	{
		return std::make_unique<LongestLifetimeAnimation>(std::move(one), std::move(two));
	}

	std::unique_ptr<ModalAnimation> inSequence(std::vector<std::unique_ptr<ModalAnimation>> animations)
	{
		return std::make_unique<SequentialAnimation>(std::move(animations));
	}

	std::unique_ptr<ModalAnimation> inSequence(std::unique_ptr<ModalAnimation> one, std::unique_ptr<ModalAnimation> two)
	{
		std::vector<std::unique_ptr<ModalAnimation>> ptrs;
		ptrs.push_back(std::move(one));
		ptrs.push_back(std::move(two));
		return std::make_unique<SequentialAnimation>(std::move(ptrs));
	}
}
