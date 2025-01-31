#include "VoidAxePlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string VoidAxePlans::SaveKey = "void-axe-plans";

VoidAxePlans* VoidAxePlans::create()
{
	VoidAxePlans* instance = new VoidAxePlans();

	instance->autorelease();

	return instance;
}

VoidAxePlans::VoidAxePlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

VoidAxePlans::~VoidAxePlans()
{
}

std::vector<Item*> VoidAxePlans::craft()
{
	return { VoidAxe::create() };
}

std::vector<std::tuple<Item*, int>> VoidAxePlans::getReagentsInternal()
{
	return
	{
		{ VoidCrystal::create(), 5},
		{ Obsidian::create(), 4 },
		{ Gold::create(), 3 },
		{ Coal::create(), 4 },
	};
}

Item* VoidAxePlans::clone()
{
	return VoidAxePlans::create();
}

LocalizedString* VoidAxePlans::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_VoidAxe::create();
}

const std::string& VoidAxePlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string VoidAxePlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Axes_VoidAxe;
}

const std::string& VoidAxePlans::getIdentifier()
{
	return VoidAxePlans::SaveKey;
}
