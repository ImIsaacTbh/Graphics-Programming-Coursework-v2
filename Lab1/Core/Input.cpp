#include "Core/Input.h"

void Input::PollEvents(const std::function<void(const SDL_Event&)>& onEvent)
{
	_mouseDelta = glm::vec2(0.0f);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (onEvent) onEvent(event);

		switch (event.type)
		{
		case SDL_QUIT:
			_quitRequested = true;
			break;
		case SDL_MOUSEMOTION:
			if (!IsMouseCaptured()) break;
			_mouseDelta += glm::vec2((float)event.motion.xrel, (float)event.motion.yrel);
			break;
		}
	}
}

bool Input::IsKeyDown(SDL_Scancode key) const
{
	return SDL_GetKeyboardState(NULL)[key] != 0;
}

bool Input::IsMouseCaptured() const
{
	return SDL_GetRelativeMouseMode() == SDL_TRUE;
}

void Input::SetMouseCaptured(bool captured)
{
	SDL_SetRelativeMouseMode(captured ? SDL_TRUE : SDL_FALSE);
}
