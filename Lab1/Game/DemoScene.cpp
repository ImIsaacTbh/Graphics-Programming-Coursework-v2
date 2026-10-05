#include "Game/DemoScene.h"
#include "Core/Paths.h"
#include "Scene/Scene.h"
#include <glm/gtc/constants.hpp>

void DemoScene::Load(Scene& scene, float aspectRatio)
{
	_texturedMonkeyId = scene.AddObject(Paths::Resource("monki.obj"), Paths::Resource("textureShader"), Paths::Resource("texture.png"),
		Transform(glm::vec3(2, 0, 0))).id;

	_colourMonkeyId = scene.AddObject(Paths::Resource("monki.obj"), Paths::Resource("funnyColourThing"), "",
		Transform(glm::vec3(-2, 0, 0))).id;

	_lightMarkerId = scene.AddObject(Paths::Resource("cube.obj"), Paths::Resource("funnyColourThing"), "",
		Transform(glm::vec3(0, 0, 0))).id;

	scene.AddObject(Paths::Resource("plane.obj"), Paths::Resource("textureShader"), Paths::Resource("bricks.jpg"),
		Transform(glm::vec3(0, 2, 0), glm::vec3(0.0, 0.0, glm::pi<float>()), glm::vec3(30, 1, 30)));

	scene.camera = Camera(glm::vec3(0, 0, -4), 5, aspectRatio, 0.1f, 10000);

	scene.lights = {
		{ glm::vec3(0.0f),				glm::vec3(1.0f, 1.0f, 1.0f) }, // orbiting, positioned in Update
		{ glm::vec3(30.0f, -5.0f, 30.0f), glm::vec3(1.0f, 0.8f, 0.6f) }
	};

	scene.shadowCasterPositions = {
		glm::vec3(0.0f),				// follows the orbiting light
		glm::vec3(0.0f, -5.0f, 0.0f),
		glm::vec3(30.0f, -5.0f, 0.0f)
	};
}

void DemoScene::Update(Scene& scene)
{
	glm::vec3 lightPos(glm::sin(_counter) * 10, -glm::abs(glm::sin(_counter * 5)), glm::cos(_counter) * 10);

	scene.lights[0].position = lightPos;
	scene.shadowCasterPositions[0] = lightPos;

	if (GameObject* monkey = scene.FindById(_texturedMonkeyId))
		monkey->transform.SetRot(glm::vec3(0.0, _counter * 2, 0.0));

	if (GameObject* monkey = scene.FindById(_colourMonkeyId))
		monkey->transform.SetRot(glm::vec3(0.0, -_counter * 2, 0.0));

	if (GameObject* marker = scene.FindById(_lightMarkerId))
		marker->transform = Transform(lightPos, lightPos, glm::vec3(0.5f));

	_counter += 0.01f;
}
