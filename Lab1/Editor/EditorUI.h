#pragma once
#include <string>
#include <vector>
#include <SDL/SDL.h>

class Window;
class Scene;
struct GameObject;

// Dear ImGui tooling: scene hierarchy, stats and object inspector.
class EditorUI
{
public:
	EditorUI() = default;
	~EditorUI();

	EditorUI(const EditorUI&) = delete;
	EditorUI& operator=(const EditorUI&) = delete;

	// Requires an initialised window / GL context.
	void Init(const Window& window);

	void ProcessEvent(const SDL_Event& event);

	// Starts the ImGui frame and builds all editor windows.
	void BeginFrame(Scene& scene, float framerate);

	// Draws the built UI on top of whatever is currently in the back buffer.
	void Render();

private:
	void DrawHierarchy(Scene& scene);
	void DrawStats(float framerate);
	void DrawInspector(Scene& scene);
	void DrawTransform(GameObject& obj);
	void DrawMaterial(GameObject& obj);

	void RefreshResourceLists();

	bool _initialized = false;

	int _selectedObject = -1;
	bool _lockScale = false;

	int _selectedShader = -1;
	int _selectedTexture = -1;
	bool _resourcesCached = false;
	std::vector<std::string> _shaderPaths;
	std::vector<std::string> _imagePaths;
};
