#include "IvyBow.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/Recipes/Weapons/Bows/IvyBowPlans.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string IvyBow::SaveKey = "ivy-bow";

IvyBow* IvyBow::create()
{
	IvyBow* instance = new IvyBow();

	instance->autorelease();

	return instance;
}

IvyBow::IvyBow() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}), BOW_DAMAGE_MIN(5), BOW_DAMAGE_MAX(5), ItemStats(
	// Health
	2,
	// Mana
	5,
	// Attack
	0,
	// Magic Attack
	0,
	// Armor
	0,
	// Speed
	0.25f
))
{
}

IvyBow::~IvyBow()
{
}

Item* IvyBow::clone()
{
	return IvyBow::create();
}

LocalizedString* IvyBow::getString()
{
	return Strings::Items_Equipment_Weapons_Bows_IvyBow::create();
}

const std::string& IvyBow::getIconResource()
{
	return ItemResources::Equipment_Weapons_Bows_IvyBow;
}

const std::string& IvyBow::getIdentifier()
{
	return IvyBow::SaveKey;
}

Vec2 IvyBow::getDisplayOffset()
{
	return Vec2(-10.0f, -66.0f);
}

Recipe* IvyBow::getRecipe()
{
	return IvyBowPlans::create();
}
