/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:20 by celva             #+#    #+#             */
/*   Updated: 2020/03/11 23:04:10 by dkathlee         ###   ########.fr       */
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
	vku_swapchain_create(&(app->vulkan));
	vku_init_render(app);
	if (vku_create_buffer(&(app->vulkan)) == 0)
		write(1, "create buffer error\n", 22);
	vku_record_cmb(&(app->vulkan));
}

int		rtv_app_destroy(t_app *app)
{

}

void	rtv_app_run(t_app *app)
{
	int	run;
	int j;

	run = 1;
	j = 0;
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
		int i = 0;
		if (j == 0 || j == 1000)
			while (i < WIN_WIDTH * WIN_HEIGHT)
			{
				if (j == 0)
					((int*)(app->vulkan.buf.mem_ptr))[i] = 255;
				else if (j == 1000)
					((int*)(app->vulkan.buf.mem_ptr))[i] = 255 << 8;
				i++;
			}
        vku_draw_frame(&(app->vulkan));
		j++;
		if (j == 2000)
			j = 0;
    }
    SDL_Quit();
}
