#pragma once
#include <chrono>
#include "Core/Window.h"
#include "Core/Input.h"
#include "Core/FrameTimer.h"
#include "Renderer/Renderer.h"
#include "Scene/Scene.h"
#include "Game/DemoScene.h"
#include "Game/CameraController.h"
#include "Editor/EditorUI.h"

// Owns every engine system and runs the main loop.
class Application
{
public:
	Application();

	void Run();

private:
	void Init();
	void Frame();
	void ProcessInput();
	void UpdateMouseCapture();

	// Declaration order is destruction order in reverse: the window (and its GL
	// context) must outlive everything that owns GL resources.
	Window _window;
	Input _input;
	FrameTimer _timer;
	Renderer _renderer;
	Scene _scene;
	DemoScene _demo;
	CameraController _cameraController;
	EditorUI _editor;

	bool _running = true;
	std::chrono::steady_clock::time_point _escCooldownEnd;
};
