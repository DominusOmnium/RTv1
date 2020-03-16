/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:20 by celva             #+#    #+#             */
/*   Updated: 2020/03/14 17:10:13 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		rtv_app_create(t_app *app)
{
	if (vku_instance_create(app) == 0)
		handle_error("Instance creation error\n");
	if (vku_window_create(app) == 0)
		handle_error("Window creation error\n");
	if (vku_get_physical_device(&(app->vulkan)) == 0)
		handle_error("Get physical device error\n");
	if (vku_create_logical_device(&(app->vulkan)) == 0)
		handle_error("Logical device creation error\n");
	if (vku_swapchain_create(&(app->vulkan)) == 0)
		handle_error("Swapchain creation error\n");
	if (vku_init_render(app) == 0)
		handle_error("Render initialisation error\n");
	if (vku_create_buffer(&(app->vulkan)) == 0)
		handle_error("Buffer creation error\n");
	if (vku_record_cmb(&(app->vulkan)) == 0)
		handle_error("Cmb record error\n");
}

void	rtv_app_destroy(t_vulkan *v)
{
	vkUnmapMemory(v->device, v->buf.dev_mem);
	vkFreeMemory(v->device, v->buf.dev_mem, NULL);
	vkDestroyBuffer(v->device, v->buf.buffer, NULL);
	vkDestroySwapchainKHR(v->device, v->swapchain, NULL);
	vkDestroyDevice(v->device, NULL);
	vkDestroyInstance(v->inst, NULL);
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
        if (vku_draw_frame(&(app->vulkan)) == 0)
			handle_error("Draw frame error");
		j++;
		if (j == 2000)
			j = 0;
    }
	rtv_app_destroy(&(app->vulkan));
    SDL_Quit();
}
