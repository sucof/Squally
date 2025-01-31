#include "MithrilAxePlans.h"

#include "Engine/Inventory/CurrencyInventory.h"
#include "Scenes/Platformer/Inventory/Currencies/IOU.h"
#include "Scenes/Platformer/Inventory/Items/PlatformerItems.h"

#include "Resources/ItemResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

const std::string MithrilAxePlans::SaveKey = "mithril-axe-plans";

MithrilAxePlans* MithrilAxePlans::create()
{
	MithrilAxePlans* instance = new MithrilAxePlans();

	instance->autorelease();

	return instance;
}

MithrilAxePlans::MithrilAxePlans() : super(CurrencyInventory::create({{ IOU::getIOUIdentifier(), 2 }}))
{
}

MithrilAxePlans::~MithrilAxePlans()
{
}

std::vector<Item*> MithrilAxePlans::craft()
{
	return { MithrilAxe::create() };
}

std::vector<std::tuple<Item*, int>> MithrilAxePlans::getReagentsInternal()
{
	return
	{
		{ Mithril::create(), 5},
		{ Iron::create(), 4 },
		{ Coal::create(), 4 },
	};
}

Item* MithrilAxePlans::clone()
{
	return MithrilAxePlans::create();
}

LocalizedString* MithrilAxePlans::getString()
{
	return Strings::Items_Equipment_Weapons_Axes_MithrilAxe::create();
}

const std::string& MithrilAxePlans::getIconResource()
{
	return ItemResources::Misc_SCROLL_1;
}

std::string MithrilAxePlans::getCraftedItemIconResource()
{
	return ItemResources::Equipment_Weapons_Axes_MithrilAxe;
}

const std::string& MithrilAxePlans::getIdentifier()
{
	return MithrilAxePlans::SaveKey;
}
