#pragma once
#include <SDL/SDL.h>
#include <GL/glew.h>
#include <string>

// Owns the SDL window and its OpenGL context.
class Window
{
public:
	Window(int width, int height, const std::string& title);
	~Window();

	Window(const Window&) = delete;
	Window& operator=(const Window&) = delete;

	void Init();
	void SwapBuffers();

	int GetWidth() const { return _width; }
	int GetHeight() const { return _height; }
	float GetAspectRatio() const { return (float)_width / (float)_height; }

	SDL_Window* GetSDLWindow() const { return _window; }
	SDL_GLContext GetGLContext() const { return _context; }

private:
	void Fail(const std::string& errorString);

	SDL_Window* _window;
	SDL_GLContext _context;
	std::string _title;
	int _width;
	int _height;
};
