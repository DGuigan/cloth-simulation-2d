#include "Cloth.h"
#include "Stick.h"
#include "Renderer.h"
#include "Point.h"
#include "Fan.h"
#include "InputHandler.h"
#include "LevelData.h"
#include "Enums.h"
#include "SDL.h"
#include "LevelManager.h"

#include <iostream>

Cloth::Cloth(int numColumns, int numRows, int spacing, int startX, int startY)
{
	CreateRectangularCloth(numColumns, numRows, spacing, startX, startY);
}

void Cloth::AddWeave(const ClothData& clothData)
{
	switch (clothData.clothType)
	{
	case (ClothType::Rectangular):
	{
		CreateRectangularCloth(clothData.dimensions.x, clothData.dimensions.y, clothData.spacing, clothData.position.x, clothData.position.y);
		break;
	}
	case (ClothType::Circular):
	{
		SDL_assert(false);
		break;
	}
	default:
	{
		SDL_assert(false);
		break;
	}
	}
}

void Cloth::AddFan(const FanData& fanData)
{
	fans.push_back(new Fan({ fanData.position, fanData.direction, fanData.magnitude, fanData.angle }));
}

void Cloth::CreateRectangularCloth(float width, float height, int spacing, int startX, int startY)
{
	int numColumns = width / spacing;
	int numRows = height / spacing;

	for (int y = 0; y <= numRows; y++) {
		for (int x = 0; x <= numColumns; x++)
		{
			Point* point = new Point(startX + x * spacing, startY + y * spacing);
			activePoints++;

			if (x != 0)
			{
				Point* leftPoint = points[this->points.size() - 1];
				Stick* s = new Stick(*point, *leftPoint, spacing);
				leftPoint->AddStick(s);
				point->AddStick(s);
				sticks.push_back(s);
			}

			if (y != 0)
			{
				Point* upPoint = points[x + (y - 1) * (numColumns + 1)];
				Stick* s = new Stick(*point, *upPoint, spacing);
				upPoint->AddStick(s);
				point->AddStick(s);
				sticks.push_back(s);
			}

			if (y == 0 && x % 2 == 0 && false)
			{
				point->SetPinned(true);
			}

			points.push_back(point);
		}
	}
}


void Cloth::Reset()
{
	for (Point* point : points)
	{
		delete point;
	}

	for (Stick* stick : sticks)
	{
		delete stick;
	}

	for (Fan* fan : fans)
	{
		delete fan;
	}

	points.clear();
	sticks.clear();
	fans.clear();

	closestSelectedPointIndex = -1;
	rightPointIndex = -1;
	leftPointIndex = -1;

	activePoints = 0;
}


void Cloth::Update(LevelState levelState, InputHandler* inputHandler, Renderer* renderer, float deltaTime)
{
	//UpdateSelection(inputHandler);

	if (levelState == LevelState::InProgress)
	{
		UpdateSimulation(renderer, inputHandler, deltaTime);
	}
	else if (levelState == LevelState::Design)
	{
		UpdateDesign(inputHandler);
	}
}

void Cloth::UpdateSelection(InputHandler* inputHandler)
{
	closestSelectedPointIndex = -1;
	float closestSelectedPointDistance = 0.f;

	if (!inputHandler->GetLeftMouseButtonDown())
	{
		leftPointIndex = -1;
		rightPointIndex = -1;
	}

	for (int i = 0; i < points.size(); ++i)
	{
		const float distance = points[i]->UpdateSelection(inputHandler);

		if (distance > -1.f && (closestSelectedPointIndex == -1 || distance < closestSelectedPointDistance))
		{
			if (inputHandler->GetLeftMouseButtonDown())
			{
				leftPointIndex = rightPointIndex;
				rightPointIndex = i;
			}

			closestSelectedPointIndex = i;
			closestSelectedPointDistance = distance;

		}
	}
}

void Cloth::UpdateSimulation(Renderer* renderer, InputHandler* inputHandler, float deltaTime)
{
	for (int i = 0; i < points.size(); i++)
	{
		Point* point = points[i];
		point->Update(deltaTime, drag, gravity, elasticity, &fans, inputHandler, this, renderer->GetWindowWidth(), renderer->GetWindowHeight());
	}

	for (int i = 0; i < sticks.size(); i++)
	{
		sticks[i]->Update();
	}

	UpdateFans(inputHandler);
}

void Cloth::UpdateFans(InputHandler* inputHandler)
{
	for (int i = 0; i < fans.size(); i++)
	{
		fans[i]->Update(inputHandler);
	}
}

void Cloth::UpdateDesign(InputHandler* inputHandler)
{
	// Not currently allowing points/fans to be placed by user at design time but could be a cool gamemode
	// 
	// UpdateClothDesign(inputHandler);

	//if (inputHandler->GetFDown())
	//{
	//	fans.push_back(new Fan(inputHandler->GetMousePosition()));
	//	inputHandler->SetFDown(false);
	//}

	UpdateFans(inputHandler);
}

void Cloth::UpdateClothDesign(InputHandler* inputHandler)
{
	if (inputHandler->GetLeftMouseButtonJustClicked())
	{
		const Vec2 position = inputHandler->GetMousePosition();

		Point* point = nullptr;

		if (closestSelectedPointIndex == -1)
		{
			point = new Point(position.x, position.y);
			points.push_back(point);
			activePoints++;
		}
		else
		{
			point = points[closestSelectedPointIndex];
		}

		if (inputHandler->GetLeftShiftDown())
		{
			point->SetPinned(!point->GetPinned());
		}
	}

	const bool validLeftPoint = leftPointIndex != -1 && leftPointIndex < points.size();
	const bool validRightPoint = rightPointIndex != -1 && rightPointIndex < points.size();
	const bool uniquePoints = leftPointIndex != rightPointIndex;
	const bool shouldConnectPoints = validLeftPoint && validRightPoint && uniquePoints;

	if (inputHandler->GetLeftMouseButtonDown() && shouldConnectPoints)
	{
		Point* leftPoint = points[leftPointIndex];
		Point* rightPoint = points[rightPointIndex];

		if (leftPoint && rightPoint)
		{
			float distance = Vec2::Distance(leftPoint->GetPosition(), rightPoint->GetPosition());
			Stick* stick = new Stick(*leftPoint, *rightPoint, distance);
			leftPoint->AddStick(stick);
			rightPoint->AddStick(stick);
			sticks.push_back(stick);
		}
	}
}

void Cloth::Draw(Renderer* renderer, LevelManager* levelManager, const bool drawPoints, const bool drawSticks) const
{
	const Uint32 clothRenderColor = levelManager->GetRenderColor(RenderElementType::Cloth);

	if (drawPoints)
	{
		for (const Point* point : points)
		{
			point->Draw(renderer, clothRenderColor);
		}
	}

	if (drawSticks)
	{
		for (const Stick* stick : sticks)
		{
			stick->Draw(renderer, clothRenderColor);
		}
	}

	for (const Fan* fan : fans)
	{
		fan->Draw(renderer, levelManager);
	}
}

void Cloth::OnPointRemoved()
{
	if (0 < activePoints)
	{
		activePoints--;
	}
	else
	{
		SDL_assert(false);
	}
}


Cloth::~Cloth()
{
	Reset();
}
