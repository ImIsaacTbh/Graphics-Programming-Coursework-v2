#include <SDL/SDL.h> // must be included here so SDL2main can wrap main()
#include "Core/Application.h"

int main(int argc, char** argv) //argument used to call SDL main
{
	Application app;
	app.Run();

	return 0;
}
