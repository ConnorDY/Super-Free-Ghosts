#include "sound_manager.h"
#include "globals.h"

SoundManager::SoundManager(settings_t const &settings)
	: settings(settings)
{
}

SoundManager::~SoundManager()
{
}


/* Actions */
void SoundManager::loadSound(const std::string &name, const std::string &filename)
{
	/* Load the sound buffer */
	auto buf = std::make_unique<sf::SoundBuffer>();
	buf->loadFromFile(filename);

#ifdef _DEBUG
	std::cout << "Loaded sound \"" << name << "\" from " << filename << std::endl;
#endif

	/* Create sound and add it to the list of sounds */
	this->sounds[name] = std::make_unique<sf::Sound>(*buf);

	/* Add the buffer to the list of buffers */
	this->buffers[name] = std::move(buf);
}

void SoundManager::playSound(const std::string &sound) const
{
	if (!settings.sound_on) return;
	if (sounds.find(sound) != sounds.end()) this->sounds.at(sound)->play();
}
