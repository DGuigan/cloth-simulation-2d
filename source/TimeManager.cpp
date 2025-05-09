#include "TimeManager.h"
#include "Renderer.h"
#include "LevelManager.h"

TimeManager::TimeManager(const Vec2 position, const Vec2 dimensions)
	: position(position), dimensions(dimensions) {}

void TimeManager::Draw(const Renderer* renderer, const LevelManager* levelManager)
{
	const float timeProgress = 1.f - (timeElapsed / timeLimit);
	const float progressBarHeight = timeProgress < 1.f ? dimensions.y * timeProgress : dimensions.y;

	const Uint32 renderColor = levelManager->GetRenderColor(RenderElementType::UI);

	renderer->DrawRect(position, dimensions, renderColor, false);
	renderer->DrawRect({ position.x, position.y + (dimensions.y - progressBarHeight) }, { dimensions.x, progressBarHeight }, renderColor, true);
}
