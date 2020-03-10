/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:20 by celva             #+#    #+#             */
/*   Updated: 2020/03/10 15:01:16 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		rtv_app_create(t_app *app)
{
	if (vku_instance_create(app) == 0)
		write(1, "inst_error\n", 11);
	vku_window_create(app);
	vku_get_physical_device(&(app->vulkan));
	vku_create_logical_device(&(app->vulkan));
	vku_swapchain_create(app);
	vku_init_render(app);
	vku_record_cmb(&(app->vulkan));
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

        vku_draw_frame(&(app->vulkan));
    }
    SDL_Quit();
}
