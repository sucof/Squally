#include "EntityPacingBehavior.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/base/CCValue.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Collision/EntityCollisionBehaviorBase.h"
#include "Scenes/Platformer/Level/Physics/PlatformerPhysicsTypes.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/UIResources.h"

using namespace cocos2d;

const std::string EntityPacingBehavior::MapKey = "pacing";
const std::string EntityPacingBehavior::PropertyTravelDistance = "travel-distance";
const float EntityPacingBehavior::DefaultTravelDistanceMax = 512.0f;
const float EntityPacingBehavior::DefaultTravelDistanceMin = 96.0f;
const float EntityPacingBehavior::MinimumPaceDistance = 56.0f;

EntityPacingBehavior* EntityPacingBehavior::create(GameObject* owner)
{
	EntityPacingBehavior* instance = new EntityPacingBehavior(owner);

	instance->autorelease();

	return instance;
}

EntityPacingBehavior::EntityPacingBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);
	this->maxTravelDistance = EntityPacingBehavior::DefaultTravelDistanceMax;

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	else
	{
		this->maxTravelDistance = this->entity->getPropertyOrDefault(EntityPacingBehavior::PropertyTravelDistance, Value(EntityPacingBehavior::DefaultTravelDistanceMax)).asFloat();
	}
}

EntityPacingBehavior::~EntityPacingBehavior()
{
}

void EntityPacingBehavior::initializePositions()
{
}

void EntityPacingBehavior::onLoad()
{
	this->spawnPosition = GameUtils::getWorldCoords(this->entity, false);

	this->entity->watchForComponent<EntityCollisionBehaviorBase>([=](EntityCollisionBehaviorBase* collisionBehavior)
	{
		collisionBehavior->leftCollision->whenCollidesWith({ (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::SolidNpcOnly }, [=](CollisionData collisionData)
		{
			this->cancelPacing();
			
			return CollisionResult::DoNothing;
		});

		collisionBehavior->rightCollision->whenCollidesWith({ (int)PlatformerCollisionType::Solid, (int)PlatformerCollisionType::SolidNpcOnly }, [=](CollisionData collisionData)
		{	
			this->cancelPacing();

			return CollisionResult::DoNothing;
		});
	});

	this->runAction(Sequence::create(
		DelayTime::create(RandomHelper::random_real(0.5f, 4.0f)),
		CallFunc::create([=]()
		{
			this->assignDestination();
		}),
		nullptr
	));
}

void EntityPacingBehavior::onDisable()
{
	super::onDisable();
}

void EntityPacingBehavior::assignDestination()
{
	float destinationX = 0.0f;
	float currentX = GameUtils::getWorldCoords(this->entity).x;

	const int LoopsMax = 256;
	int loopSafety = 0;

	do
	{
		destinationX = this->spawnPosition.x + RandomHelper::random_real(-this->maxTravelDistance, this->maxTravelDistance);

		if (loopSafety++ > LoopsMax)
		{
			return;
		}
	}
	while (std::abs(currentX - destinationX) < EntityPacingBehavior::MinimumPaceDistance);


	// Leverage the cinematic movement code for enemy pacing, should work fine
	if (!this->entity->getRuntimeStateOrDefault(StateKeys::PatrolHijacked, Value(false)).asBool())
	{
		Vec3 source = GameUtils::getWorldCoords3D(this->entity);

		this->entity->setState(StateKeys::PatrolSourceX, Value(source.x));
		this->entity->setState(StateKeys::PatrolDestinationX, Value(destinationX));
	}

	this->runAction(Sequence::create(
		DelayTime::create(RandomHelper::random_real(2.0f, 7.5f)),
		CallFunc::create([=]()
		{
			this->assignDestination();
		}),
		nullptr
	));
}

void EntityPacingBehavior::cancelPacing()
{
	this->entity->clearState(StateKeys::PatrolSourceX);
	this->entity->clearState(StateKeys::PatrolDestinationX);
}
