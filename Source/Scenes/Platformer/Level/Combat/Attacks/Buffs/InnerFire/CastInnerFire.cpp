#include "CastInnerFire.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/CombatUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Scenes/Platformer/Components/Entities/Combat/EntityBuffBehavior.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Buffs/InnerFire/InnerFire.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

CastInnerFire* CastInnerFire::create(float attackDuration, float recoverDuration, Priority priority)
{
	CastInnerFire* instance = new CastInnerFire(attackDuration, recoverDuration, priority);

	instance->autorelease();

	return instance;
}

CastInnerFire::CastInnerFire(float attackDuration, float recoverDuration, Priority priority)
	: super(AttackType::Damage, UIResources::Menus_Icons_SkullLavaEyes, priority, AbilityType::Fire, 0, 0, 4, attackDuration, recoverDuration, TargetingType::Single)
{
	this->castSound = WorldSound::create(SoundResources::Platformer_Spells_Heal5);
	
	this->addChild(this->castSound);
}

CastInnerFire::~CastInnerFire()
{
}

void CastInnerFire::initializePositions()
{
	super::initializePositions();
}

PlatformerAttack* CastInnerFire::cloneInternal()
{
	return CastInnerFire::create(this->getAttackDuration(), this->getRecoverDuration(), this->priority);
}

LocalizedString* CastInnerFire::getString()
{
	return Strings::Menus_Hacking_Abilities_Abilities_InnerFire_InnerFire::create();
}

std::string CastInnerFire::getAttackAnimation()
{
	return "AttackCast";
}

void CastInnerFire::performAttack(PlatformerEntity* owner, std::vector<PlatformerEntity*> targets)
{
	super::performAttack(owner, targets);

	this->castSound->play();
	owner->getAnimations()->clearAnimationPriority();
	owner->getAnimations()->playAnimation("AttackCast");

	for (PlatformerEntity* next : targets)
	{
		next->getComponent<EntityBuffBehavior>([=](EntityBuffBehavior* entityBuffBehavior)
		{
			entityBuffBehavior->applyBuff(InnerFire::create(owner, next));
		});
	}
}

void CastInnerFire::onCleanup()
{
}

bool CastInnerFire::isWorthUsing(PlatformerEntity* caster, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	int uncastableCount = 0;

	for (PlatformerEntity* next : otherTeam)
	{
		if (!next->getRuntimeStateOrDefaultBool(StateKeys::IsAlive, true))
		{
			uncastableCount++;
			continue;
		}
		
		if (CombatUtils::HasDuplicateCastOnLivingTarget(caster, next, [](PlatformerAttack* next) { return dynamic_cast<CastInnerFire*>(next) != nullptr;  }))
		{
			uncastableCount++;
			continue;
		}

		next->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
		{
			entityBuffBehavior->getBuff<InnerFire>([&](InnerFire* debuff)
			{
				uncastableCount++;
			});
		});
	}

	return uncastableCount != int(otherTeam.size());
}

float CastInnerFire::getUseUtility(PlatformerEntity* caster, PlatformerEntity* target, const std::vector<PlatformerEntity*>& sameTeam, const std::vector<PlatformerEntity*>& otherTeam)
{
	float utility = CombatUtils::HasDuplicateCastOnLivingTarget(caster, target, [](PlatformerAttack* next) { return dynamic_cast<CastInnerFire*>(next) != nullptr;  }) ? 0.0f : 1.0f;

	target->getComponent<EntityBuffBehavior>([&](EntityBuffBehavior* entityBuffBehavior)
	{
		entityBuffBehavior->getBuff<InnerFire>([&](InnerFire* debuff)
		{
			utility = 0.0f;
		});
	});
	
	return utility;
}
