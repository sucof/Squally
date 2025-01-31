#include "Snowball.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string Snowball::SaveKey = "snowball";

Snowball* Snowball::create()
{
	Snowball* instance = new Snowball();

	instance->autorelease();

	return instance;
}

Snowball::Snowball() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 21 }}), ItemMeta(20000))
{
}

Snowball::~Snowball()
{
}

Item* Snowball::clone()
{
	return Snowball::create();
}

LocalizedString* Snowball::getString()
{
	return Strings::Items_Crafting_PotionReagents_Snowball::create();
}

const std::string& Snowball::getIconResource()
{
	return ItemResources::Crafting_Alchemy_Potions_T7_Snowball;
}

const std::string& Snowball::getIdentifier()
{
	return Snowball::SaveKey;
}
