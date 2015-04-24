#ifndef COMBINED_ANIMATIONS_H
#define COMBINED_ANIMATIONS_H
#include <memory>
#include <vector>
#include "modal_animation.h"

namespace CombinedAnimations
{
	std::unique_ptr<ModalAnimation> keepUntilBothFinish(std::unique_ptr<ModalAnimation> one, std::unique_ptr<ModalAnimation> two);
	std::unique_ptr<ModalAnimation> inSequence(std::initializer_list<std::unique_ptr<ModalAnimation>> animations);
	std::unique_ptr<ModalAnimation> inSequence(std::unique_ptr<ModalAnimation> one, std::unique_ptr<ModalAnimation> two);
}

#endif
