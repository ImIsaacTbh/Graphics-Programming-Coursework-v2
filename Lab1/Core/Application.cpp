#include "Core/Application.h"

Application::Application()
	: _window(1706, 960, "Game Window")
{
}

void Application::Run()
{
	Init();
	while (_running)
	{
		Frame();
	}
}

void Application::Init()
{
	_window.Init();
	_input.SetMouseCaptured(true);
	_editor.Init(_window);
	_renderer.Init();
	_demo.Load(_scene, _window.GetAspectRatio());
}

void Application::Frame()
{
	ProcessInput();

	_editor.BeginFrame(_scene, _timer.GetFramerate());
	_demo.Update(_scene);

	_renderer.Render(_scene, _window);
	_editor.Render();
	_window.SwapBuffers();

	_timer.Tick();
}

void Application::ProcessInput()
{
	_input.PollEvents([this](const SDL_Event& event) { _editor.ProcessEvent(event); });
	if (_input.QuitRequested())
		_running = false;

	UpdateMouseCapture();
	_cameraController.Update(_scene.camera, _input, (float)_timer.GetDeltaTime());
}

// Escape toggles between controlling the camera and using the editor UI.
void Application::UpdateMouseCapture()
{
	using namespace std::chrono_literals;

	if (_input.IsKeyDown(SDL_SCANCODE_ESCAPE) && _escCooldownEnd <= std::chrono::steady_clock::now())
	{
		_escCooldownEnd = std::chrono::steady_clock::now() + 250ms;
		_input.SetMouseCaptured(!_input.IsMouseCaptured());
	}
}
