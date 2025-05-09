#include "LevelManager.h"

#include "Renderer.h"
#include "Application.h"
#include "Cloth.h"
#include "ScoreManager.h"
#include "TimeManager.h"

void LevelManager::InitLevels(const Renderer* renderer)
{
	{ // LEVEL 1
		LevelData& level1 = levelData.emplace_back();

		float clothWidth = 1200.f;
		float clothHeight = 320.f;
		float clothX = (renderer->GetWindowWidth() - clothWidth) * 0.5f;
		float clothY = renderer->GetWindowHeight() * 0.1f;

		level1.targetScore = 3000000;
		level1.timeLimit = 15.f;
		level1.clothData.push_back({ ClothType::Rectangular, 10.f, {clothWidth, clothHeight}, {clothX, clothY} });

		level1.fanData.push_back({ { 350, 400 }, { 1, 0 }, 100, 30 });
		level1.fanData.push_back({ { 1000, 500 }, { 0, -1 }, 100, 30 });

		level1.colorData = {
			tealColor,
			blackColor,
			blueColor,
			greenColor,
			redColor
		};
	}

	{ // LEVEL 2 
		LevelData& level2 = levelData.emplace_back();

		float clothWidth = 750.f;
		float clothHeight = 250.f;
		float clothX = (renderer->GetWindowWidth() - clothWidth) * 0.5f;
		float clothY = renderer->GetWindowHeight() * 0.1f;

		level2.targetScore = 2000000;
		level2.timeLimit = 20.f;
		level2.clothData.push_back({ ClothType::Rectangular, 10.f, {clothWidth, clothHeight}, {clothX, clothY} });

		level2.fanData.push_back({ { 350, 600 }, { 1, 1 }, 100, 30 });
		level2.fanData.push_back({ { 1000, 500 }, { 0, -1 }, 100, 30 });

		level2.colorData = {
			blackColor,
			whiteColor,
			blackColor,
			greenColor,
			redColor
		};
	}

	{
		LevelData& level3 = levelData.emplace_back();

		float clothWidth = 750.f;
		float clothHeight = 250.f;
		float clothX = renderer->GetWindowWidth() * 0.5f;
		float clothY = renderer->GetWindowHeight() * 0.2f;

		level3.targetScore = 2000000;
		level3.timeLimit = 20.f;

		level3.clothData.push_back({ ClothType::Circular, 10.f, {500.f, 175.f}, {renderer->GetWindowWidth() * 0.5f, renderer->GetWindowHeight() * 0.5f} });
		level3.clothData.push_back({ ClothType::Circular, 10.f, {250.f, 75.f}, {renderer->GetWindowWidth() * 0.2f, renderer->GetWindowHeight() * 0.3f} });
		level3.clothData.push_back({ ClothType::Circular, 10.f, {250.f, 75.f}, {renderer->GetWindowWidth() * 0.8f, renderer->GetWindowHeight() * 0.3f} });

		level3.fanData.push_back({ { 350, 600 }, { 1, 1 }, 100, 30 });
		level3.fanData.push_back({ { 1000, 500 }, { 0, -1 }, 100, 30 });

		level3.colorData = {
			yellowColor,
			greyColor,
			blueColor,
			greenColor,
			redColor
		};
	}
}

void LevelManager::LoadLevel(const int levelIndex, Application* application, Cloth* cloth, ScoreManager* scoreManager, TimeManager* timeManager)
{
	application->Reset();

	currentLevelState = LevelState::Design;
	timeSpentWaitingForLevelEnd = 0.f;

	if (levelIndex != -1)
	{
		currentLevelIndex = levelIndex;
	}

	const LevelData& level = levelData[currentLevelIndex];

	scoreManager->SetTargetScore(level.targetScore);

	timeManager->SetTimeLimit(level.timeLimit);

	for (const ClothData& clothData : level.clothData)
	{
		cloth->AddWeave(clothData);
	}

	for (const FanData& fanData : level.fanData)
	{
		cloth->AddFan(fanData);
	}
}

void LevelManager::StartLevel()
{
	if (GetCurrentLevelState() == LevelState::Design)
	{
		currentLevelState = LevelState::InProgress;
	}
}

void LevelManager::Update(Application* application, Cloth* cloth, ScoreManager* scoreManager, TimeManager* timeManager, const float deltaTime)
{
	if (currentLevelState == LevelState::InProgress)
	{
		if (scoreManager->GetTargetScoreReached())
		{
			currentLevelState = LevelState::Completed;
		}
		else if (cloth->GetNumActivePoints() == 0 || timeManager->GetTimeLimitReached())
		{
			currentLevelState = LevelState::Failed;
		}
	}

	if (currentLevelState == LevelState::Completed || currentLevelState == LevelState::Failed)
	{
		timeSpentWaitingForLevelEnd += deltaTime;

		if (secondsBeforeLevelEnd <= timeSpentWaitingForLevelEnd)
		{
			if (currentLevelState == LevelState::Completed)
			{
				IncrementLevelIndex();
			}

			LoadLevel(currentLevelIndex, application, cloth, scoreManager, timeManager);
		}
	}
}

Uint32 LevelManager::GetRenderColor(const RenderElementType renderElementType) const
{
	const LevelColorData& colorData = levelData[currentLevelIndex].colorData;

	if (renderElementType == RenderElementType::Background)
	{
		return colorData.backgroundColor;
	}

	switch (GetCurrentLevelState())
	{
	case (LevelState::Completed):
	{
		return colorData.successColor;
	}
	case (LevelState::Failed):
	{
		return colorData.failureColor;
	}
	default:
		switch (renderElementType)
		{
		case (RenderElementType::UI):
		{
			return colorData.uiColor;
		}
		case (RenderElementType::Cloth):
		{
			return colorData.clothColor;
		}
		default:
		{
			return 0xFFFFFF;
		}
		}
	}
}

void LevelManager::LoadNextLevel(Application* application, Cloth* cloth, ScoreManager* scoreManager, TimeManager* timeManager)
{
	IncrementLevelIndex();
	LoadLevel(-1, application, cloth, scoreManager, timeManager);
}

void LevelManager::IncrementLevelIndex()
{
	currentLevelIndex = (currentLevelIndex + 1) % levelData.size();
}

