#include "Game/CameraController.h"
#include "Core/Input.h"
#include "Scene/Camera.h"

void CameraController::Update(Camera& camera, const Input& input, float deltaTime)
{
	if (!input.IsMouseCaptured()) return;

	glm::vec2 mouseDelta = input.GetMouseDelta();
	if (mouseDelta.y != 0.0f) camera.Pitch(mouseDelta.y * -0.05f * _lookSensitivity * deltaTime);
	if (mouseDelta.x != 0.0f) camera.RotateY(mouseDelta.x * 0.05f * _lookSensitivity * deltaTime);

	float step = _moveSpeed * deltaTime;
	if (input.IsKeyDown(SDL_SCANCODE_W)) camera.MoveForward(step);
	if (input.IsKeyDown(SDL_SCANCODE_S)) camera.MoveForward(-step);
	if (input.IsKeyDown(SDL_SCANCODE_A)) camera.MoveRight(-step);
	if (input.IsKeyDown(SDL_SCANCODE_D)) camera.MoveRight(step);
}
