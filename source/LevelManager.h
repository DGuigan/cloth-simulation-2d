#pragma once

#include "LevelData.h"
#include "Enums.h"
#include "SDL.h"

#include <vector>

class Renderer;
class Cloth;
class ScoreManager;
class TimeManager;

struct Application;

class LevelManager
{
private:
	std::vector<LevelData> levelData;

	int currentLevelIndex = 0;
	LevelState currentLevelState = LevelState::InProgress;

	float timeSpentWaitingForLevelEnd = 0.f;
	float secondsBeforeLevelEnd = 3.f;

	const Uint32 greenColor = 0x1EC900;
	const Uint32 orangeColor = 0xFCE803;
	const Uint32 redColor = 0xC90000;
	const Uint32 blueColor = 0xFF0048E3;
	const Uint32 whiteColor = 0xEDF2ED;
	const Uint32 tealColor = 0x03FCA5;
	const Uint32 blackColor = 0xFF000816;

	void IncrementLevelIndex();

public:
	void InitLevels(const Renderer* renderer);

	void LoadLevel(const int levelIndex, Application* application, Cloth* cloth, ScoreManager* scoreManager, TimeManager* timeManager);

	void StartLevel();

	void Update(Application* application, Cloth* cloth, ScoreManager* scoreManager, TimeManager* timeManager, const float deltaTime);

	Uint32 GetRenderColor(const RenderElementType renderElementType) const;

	LevelState GetCurrentLevelState() const { return currentLevelState; };

	void LoadNextLevel(Application* application, Cloth* cloth, ScoreManager* scoreManager, TimeManager* timeManager);
};