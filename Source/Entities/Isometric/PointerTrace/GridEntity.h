#pragma once

#include "Entities/Isometric/IsometricEntity.h"

namespace cocos2d
{
	class Sprite;
}

class LocalizedLabel;

class GridEntity : public IsometricEntity
{
public:
	void interruptMovement();
	void uninterruptMovement();
	bool isMovementInterrupted();
	void lockMovement();
	void unlockMovement();
	bool isMovementLocked();
	int getInitialGridIndex();
	int getGridIndex();
	void setInitialGridIndex(int gridIndex);
	void setGridIndex(int gridIndex);
	void runJumpAnimation(cocos2d::Vec2 destPosition, std::function<void()> callback = nullptr);

protected:
	GridEntity(cocos2d::ValueMap& properties,
		std::string scmlResource,
		float scale = 1.0f,
		cocos2d::Vec2 offset = cocos2d::Vec2(0.0f, 0.0f),
		cocos2d::CSize size = cocos2d::CSize(256.0f, 128.0f));
	virtual ~GridEntity();

	void onEnter() override;
	void initializePositions() override;
	void moveLeft(float speed);
	void moveRight(float speed);
	void moveUp(float speed);
	void moveDown(float speed);

private:
	typedef IsometricEntity super;

	cocos2d::Sprite* shadow = nullptr;
	
	bool movementInterrupted = false;
	bool movementLocked = false;
	int gridIndex = 0;
	int initialGridIndex = 0;
};
