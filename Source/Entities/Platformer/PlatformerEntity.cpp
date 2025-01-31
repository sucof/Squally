#include "PlatformerEntity.h"

#include "cocos/2d/CCActionInstant.h"
#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"
#include "cocos/base/CCEventCustom.h"
#include "cocos/base/CCEventListenerCustom.h"

#include "Deserializers/Platformer/PlatformerEntityDeserializer.h"
#include "Engine/Animations/AnimationPart.h"
#include "Engine/Animations/SmartAnimationNode.h"
#include "Engine/Camera/GameCamera.h"
#include "Engine/Dialogue/SpeechBubble.h"
#include "Engine/Hackables/Menus/HackablePreview.h"
#include "Engine/Inventory/CurrencyInventory.h"
#include "Engine/Inventory/Inventory.h"
#include "Engine/Physics/CollisionObject.h"
#include "Engine/Utils/GameUtils.h"
#include "Events/PlatformerEvents.h"
#include "Entities/Platformer/EntityPreview.h"
#include "Scenes/Platformer/Level/Combat/Attacks/PlatformerAttack.h"
#include "Scenes/Platformer/Inventory/EquipmentInventory.h"

#include "Resources/EntityResources.h"
#include "Resources/UIResources.h"
#include "Resources/SoundResources.h"

using namespace cocos2d;

const std::string PlatformerEntity::PlatformerEntityTag = "platformer-entity";
const std::string PlatformerEntity::PropertyBattleBehavior = "battle-behavior";
const std::string PlatformerEntity::PropertyState = "state";

PlatformerEntity::PlatformerEntity(
	ValueMap& properties, 
	std::string entityName,
	std::string scmlResource,
	std::string emblemResource,
	CSize size,
	float scale, 
	float hoverHeight
	) : super(properties)
{
	this->floatNode = Node::create();
	this->belowAnimationNode = Node::create();
	this->dropShadow = Sprite::create(EntityResources::DropShadow);
	this->animationNode = SmartAnimationNode::create(scmlResource);
	this->entityScale = scale * GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyScale, Value(1.0f)).asFloat();
	this->animationResource = scmlResource;
	this->emblemResource = emblemResource;
	this->entityName = entityName;
	this->state = GameUtils::getKeyOrDefault(this->properties, PlatformerEntity::PropertyState, Value("")).asString();
	this->battleBehavior = entityName + "-combat," + GameUtils::getKeyOrDefault(this->properties, PlatformerEntity::PropertyBattleBehavior, Value("")).asString();
	this->entitySize = size * this->entityScale;
	this->platformerEntityDeserializer = PlatformerEntityDeserializer::create();

	// Tag all entities by class to optimize object queries (ObjectEvents.h)
	this->addTag(PlatformerEntity::PlatformerEntityTag);
	this->addTag(PlatformerEntity::entityName);
	
	this->hoverHeight = hoverHeight;
	this->controlState = ControlState::Normal;
	this->controlStateOverride = ControlState::None;

	this->hackButtonOffset = Vec2(0.0f, this->entitySize.height);

	this->dropShadow->setAnchorPoint(Vec2(0.5f, 0.5f));
	this->dropShadow->setOpacity(0);

	this->animationNode->setScale(this->entityScale);
	this->animationNode->playAnimation();
	
	this->animationNode->setAnchorPoint(Vec2(0.5f, 0.0f));
	this->setAnchorPoint(Vec2(0.5f, 0.0f));

	this->animationNode->setFlippedX(GameUtils::getKeyOrDefault(this->properties, PlatformerEntity::MapKeyFlipX, Value(false)).asBool());

	if (GameUtils::getKeyOrDefault(this->properties, PlatformerEntity::MapKeyFlipY, Value(false)).asBool())
	{
		this->setRotation(180.0f);
	}

	this->addChild(this->dropShadow);
	this->floatNode->addChild(this->belowAnimationNode);
	this->floatNode->addChild(this->animationNode);
	this->addChild(this->platformerEntityDeserializer);
	this->addChild(this->floatNode);
}

PlatformerEntity::~PlatformerEntity()
{
}

void PlatformerEntity::onEnter()
{
	super::onEnter();

	this->scheduleUpdate();
}

void PlatformerEntity::update(float dt)
{
	super::update(dt);

	this->optimizationHideOffscreenEntity();
}

Vec2 PlatformerEntity::getRainOffset()
{
	return this->getEntityCenterPoint();
}

Vec2 PlatformerEntity::getButtonOffset()
{
	return this->hackButtonOffset;
}

HackablePreview* PlatformerEntity::createDefaultPreview()
{
	return EntityPreview::create(this);
}

std::string PlatformerEntity::getEntityKey()
{
	return this->entityName;
}

std::string PlatformerEntity::getSwimAnimation()
{
	return "Swim";
}

std::string PlatformerEntity::getJumpAnimation()
{
	return "Jump";
}

std::string PlatformerEntity::getJumpSound()
{
	return SoundResources::Platformer_Entities_Generic_Movement_Jump3;
}

