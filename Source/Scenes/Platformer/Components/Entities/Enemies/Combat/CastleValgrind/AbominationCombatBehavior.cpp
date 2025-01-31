#include "AbominationCombatBehavior.h"

#include "Engine/Inventory/Inventory.h"
#include "Engine/Inventory/Item.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Inventory/EntityInventoryBehavior.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityAttackBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/Radiation/CastRadiation.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Abilities/BasicSlash/BasicSlash.h"

#include "Resources/UIResources.h"

using namespace cocos2d;
	
const std::string AbominationCombatBehavior::MapKey = "abomination-combat";

AbominationCombatBehavior* AbominationCombatBehavior::create(GameObject* owner)
{
	AbominationCombatBehavior* instance = new AbominationCombatBehavior(owner);

	instance->autorelease();

	return instance;
}

AbominationCombatBehavior::AbominationCombatBehavior(GameObject* owner) : super(owner)
{
	this->entity = dynamic_cast<PlatformerEntity*>(owner);

	if (this->entity == nullptr)
	{
		this->invalidate();
	}
	
	this->setTimelineSpeedBase(1.25f);
}

AbominationCombatBehavior::~AbominationCombatBehavior()
{
}

void AbominationCombatBehavior::initializePositions()
{
}

void AbominationCombatBehavior::onLoad()
{
	super::onLoad();
	
	this->entity->watchForComponent<EntityAttackBehavior>([=](EntityAttackBehavior* attackBehavior)
	{
		attackBehavior->registerAttack(CastRadiation::create(0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Guaranteed));
		attackBehavior->registerAttack(BasicSlash::create(6, 8, 0.7f, EntityAttackBehavior::DefaultRecoverSpeed, PlatformerAttack::Priority::Common));
	});
	
	this->entity->watchForComponent<EntityInventoryBehavior>([=](EntityInventoryBehavior* entityInventoryBehavior)
	{
		Inventory* inventory = entityInventoryBehavior->getInventory();

		if (inventory != nullptr)
		{
		}
	});
}
