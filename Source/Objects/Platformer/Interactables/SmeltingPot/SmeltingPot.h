#pragma once

#include "Objects/Platformer/Interactables/InteractObject.h"

namespace cocos2d
{
	class Sprite;
}

class Item;
class SmartAnimationSequenceNode;
class WorldSound;

class SmeltingPot : public InteractObject
{
public:
	static SmeltingPot* create(cocos2d::ValueMap& properties);
	
	void activate();
	void deactivate();

	static const std::string MapKey;

protected:
	SmeltingPot(cocos2d::ValueMap& properties);
	virtual ~SmeltingPot();

	void onEnter() override;
	void initializePositions() override;
	void onInteract(PlatformerEntity* interactingEntity) override;

private:
	typedef InteractObject super;

	cocos2d::Sprite* smeltingpot = nullptr;
	cocos2d::Node* floatContainer = nullptr;
	cocos2d::Sprite* shine = nullptr;
	cocos2d::Sprite* hammer = nullptr;
};
