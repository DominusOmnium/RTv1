#include <CoreGraphics/CGWindow.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

int main(int argc, char *argv[])
{
	SDL_version v;
    SDL_GetVersion(&v);
	printf("%d.%d.%d\n", v.major, v.minor, v.patch);
    return 0;
}
