#include "Editor/EditorUI.h"
#include "Core/FileSystem.h"
#include "Core/Paths.h"
#include "Core/Window.h"
#include "Scene/Scene.h"

#include <glm/gtc/type_ptr.hpp>
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/imgui_impl_sdl2.h"
#include "ThirdParty/imgui/imgui_impl_opengl3.h"

EditorUI::~EditorUI()
{
	if (!_initialized) return;
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void EditorUI::Init(const Window& window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window.GetSDLWindow(), window.GetGLContext());
	ImGui_ImplOpenGL3_Init();
	_initialized = true;
}

void EditorUI::ProcessEvent(const SDL_Event& event)
{
	ImGui_ImplSDL2_ProcessEvent(&event);
}

void EditorUI::BeginFrame(Scene& scene, float framerate)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	DrawHierarchy(scene);
	DrawStats(framerate);
	DrawInspector(scene);
}

void EditorUI::Render()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUI::DrawHierarchy(Scene& scene)
{
	const std::string textureShader = Paths::Resource("textureShader");

	ImGui::SetNextWindowSize(ImVec2(300, 400), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(10, 10), ImGuiCond_Once);
	ImGui::Begin("Scene##hierarchy");

	ImGui::Text("Create Primitive");
	if (ImGui::Button("Cube"))			scene.AddPrimitive(Primitive::Cube, textureShader, Paths::Resource("bricks.jpg"));
	ImGui::SameLine();
	if (ImGui::Button("Sphere"))		scene.AddPrimitive(Primitive::IcoSphere, textureShader, Paths::Resource("bricks.jpg"));
	ImGui::SameLine();
	if (ImGui::Button("Plane"))			scene.AddPrimitive(Primitive::Plane, textureShader, Paths::Resource("bricks.jpg"));
	ImGui::SameLine();
	if (ImGui::Button("Monki"))			scene.AddPrimitive(Primitive::Monki, textureShader, Paths::Resource("texture.png"));

	if (ImGui::Button("AssadSphere"))	scene.AddPrimitive(Primitive::UVSphere, textureShader, Paths::Resource("assad.png"), glm::vec3(2));
	ImGui::SameLine();
	if (ImGui::Button("Lucio"))			scene.AddPrimitive(Primitive::Lucio, textureShader, Paths::Resource("BakedShiz.png"));

	std::vector<GameObject>& objects = scene.GetObjects();

	ImGui::Separator();
	ImGui::Text("Scene Objects (%zu)", objects.size());
	ImGui::BeginChild("ObjectList", ImVec2(0, 0), true);
	for (int i = 0; i < (int)objects.size(); i++)
	{
		// PushID so objects that share a name stay individually selectable
		ImGui::PushID(objects[i].id);
		if (ImGui::Selectable(objects[i].name, _selectedObject == i))
			_selectedObject = i;
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::End();
}

void EditorUI::DrawStats(float framerate)
{
	ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(320, 10), ImGuiCond_Once);
	ImGui::Begin("Stats");

	ImGui::Text("Framerate: %f", framerate);

	ImGui::End();
}

void EditorUI::DrawInspector(Scene& scene)
{
	ImGui::SetNextWindowSize(ImVec2(300, 475), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(10, 420), ImGuiCond_Once);
	ImGui::Begin("Inspector");

	std::vector<GameObject>& objects = scene.GetObjects();
	if (_selectedObject < 0 || _selectedObject >= (int)objects.size())
	{
		ImGui::TextDisabled("No object selected");
		ImGui::End();
		return;
	}

	GameObject& obj = objects[_selectedObject];

	ImGui::InputText("Name", obj.name, sizeof(obj.name));

	ImGui::Separator();
	DrawTransform(obj);

	ImGui::Separator();
	DrawMaterial(obj);

	ImGui::Separator();
	ImGui::Spacing();
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.1f, 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.1f, 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.1f, 0.1f, 1.0f));
	if (ImGui::Button("Delete Object", ImVec2(-1, 0)))
	{
		scene.RemoveObject(_selectedObject);
		_selectedObject = -1;
	}
	ImGui::PopStyleColor(3);

	ImGui::End();
}

