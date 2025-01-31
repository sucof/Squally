#include "UnlockButton.h"

#include "cocos/2d/CCActionInterval.h"
#include "cocos/2d/CCSprite.h"
#include "cocos/base/CCDirector.h"

#include "Engine/Input/ClickableTextNode.h"
#include "Engine/Localization/LocalizedLabel.h"
#include "Scenes/Cipher/CipherConfig.h"
#include "Scenes/Cipher/CipherState.h"

#include "Resources/CipherResources.h"

#include "Strings/Strings.h"

using namespace cocos2d;

UnlockButton* UnlockButton::create()
{
	UnlockButton* instance = new UnlockButton();

	instance->autorelease();

	return instance;
}

UnlockButton::UnlockButton()
{
	LocalizedLabel*	unlockLabel = LocalizedLabel::create(LocalizedLabel::FontStyle::Main, LocalizedLabel::FontSize::H3, Strings::Cipher_Unlock::create());
	LocalizedLabel*	unlockLabelHover = unlockLabel->clone();

	unlockLabel->enableShadow(Color4B::BLACK, CSize(-2.0f, -2.0f), 2);
	unlockLabel->enableGlow(Color4B::BLACK);

	unlockLabelHover->setColor(Color3B::YELLOW);
	unlockLabelHover->enableShadow(Color4B::BLACK, CSize(-2.0f, -2.0f), 2);
	unlockLabelHover->enableGlow(Color4B::ORANGE);

	this->unlockButton = ClickableTextNode::create(
		unlockLabel,
		unlockLabelHover,
		CipherResources::Buttons_RunButton,
		CipherResources::Buttons_RunButtonSelected);

	this->addChild(this->unlockButton);
}

UnlockButton::~UnlockButton()
{
}

void UnlockButton::onEnter()
{
	super::onEnter();
}

void UnlockButton::initializePositions()
{
	super::initializePositions();

	CSize visibleSize = Director::getInstance()->getVisibleSize();

	this->unlockButton->setPosition(Vec2(visibleSize.width / 2.0f + CipherConfig::LeftColumnCenter, visibleSize.height / 2.0f - 420.0f));
}

void UnlockButton::initializeListeners()
{
	super::initializeListeners();

	this->unlockButton->setMouseClickCallback([=](InputEvents::MouseEventArgs* args)
	{
		if (this->activeCipherState != nullptr)
		{
			this->activeCipherState->updateState(this->activeCipherState, CipherState::StateType::TransitionUnlocking);
		}
	});
}

void UnlockButton::onAnyStateChange(CipherState* cipherState)
{
	super::onAnyStateChange(cipherState);

	switch(cipherState->stateType)
	{
		case CipherState::StateType::Neutral:
		{
			this->unlockButton->enableInteraction();
			break;
		}
		default:
		{
			this->unlockButton->disableInteraction();
			break;
		}
	}

	this->activeCipherState = cipherState;
}
