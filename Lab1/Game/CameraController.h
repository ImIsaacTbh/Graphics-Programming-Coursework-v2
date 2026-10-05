#pragma once

class Input;
struct Camera;

// Free-fly camera: mouse look + WASD, active only while the mouse is captured.
class CameraController
{
public:
	void Update(Camera& camera, const Input& input, float deltaTime);

private:
	float _lookSensitivity = 5.0f;
	float _moveSpeed = 10.0f;
};
