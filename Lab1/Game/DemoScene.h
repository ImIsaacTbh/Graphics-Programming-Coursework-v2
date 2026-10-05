#pragma once

class Scene;

// The default scene content: two spinning monkeys, a floor, and a cube that
// marks an orbiting light.
class DemoScene
{
public:
	void Load(Scene& scene, float aspectRatio);

	// Advances the animation by one frame.
	void Update(Scene& scene);

private:
	float _counter = 0.0f;
	int _texturedMonkeyId = -1;
	int _colourMonkeyId = -1;
	int _lightMarkerId = -1;
};
