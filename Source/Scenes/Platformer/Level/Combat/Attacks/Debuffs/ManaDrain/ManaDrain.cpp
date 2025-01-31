#include "ManaDrain.h"

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
#include "Engine/Sound/WorldSound.h"
#include "Engine/Utils/GameUtils.h"
#include "Engine/Utils/MathUtils.h"
#include "Entities/Platformer/PlatformerEntity.h"
#include "Events/CombatEvents.h"
#include "Events/PlatformerEvents.h"
#include "Scenes/Platformer/Hackables/HackFlags.h"
#include "Scenes/Platformer/Level/Combat/Attacks/Debuffs/ManaDrain/ManaDrainGenericPreview.h"
#include "Scenes/Platformer/Level/Combat/CombatMap.h"
#include "Scenes/Platformer/Level/Combat/TimelineEvent.h"
#include "Scenes/Platformer/Level/Combat/TimelineEventGroup.h"
#include "Scenes/Platformer/State/StateKeys.h"

#include "Resources/FXResources.h"
#include "Resources/ItemResources.h"
#include "Resources/SoundResources.h"
#include "Resources/UIResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

#define LOCAL_FUNC_ID_MANA_DRAIN 1

const std::string ManaDrain::ManaDrainIdentifier = "mana-drain";
const int ManaDrain::DrainAmount = 7;
const float ManaDrain::TimeBetweenTicks = 0.75f;
const float ManaDrain::StartDelay = 0.5f;

ManaDrain* ManaDrain::create(PlatformerEntity* caster, PlatformerEntity* target)
{
	ManaDrain* instance = new ManaDrain(caster, target);

	instance->autorelease();

	return instance;
}

ManaDrain::ManaDrain(PlatformerEntity* caster, PlatformerEntity* target)
	: super(caster, target, UIResources::Menus_Icons_ManaSkull, AbilityType::Shadow, BuffData())
{
	this->healEffect = SmartAnimationSequenceNode::create();
	this->drainAmount = ManaDrain::DrainAmount;
	this->impactSound = WorldSound::create(SoundResources::Platformer_Spells_Heal2);
	this->healSound = WorldSound::create(SoundResources::Platformer_Spells_Ding1);

	this->healEffect->setAnimationAnchor(Vec2(0.5f, 0.0f));
	
	this->addChild(this->healEffect);
	this->addChild(this->impactSound);
	this->addChild(this->healSound);
}

ManaDrain::~ManaDrain()
{
}

void ManaDrain::onEnter()
{
	super::onEnter();

	this->runManaDrain();

	CombatEvents::TriggerHackableCombatCue();
}

void ManaDrain::initializePositions()
{
	super::initializePositions();

	this->healEffect->setPositionY(this->owner->getEntityBottomPointRelative().y - 12.0f);
}