std::vector<std::string> PlatformerEntity::getSwimSounds()
{
	return { SoundResources::Platformer_Environment_Swim1, SoundResources::Platformer_Environment_Swim2, SoundResources::Platformer_Environment_Swim3 };
}

std::vector<std::string> PlatformerEntity::getWalkSounds()
{
	return { SoundResources::Platformer_Entities_Generic_Movement_Steps1, SoundResources::Platformer_Entities_Generic_Movement_Steps2, SoundResources::Platformer_Entities_Generic_Movement_Steps3 };
}

PlatformerEntity::ControlState PlatformerEntity::getControlState()
{
	return (this->controlStateOverride == PlatformerEntity::ControlState::None) ? this->controlState : this->controlStateOverride;
}

std::string PlatformerEntity::getBattleBehavior()
{
	return this->battleBehavior;
}

Node* PlatformerEntity::getFloatNode()
{
	return this->floatNode;
}

cocos2d::Sprite* PlatformerEntity::getDropShadow()
{
	return this->dropShadow;
}

SmartAnimationNode* PlatformerEntity::getAnimations()
{
	return this->animationNode;
}

CSize PlatformerEntity::getEntitySize()
{
	return this->entitySize;
}

Vec2 PlatformerEntity::getEntityCenterPoint()
{
	float centerPoint = this->getEntitySize().height / 2.0f;

	return Vec2(0.0f, centerPoint);
}

Vec2 PlatformerEntity::getEntityTopPoint()
{
	Vec2 center = this->getEntityCenterPoint();
	Vec2 offset = this->getEntityTopPointRelative();

	return center + offset;
}

Vec2 PlatformerEntity::getEntityBottomPoint()
{
	Vec2 center = this->getEntityCenterPoint();
	Vec2 offset = this->getEntityBottomPointRelative();

	return center + offset;
}

Vec2 PlatformerEntity::getEntityTopPointRelative()
{
	float topPoint = this->getEntitySize().height / 2.0f;

	return Vec2(0.0f, this->isFlippedY() ? -topPoint : topPoint);
}

Vec2 PlatformerEntity::getEntityBottomPointRelative()
{
	float bottomPoint = -this->getEntitySize().height / 2.0f - this->getHoverHeight();

	return Vec2(0.0f, this->isFlippedY() ? -bottomPoint : bottomPoint);
}

float PlatformerEntity::getHoverHeight()
{
	return this->hoverHeight;
}

float PlatformerEntity::getScale()
{
	return this->entityScale;
}

std::string PlatformerEntity::getAnimationResource()
{
	return this->animationResource;
}

std::string PlatformerEntity::getEmblemResource()
{
	return this->emblemResource;
}

bool PlatformerEntity::isFlippedX()
{
	return this->getAnimations()->getFlippedX();
}

bool PlatformerEntity::isFlippedY()
{
	// Flipped Y actually involves in rotating the entire entity. This is to properly position collision boxes (flipping the anims wouldnt do this)
	// As of now, this value doesn't change at run-time, so just read it from properties.
	return GameUtils::getKeyOrDefault(this->properties, PlatformerEntity::MapKeyFlipY, Value(false)).asBool();
}

PlatformerEntity* PlatformerEntity::uiClone()
{
	PlatformerEntity* softClone = nullptr;
	ValueMap properties = ValueMap();

	properties[GameObject::MapKeyType] = PlatformerEntityDeserializer::MapKeyTypeEntity;
	properties[GameObject::MapKeyName] = Value(this->entityName);
	properties[GameObject::MapKeyScale] = GameUtils::getKeyOrDefault(this->properties, GameObject::MapKeyScale, Value(1.0f));
	properties[GameObject::MapKeyComponent] = "disarmed";
	properties[GameObject::MapKeyQueryable] = Value(false);

	ObjectDeserializer::ObjectDeserializationRequestArgs args = ObjectDeserializer::ObjectDeserializationRequestArgs(
		properties,
		[&] (ObjectDeserializer::ObjectDeserializationArgs deserializeArgs)
		{
			softClone = dynamic_cast<PlatformerEntity*>(deserializeArgs.gameObject);
		}
	);

	this->platformerEntityDeserializer->deserialize(&args);

	return softClone;
}

void PlatformerEntity::optimizationHideOffscreenEntity()
{
	// Slight padding for any rendering beyond the entity hitbox
	static const CSize Padding = CSize(768.0f, 768.0f);
	static const CRect CameraRect = CRect(Vec2::ZERO, Director::getInstance()->getVisibleSize());
	CRect thisRect = GameUtils::getScreenBounds(this->animationNode, this->getEntitySize() + Padding, false);

	if (CameraRect.intersectsRect(thisRect))
	{
		this->animationNode->enableRender();

		// animationNode can be set visible by other scripts, use the parent node instead to avoid conflicts
		this->floatNode->setVisible(true);
	}
	else
	{
		this->animationNode->disableRender();
		this->floatNode->setVisible(false);
	}
}
