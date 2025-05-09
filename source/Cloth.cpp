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
		CreateCircularCloth(clothData.dimensions.x, clothData.dimensions.y, clothData.spacing, clothData.position.x, clothData.position.y);
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

void Cloth::AddStick(const int pointAIndex, const int pointBIndex)
{
	AddStick(points[pointAIndex], points[pointBIndex]);
}

void Cloth::AddStick(Point* pointA, const int pointBIndex)
{
	AddStick(pointA, points[pointBIndex]);
}

void Cloth::AddStick(Point* pointA, Point* pointB)
{
	Stick* s = new Stick(*pointA, *pointB, Vec2::Distance(pointA->GetPosition(), pointB->GetPosition()));
	pointA->AddStick(s);
	pointB->AddStick(s);
	sticks.push_back(s);
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
				AddStick(point, this->points.size() - 1);
			}

			if (y != 0)
			{
				AddStick(point, x + (y - 1) * (numColumns + 1));
			}

			if (y == 0 && x % 2 == 0 && false)
			{
				point->SetPinned(true);
			}

			points.push_back(point);
		}
	}
}

void Cloth::CreateCircularCloth(float width, float height, int spacing, int centerX, int centerY)
{
	int numRows = height / spacing;

	int previousInitialIndex = points.size();
	int previousNumPointsAdded = 1;

	// create root point
	points.push_back(new Point(centerX, centerY));
	activePoints++;

	int pointAIndex = previousInitialIndex;
	int pointBIndex = previousInitialIndex;
	int pointCIndex = previousInitialIndex;
	int pointDIndex = previousInitialIndex;

	for (int i = 0; i < numRows; i++)
	{
		const float ratio = static_cast<float>(i + 1) / static_cast<float>(numRows);

		const int numPointsBefore = points.size();

		CreateSingleCircularCloth(width * ratio, height * ratio, spacing, centerX, centerY);

		const int numPointsAfter = points.size();

		const int numPointsAdded = numPointsAfter - numPointsBefore;

		AddStick(pointAIndex, numPointsAfter - 1);
		AddStick(pointBIndex, numPointsAfter - 2);
		AddStick(pointCIndex, numPointsAfter - 3);
		AddStick(pointDIndex, numPointsAfter - 4);

		pointAIndex = numPointsAfter - 1;
		pointBIndex = numPointsAfter - 2;
		pointCIndex = numPointsAfter - 3;
		pointDIndex = numPointsAfter - 4;

		// Iterate through all the points added in the previous iteration mapping them to points added in this iteration
		for (int j = 0; j < numPointsAdded - 4; j++)
		{
			int quadrant = j % 4;

			int currentPointIndex = numPointsBefore + j;

			float currentPointPositionNormalized = static_cast<float>(j) / numPointsAdded;

			int localPointToMapTo = static_cast<int>(previousNumPointsAdded * currentPointPositionNormalized);
			int localPointQuadrant = localPointToMapTo % 4;
			if (localPointQuadrant != quadrant && i != 0)
			{
				localPointToMapTo += (quadrant - localPointQuadrant) % 4;
			}

			int pointIndexToMapTo = previousInitialIndex + localPointToMapTo;

			AddStick(currentPointIndex, pointIndexToMapTo);
		}

		previousNumPointsAdded = numPointsAdded;
		previousInitialIndex = numPointsBefore;
	}
}

void Cloth::CreateSingleCircularCloth(float width, float height, int spacing, int centerX, int centerY)
{
	int numColumns = width / spacing;
	float widthSquared = width * width;
	float heightSquared = height * height;
	const int numPointsPerLoop = 4;

	const int firstPointIndex = points.size();

	// generate all the points other than the four at the end of each axis to avoid duplication
	for (int i = 0; i < numColumns - 1; i++)
	{
		float x = (i + 1) * spacing;
		float xSquared = x * x;

		float y = sqrt(heightSquared - ((heightSquared * xSquared) / widthSquared));

		// Order is very important here as the rest of the function relies on it
		points.push_back(new Point(centerX + x, centerY + y));
		points.push_back(new Point(centerX - x, centerY + y));
		points.push_back(new Point(centerX + x, centerY - y));
		points.push_back(new Point(centerX - x, centerY - y));
		activePoints += numPointsPerLoop;

		int basePointIndex = firstPointIndex + (i * numPointsPerLoop);
		if (i != 0)
		{
			for (int j = 0; j < numPointsPerLoop; j++)
			{
				int pointAIndex = basePointIndex + j;
				int pointBIndex = pointAIndex - numPointsPerLoop;

				AddStick(pointAIndex, pointBIndex);
			}
		}
	}

	// create the points at the end of each axis and connect them to the others
	int finalPointIndex = points.size();

	points.push_back(new Point(static_cast<float>(centerX), centerY + height));
	points.push_back(new Point(static_cast<float>(centerX), centerY - height));
	points.push_back(new Point(centerX + width, static_cast<float>(centerY)));
	points.push_back(new Point(centerX - width, static_cast<float>(centerY)));
	activePoints += numPointsPerLoop;

	AddStick(finalPointIndex, firstPointIndex);
	AddStick(finalPointIndex, firstPointIndex + 1);

	AddStick(finalPointIndex + 1, firstPointIndex + 2);
	AddStick(finalPointIndex + 1, firstPointIndex + 3);

	AddStick(finalPointIndex + 2, finalPointIndex - 4);
	AddStick(finalPointIndex + 2, finalPointIndex - 2);

	AddStick(finalPointIndex + 3, finalPointIndex - 3);
	AddStick(finalPointIndex + 3, finalPointIndex - 1);
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