void EditorUI::DrawTransform(GameObject& obj)
{
	if (!ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) return;

	Transform& transform = obj.transform;
	glm::vec3 pos = transform.GetPos();
	glm::vec3 scl = transform.GetScale();
	glm::vec3 rotDeg = glm::degrees(transform.GetRot());

	if (ImGui::DragFloat3("Position", glm::value_ptr(pos), 0.1f))
		transform.SetPos(pos);

	if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotDeg), 1.0f))
		transform.SetRot(glm::radians(rotDeg));

	glm::vec3 prevScl = transform.GetScale();
	if (ImGui::DragFloat3("Scale", glm::value_ptr(scl), 0.1f))
	{
		if (_lockScale)
		{
			// whichever axis was dragged drives all three
			glm::vec3 delta = scl - prevScl;
			float driven = (delta.x != 0.0f) ? scl.x : (delta.y != 0.0f) ? scl.y : scl.z;
			scl = glm::vec3(driven);
		}
		transform.SetScale(scl);
	}

	if (_lockScale)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.2f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.65f, 0.3f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.4f, 0.1f, 1.0f));
		if (ImGui::Button("Scale Locked")) _lockScale = false;
		ImGui::PopStyleColor(3);
	}
	else
	{
		if (ImGui::Button("Lock Scale Together")) _lockScale = true;
	}
}

void EditorUI::RefreshResourceLists()
{
	_shaderPaths = FileSystem::FindShaderPrograms(Paths::ResourceDir);
	_imagePaths = FileSystem::FindImages(Paths::ResourceDir);
	_resourcesCached = true;
}

void EditorUI::DrawMaterial(GameObject& obj)
{
	if (!ImGui::CollapsingHeader("Graficks", ImGuiTreeNodeFlags_DefaultOpen)) return;

	// cache the lists, only refresh when requested
	if (!_resourcesCached) RefreshResourceLists();

	Material& material = obj.material;

	// shady guy
	ImGui::Text("Shader");
	ImGui::BeginChild("ShaderEdits", ImVec2(0, 75.0f), true);
	if (material.shader != nullptr)
		ImGui::TextWrapped("Current: %s", material.shader->GetPath().c_str());
	else
		ImGui::TextDisabled("No shader assigned (oh shid)");

	{
		std::vector<const char*> shaderItems;
		for (const std::string& s : _shaderPaths) shaderItems.push_back(s.c_str());

		if (ImGui::Combo("##shaderdrop", &_selectedShader, shaderItems.data(), (int)shaderItems.size()))
			material.shader = std::make_unique<Shader>(_shaderPaths[_selectedShader]);
	}
	ImGui::SameLine();
	if (ImGui::Button("R##shaderrefresh")) _resourcesCached = false;
	ImGui::EndChild();

	// textur
	ImGui::BeginChild("TextureEdits", ImVec2(0, 100.0f), true);
	ImGui::Text("Texture");
	if (material.diffuse != nullptr)
		ImGui::TextWrapped("Current: %s", material.diffuse->GetPath().c_str());
	else
		ImGui::TextDisabled("No texture assigned");

	{
		std::vector<const char*> texItems;
		for (const std::string& t : _imagePaths) texItems.push_back(t.c_str());

		if (ImGui::Combo("##texdrop", &_selectedTexture, texItems.data(), (int)texItems.size()))
			material.diffuse = std::make_unique<Texture>(_imagePaths[_selectedTexture]);
	}
	ImGui::SameLine();
	if (ImGui::Button("Clear##tex"))
	{
		material.diffuse.reset();
		_selectedTexture = -1;
	}
	ImGui::SameLine();
	if (ImGui::Button("R##texrefresh")) _resourcesCached = false;
	ImGui::EndChild();
}
