#pragma once

#include "Objects/Platformer/Breakables/CagedAnimals/CagedAnimal.h"

namespace cocos2d
{
	class Sprite;
};

class CagedGoat : public CagedAnimal
{
public:
	static CagedGoat* create(cocos2d::ValueMap& properties);

	static const std::string MapKey;

protected:
	CagedGoat(cocos2d::ValueMap& properties);
	virtual ~CagedGoat();

	void initializePositions() override;

	LocalizedString* getAnimalName() override;
	std::string getSpriteResource() override;

private:
	typedef CagedAnimal super;

	cocos2d::Sprite* animalSprite = nullptr;
};
