#pragma once
#include <SDL/SDL.h>
#include <glm/glm.hpp>
#include <functional>

// Polls SDL events once per frame and exposes keyboard / mouse state.
class Input
{
public:
	// Drains the SDL event queue. Every event is handed to onEvent first (e.g. so
	// the editor UI can see it) before Input records what it cares about.
	void PollEvents(const std::function<void(const SDL_Event&)>& onEvent);

	bool QuitRequested() const { return _quitRequested; }
	bool IsKeyDown(SDL_Scancode key) const;

	// Relative mouse motion accumulated during the last PollEvents, only while the mouse is captured.
	glm::vec2 GetMouseDelta() const { return _mouseDelta; }

	bool IsMouseCaptured() const;
	void SetMouseCaptured(bool captured);

private:
	bool _quitRequested = false;
	glm::vec2 _mouseDelta = glm::vec2(0.0f);
};
