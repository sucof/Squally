#include "BrokenBlade.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"

#include "Engine/Animations/SmartAnimationSequenceNode.h"
#include "Engine/Events/ObjectEvents.h"
#include "Engine/Hackables/HackableCode.h"
#include "Engine/Hackables/HackableObject.h"
#include "Engine/Hackables/Menus/HackablePreview.h"
#include "Engine/Localization/ConcatString.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Optimization/LazyNode.h"
#include "Engine/Particles/SmartParticles.h"
#include "Engine/Localization/ConstantString.h"
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/BrokenBlade/BrokenBladeGenericPreview.h"
#include "Scenes/Platformer/Level/Combat/CombatMap.h"
#include "Scenes/Platformer/Level/Combat/TimelineEvent.h"
#include "Scenes/Platformer/Level/Combat/TimelineEventGroup.h"

#include "Resources/FXResources.h"
#include "Resources/ObjectResources.h"
#include "Resources/ParticleResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_BROKEN_BLADE 1

const std::string BrokenBlade::BrokenBladeIdentifier = "broken-blade";
const std::string BrokenBlade::HackIdentifierBrokenBlade = "broken-blade";

const int BrokenBlade::MaxMultiplier = 4;
const float BrokenBlade::Duration = 12.0f;

BrokenBlade* BrokenBlade::create(PlatformerEntity* caster, PlatformerEntity* target)
{
	BrokenBlade* instance = new BrokenBlade(caster, target);

	instance->autorelease();

	return instance;
}

BrokenBlade::BrokenBlade(PlatformerEntity* caster, PlatformerEntity* target)
	: super(caster, target, UIResources::Menus_Icons_SwordBrokenAlt, AbilityType::Physical, BuffData(BrokenBlade::Duration, BrokenBlade::BrokenBladeIdentifier))
{
	this->spellEffect = SmartParticles::create(ParticleResources::Platformer_Combat_Abilities_Speed);

	this->addChild(this->spellEffect);
}

BrokenBlade::~BrokenBlade()
{
}

void BrokenBlade::onEnter()
{
	super::onEnter();

	this->spellEffect->setPositionY(this->owner->getEntityBottomPointRelative().y);
	this->spellEffect->start();

	// No PFX on broken blade for now, just hide it
	this->spellEffect->setVisible(false);

	CombatEvents::TriggerHackableCombatCue();
}

void BrokenBlade::initializePositions()
{
	super::initializePositions();
}

void BrokenBlade::registerHackables()
{
	super::registerHackables();

	if (this->owner == nullptr)
	{
		return;
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_BROKEN_BLADE,
			HackableCode::HackableCodeInfo(
				BrokenBlade::HackIdentifierBrokenBlade,
				Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_BrokenBlade::create(),
				HackableBase::HackBarColor::Gray,
				UIResources::Menus_Icons_SwordBrokenAlt,
				LazyNode<HackablePreview>::create([=](){ return BrokenBladeGenericPreview::create(); }),
				{
					{
						HackableCode::Register::zax, Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_RegisterEax::create(), HackableDataType::Int32,
					},
					{
						HackableCode::Register::zbx, Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_RegisterEbx::create(), HackableDataType::Int32,
					},
				},
				int(HackFlags::None),
				this->getRemainingDuration(),
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_CodeEditor_OriginalCode::create(),
						// x86
						ConcatString::create({
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentMaxDamage::create()),
							ConstantString::create("mov ebx, 3\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentCompare::create()),
							ConstantString::create("cmp eax, ebx\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentConditionalMov::create()),
							ConstantString::create("cmovge eax, ebx\n\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovge::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentGe::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentFinale::create())
								->setStringReplacementVariables({
									Strings::Menus_Hacking_Lexicon_Assembly_RegisterEax::create(),
									Strings::Menus_Hacking_Lexicon_Assembly_RegisterEbx::create(),
								}),
							ConstantString::create("\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentHint::create()
								->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterEax::create()))
						})
						, // x64
						ConcatString::create({
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentMaxDamage::create()),
							ConstantString::create("mov rbx, 3\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentCompare::create()),
							ConstantString::create("cmp rax, rbx\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentConditionalMov::create()),
							ConstantString::create("cmovge rax, rbx\n\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovge::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentGe::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentFinale::create())
								->setStringReplacementVariables({
									Strings::Menus_Hacking_Lexicon_Assembly_RegisterRax::create(),
									Strings::Menus_Hacking_Lexicon_Assembly_RegisterRbx::create(),
								}),
							ConstantString::create("\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_BrokenBlade_CommentHint::create()
								->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterRax::create()))
						})
					),
				},
				true
			)
		},
	};

	this->hackables = CREATE_HACKABLES(BrokenBlade::applyBrokenBlade, codeInfoMap);

	for (HackableCode* next : this->hackables)
	{
		this->owner->registerCode(next);
	}
}

void BrokenBlade::onBeforeDamageDealt(CombatEvents::ModifiableDamageOrHealingArgs* damageOrHealing)
{
	super::onBeforeDamageDealt(damageOrHealing);

	Buff::HackStateStorage[Buff::StateKeyDamageDealt] = Value(damageOrHealing->damageOrHealingValue);

	this->applyBrokenBlade();

	int min = -std::abs(Buff::HackStateStorage[Buff::StateKeyOriginalDamageOrHealing].asInt() * BrokenBlade::MaxMultiplier);
	int max = std::abs(Buff::HackStateStorage[Buff::StateKeyOriginalDamageOrHealing].asInt() * BrokenBlade::MaxMultiplier);

	*damageOrHealing->damageOrHealing = Buff::HackStateStorage[Buff::StateKeyDamageDealt].asInt();
	*damageOrHealing->damageOrHealingMin = min;
	*damageOrHealing->damageOrHealingMax = max;
}

NO_OPTIMIZE void BrokenBlade::applyBrokenBlade()
{
	static volatile int currentDamageDealtLocal = 0;

	currentDamageDealtLocal = GameUtils::getKeyOrDefault(Buff::HackStateStorage, Buff::StateKeyDamageDealt, Value(0)).asInt();

	ASM_PUSH_EFLAGS();
	ASM(push ZAX);
	ASM(push ZBX);

	ASM_MOV_REG_VAR(eax, currentDamageDealtLocal);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_BROKEN_BLADE);
	ASM(mov ZBX, 3);
	ASM(cmp ZAX, ZBX);
	ASM(cmovge ZAX, ZBX);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(currentDamageDealtLocal, eax);

	ASM(pop ZBX);
	ASM(pop ZAX);
	ASM_POP_EFLAGS();

	Buff::HackStateStorage[Buff::StateKeyDamageDealt] = Value(currentDamageDealtLocal);

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
