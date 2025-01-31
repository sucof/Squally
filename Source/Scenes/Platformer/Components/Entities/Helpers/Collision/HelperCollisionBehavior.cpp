#include "HelperCollisionBehavior.h"

#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"

using namespace cocos2d;

const std::string HelperCollisionBehavior::MapKey = "helper-collisions";

HelperCollisionBehavior* HelperCollisionBehavior::create(GameObject* owner)
{
	HelperCollisionBehavior* instance = new HelperCollisionBehavior(owner);

	instance->autorelease();

	return instance;
}

HelperCollisionBehavior::HelperCollisionBehavior(GameObject* owner) : super(owner, (int)PlatformerCollisionType::Helper, (int)PlatformerCollisionType::EntityMovement)
{
}

HelperCollisionBehavior::~HelperCollisionBehavior()
{
}

void HelperCollisionBehavior::onEntityCollisionCreated()
{
}

void HelperCollisionBehavior::onDisable()
{
	super::onDisable();
}
