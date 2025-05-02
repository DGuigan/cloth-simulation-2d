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

	if (applicationMode == ApplicationMode::Simulate)
	{
		// build a default cloth if immediately simulating
		cloth = new Cloth(numColumns, numRows, clothSpacing, startX, startY);
	}
	else if (applicationMode == ApplicationMode::Design)
	{
		// empty cloth to draw on
		cloth = new Cloth();
	}

	lastUpdateTime = SDL_GetTicks();
}

void Application::Reset()
{
	applicationMode = ApplicationMode::Design;
	cloth->Reset();
}


void Application::Input()
{
	SDL_Event event;

	inputHandler->OnFrameStart();

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
			case (SDLK_DELETE):
			{
				Reset();
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
			case (SDLK_RETURN):
			{
				if (applicationMode != ApplicationMode::Simulate)
				{
					applicationMode = ApplicationMode::Simulate;
				}
				else
				{
					applicationMode = ApplicationMode::Design;
				}
				break;
			}
			case (SDLK_LCTRL):
			{
				inputHandler->SetLeftCtrlDown(true);
				break;
			}
			case (SDLK_LSHIFT):
			{
				inputHandler->SetLeftShiftDown(true);
				break;
			}
			case (SDLK_RIGHT):
			{
				inputHandler->SetArrowKeyState(0, 1.f);
				break;
			}
			case (SDLK_LEFT):
			{
				inputHandler->SetArrowKeyState(0, -1.f);
				break;
			}
			case(SDLK_UP):
			{
				inputHandler->SetArrowKeyState(1, 1.f);
				break;
			}
			case(SDLK_DOWN):
			{
				inputHandler->SetArrowKeyState(1, -1.f);
				break;
			}
			case(SDLK_f):
			{
				inputHandler->SetFDown(true);
				break;
			}
			}
			break;
		}
		case SDL_KEYUP:
		{
			switch (event.key.keysym.sym)
			{
			case (SDLK_LCTRL):
			{
				inputHandler->SetLeftCtrlDown(false);
				break;
			}
			case (SDLK_LSHIFT):
			{
				inputHandler->SetLeftShiftDown(false);
				break;
			}
			case (SDLK_RIGHT):
			{
				inputHandler->SetArrowKeyState(0, 0.f);
				break;
			}
			case (SDLK_LEFT):
			{
				inputHandler->SetArrowKeyState(0, 0.f);
				break;
			}
			case(SDLK_UP):
			{
				inputHandler->SetArrowKeyState(1, 0.f);
				break;
			}
			case(SDLK_DOWN):
			{
				inputHandler->SetArrowKeyState(1, 0.f);
				break;
			}
			case(SDLK_f):
			{
				inputHandler->SetFDown(false);
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

	cloth->Update(applicationMode, inputHandler, renderer, deltaTime);

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
