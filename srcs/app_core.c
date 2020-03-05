#include "rtv1.h"

int		rtv_app_create(t_app *app)
{
	vkl_instance_create(app);
}

int		rtv_app_destroy(t_app *app)
{

}

void	rtv_app_run(t_app *app)
{
	int	run;

	run = 1;
	while (run)
    {
        SDL_Event evt;
        while (SDL_PollEvent(&evt))
        {
            if (evt.type == SDL_QUIT)
            {
                run = 0;
            }
        }

        draw_frame();
    }
    SDL_Quit();
}