void ManaDrain::registerHackables()
{
	super::registerHackables();

	if (this->owner == nullptr)
	{
		return;
	}

	HackableCode::CodeInfoMap codeInfoMap =
	{
		{
			LOCAL_FUNC_ID_MANA_DRAIN,
			HackableCode::HackableCodeInfo(
				ManaDrain::ManaDrainIdentifier,
				Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_ManaDrain::create(),
				HackableBase::HackBarColor::Blue,
				UIResources::Menus_Icons_ManaSkull,
				LazyNode<HackablePreview>::create([=](){ return ManaDrainGenericPreview::create(); }),
				{
					{
						HackableCode::Register::zcx, Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_RegisterEcx::create(), HackableDataType::Int32,
					},
					{
						HackableCode::Register::zdx, Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_RegisterEdx::create(), HackableDataType::Int32,
					},
					{
						HackableCode::Register::zsi, Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_RegisterEsi::create(), HackableDataType::Int32,
					}
				},
				int(HackFlags::None),
				this->getRemainingDuration(),
				0.0f,
				{
					HackableCode::ReadOnlyScript(
						Strings::Menus_Hacking_CodeEditor_OriginalCode::create(),
						// x86
						ConcatString::create({
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentCompare::create()),
							ConstantString::create("cmp ecx, 4\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentConditionalMov::create()
								->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterEsi::create())),
							ConstantString::create("cmovge esi, edx\n\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovge::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentGe::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentFinale::create()),
							ConstantString::create("\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentHint::create()
								->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterEsi::create()))
						})
						, // x64
						ConcatString::create({
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentCompare::create()),
							ConstantString::create("cmp rcx, 4\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentConditionalMov::create()
								->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterEsi::create())),
							ConstantString::create("cmovge rsi, rdx\n\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentCmovge::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentC::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentMov::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Generic_Conditional_CommentGe::create()),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentFinale::create()),
							ConstantString::create("\n"),
							COMMENT(Strings::Menus_Hacking_Abilities_Debuffs_ManaDrain_CommentHint::create()
								->setStringReplacementVariables(Strings::Menus_Hacking_Lexicon_Assembly_RegisterRsi::create()))
						})
					),
				},
				true
			)
		},
	};

	this->hackables = CREATE_HACKABLES(ManaDrain::runRestoreTick, codeInfoMap);

	for (HackableCode* next : this->hackables)
	{
		this->owner->registerCode(next);
	}
}

void ManaDrain::runManaDrain()
{
	this->impactSound->play();

	std::vector<TimelineEvent*> timelineEvents = std::vector<TimelineEvent*>();

	for (int healIndex = 0; healIndex < this->drainAmount; healIndex++)
	{
		Sprite* icon = Sprite::create(UIResources::Menus_Icons_ManaSkull);

		icon->setScale(0.5f);

		timelineEvents.push_back(TimelineEvent::create(
				this->owner,
				icon,
				ManaDrain::TimeBetweenTicks * float(healIndex) + ManaDrain::StartDelay, [=]()
			{
				if (!this->healEffect->isPlayingAnimation())
				{
					this->healEffect->playAnimation(FXResources::HealMP_Heal_0000, 0.05f);
				}
				
				this->runRestoreTick();
			})
		);
	}

	CombatEvents::TriggerRegisterTimelineEventGroup(CombatEvents::RegisterTimelineEventGroupArgs(
		TimelineEventGroup::create(timelineEvents, this, this->owner, [=]()
		{
			this->removeBuff();
		})
	));
}

NO_OPTIMIZE void ManaDrain::runRestoreTick()
{
	static volatile int drainAmount = 0;
	static volatile int currentMana = 0;

	drainAmount = 0;
	currentMana = this->owner->getRuntimeStateOrDefaultInt(StateKeys::Mana, 0);

	ASM_PUSH_EFLAGS();
	ASM(push ZCX);
	ASM(push ZDX);
	ASM(push ZSI);

	ASM_MOV_REG_VAR(ecx, currentMana);
	ASM(mov ZSI, 0);
	ASM(mov ZDX, 1);

	HACKABLE_CODE_BEGIN(LOCAL_FUNC_ID_MANA_DRAIN);
	ASM(cmp ZCX, 4);
	ASM(cmovge ZSI, ZDX);
	ASM_NOP16();
	HACKABLE_CODE_END();

	ASM_MOV_VAR_REG(drainAmount, esi);

	ASM(pop ZSI);
	ASM(pop ZDX);
	ASM(pop ZCX);
	ASM_POP_EFLAGS();

	HACKABLES_STOP_SEARCH();

	bool overflowedMin = drainAmount >= 3;
	bool overflowedMax = drainAmount <= -3;
	drainAmount = MathUtils::clamp(drainAmount, -3, 3);

	this->healSound->play();
	CombatEvents::TriggerManaDrain(CombatEvents::ManaRestoreOrDrainArgs(this->caster, this->owner, drainAmount, this->abilityType, true, overflowedMin, overflowedMax));

	HACKABLES_STOP_SEARCH();
}
END_NO_OPTIMIZE
