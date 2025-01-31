#pragma once

#include "Scenes/Platformer/Inventory/Items/Misc/Keys/Key.h"

class RustyKey : public Key
{
public:
	static RustyKey* create();

	Item* clone() override;
	LocalizedString* getString() override;
	const std::string& getIconResource() override;
	const std::string& getIdentifier() override;

	static const std::string SaveKey;

protected:
	RustyKey();
	virtual ~RustyKey();

private:
	typedef Key super;
};
