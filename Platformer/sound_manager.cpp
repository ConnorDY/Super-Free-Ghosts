#include "sound_manager.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
	for (auto const &it : sounds)
	{
		delete it.second;
	}

	for (auto const &it : buffers)
	{
		delete it.second;
	}
}


/* Actions */
void SoundManager::loadSound(const std::string &name, const std::string &filename)
{
	/* Load the sound buffer */
	sf::SoundBuffer *buf = new sf::SoundBuffer();
	buf->loadFromFile(filename);

	if (_DEBUG) std::cout << "Loaded sound \"" << name << "\" from " << filename << std::endl;

	/* Add it to the list of buffers */
	this->buffers[name] = buf;

	/* Create sound and add it to the list of sounds */
	sf::Sound *snd = new sf::Sound();
	snd->setBuffer(*this->buffers.at(name));
	this->sounds[name] = snd;
}

void SoundManager::playSound(const std::string &sound)
{
	if (sounds.find(sound) != sounds.end()) this->sounds.at(sound)->play();
}
