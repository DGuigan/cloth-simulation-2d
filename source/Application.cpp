#include <iostream>
#include "Application.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "Cloth.h"

void Application::Setup(int clothWidth, int clothHeight, int clothSpacing)
{
	renderer = new Renderer();
	inputHandler = new InputHandler();

	isRunning = renderer->Setup();

	int numColumns = clothWidth / clothSpacing;
	int numRows = clothHeight / clothSpacing;
	int startX = static_cast<int>((renderer->GetWindowWidth() - clothWidth) * 0.5f);
	int startY = static_cast<int>(renderer->GetWindowHeight() * 0.1f);

	cloth = new Cloth(numColumns, numRows, clothSpacing, startX, startY);

	lastUpdateTime = SDL_GetTicks();
}

void Application::Input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
		{
			isRunning = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			switch (event.key.keysym.sym)
			{
			case (SDLK_ESCAPE):
			{
				isRunning = false;
				break;
			}
			case (SDLK_1):
			{
				drawPoints = !drawPoints;
				break;
			}
			case (SDLK_2):
			{
				drawSticks = !drawSticks;
				break;
			}
			}
			break;
		}
		case SDL_MOUSEMOTION:
		{
			int x = event.motion.x;
			int y = event.motion.y;
			inputHandler->UpdateMousePosition(x, y);
			break;
		}
		case SDL_MOUSEBUTTONDOWN:
		{
			int x, y;
			SDL_GetMouseState(&x, &y);
			inputHandler->UpdateMousePosition(x, y);

			if (!inputHandler->GetLeftMouseButtonDown() && event.button.button == SDL_BUTTON_LEFT)
			{
				inputHandler->SetLeftMouseButtonDown(true);
			}
			if (!inputHandler->GetRightMouseButtonDown() && event.button.button == SDL_BUTTON_RIGHT)
			{
				inputHandler->SetRightMouseButtonDown(true);
			}
			break;
		}
		case SDL_MOUSEBUTTONUP:
		{
			if (inputHandler->GetLeftMouseButtonDown() && event.button.button == SDL_BUTTON_LEFT)
			{
				inputHandler->SetLeftMouseButtonDown(false);
			}
			if (inputHandler->GetRightMouseButtonDown() && event.button.button == SDL_BUTTON_RIGHT)
			{
				inputHandler->SetRightMouseButtonDown(false);
			}
			break;
		}
		case SDL_MOUSEWHEEL:
		{
			if (event.wheel.y > 0)
			{
				inputHandler->IncreaseCursorSize(10);
			}
			else if (event.wheel.y < 0)
			{
				inputHandler->IncreaseCursorSize(-10);
			}
			break;
		}
		}
	}
}

void Application::Update()
{
	Uint32 currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastUpdateTime) / 1000.0f;

	cloth->Update(renderer, inputHandler, deltaTime);

	lastUpdateTime = currentTime;
}

void Application::Render() const
{
	renderer->ClearScreen(0xFF000816);

	cloth->Draw(renderer, drawPoints, drawSticks);

	renderer->Render();
}


bool Application::IsRunning() const
{
	return isRunning;
}

void Application::Destroy()
{
	delete inputHandler;
	delete renderer;
	delete cloth;
}
