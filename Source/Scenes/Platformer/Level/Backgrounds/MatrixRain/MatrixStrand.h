#pragma once

#include "Engine/SmartNode.h"

class MatrixLetter;

class MatrixStrand : public SmartNode
{
public:
	static MatrixStrand* create(int strandIndex);

protected:
	MatrixStrand(int strandIndex);
	virtual ~MatrixStrand();

	void onEnter() override;
	void update(float) override;
	void pause(void) override;
	void onHackerModeEnable() override;

private:
	typedef SmartNode super;

	void nextStrandAction();
	void beginStrand();
	void killStrand();

	void randomizeInitialState(int strandIndex);
	void randomizeState(bool spawnInRange);

	std::vector<MatrixLetter*> letters;

	bool isAlive = true;
	float spawnSpeed = 0.0f;
	int currentLetterIndex = 0;
	int letterCount = 0;
	float elapsedDuration = 0.0f;
	bool hasBuiltLetters = false;

	static const int MinLetterCount;
	static const int MaxLetterCount;
	static const float MovementSpeed;
	static const float StrandScale;
	static const float MinSpawnSpeed;
	static const float MaxSpawnSpeed;
	static const float OverFlowY;
	static const float UnderFlowY;
	static const float MinSpawnDistance;
	static const float MaxSpawnDistance;
};
