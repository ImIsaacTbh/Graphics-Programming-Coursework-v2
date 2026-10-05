#include "Core/Window.h"
#include <iostream>
#include <cstdlib>

Window::Window(int width, int height, const std::string& title)
	: _window(nullptr), _context(nullptr), _title(title), _width(width), _height(height)
{
}

Window::~Window()
{
	if (_context != nullptr) SDL_GL_DeleteContext(_context);
	if (_window != nullptr) SDL_DestroyWindow(_window);
	SDL_Quit();
}

void Window::Fail(const std::string& errorString)
{
	std::cout << (errorString + "\n" + "Press Any Key To Quit...");
	exit(0);
}

void Window::SwapBuffers()
{
	SDL_GL_SwapWindow(_window);
}

void Window::Init()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer

	_window = SDL_CreateWindow(_title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _width, _height, SDL_WINDOW_OPENGL);
	if (_window == nullptr) Fail("Failed to create window.");

	_context = SDL_GL_CreateContext(_window);
	if (_context == nullptr) Fail("Failed to create openGL context.");

	if (glewInit() != GLEW_OK) Fail("Failed to initalise GLEW.");

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	glClearColor(0.13f, 0.6f, 0.71f, 0.0f);

	std::cout << glGetString(GL_VERSION) << std::endl;
}
