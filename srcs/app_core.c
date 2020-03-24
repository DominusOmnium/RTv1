/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:20 by celva             #+#    #+#             */
/*   Updated: 2020/03/24 12:39:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		rtv_app_create(t_app *app)
{
	if (vku_instance_create(app) == 0)
		handle_error("Instance creation error\n");
	if (vku_window_create(app) == 0)
		handle_error("Window creation error\n");
	printf("1\n");
	if (vku_get_physical_device(&(app->vulkan)) == 0)
		handle_error("Get physical device error\n");
	printf("2\n");
	if (vku_create_logical_device(&(app->vulkan)) == 0)
		handle_error("Logical device creation error\n");
	printf("3\n");
	if (vku_swapchain_create(&(app->vulkan)) == 0)
		handle_error("Swapchain creation error\n");
	printf("1\n");
	if (vku_init_render(app) == 0)
		handle_error("Render initialisation error\n");
	printf("1\n");
	if (vku_create_buffer(&(app->vulkan)) == 0)
		handle_error("Buffer creation error\n");
	printf("1\n");
	if (vku_record_cmb(&(app->vulkan)) == 0)
		handle_error("Cmb record error\n");
	printf("app create done\n");
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

void	init_struct(t_retr *r)
{
	r->n_fig = 4;
	r->n_lig = 3;
	r->figures = (t_sphere*)malloc(sizeof(t_sphere) * r->n_fig);
	r->lights = (t_light*)malloc(sizeof(t_light) * r->n_lig);
    r->vw = 1.5;
    r->vh = 1;
    r->d = 1;
    (r->figures)[0] = (t_sphere){(t_vec3){0, -1.0, 3.0}, 1.0, (t_vec3){255, 0, 0}, 500};
    (r->figures)[1] = (t_sphere){(t_vec3){2.0, 0, 4.0}, 1.0, (t_vec3){0, 0, 255}, 500};
    (r->figures)[2] = (t_sphere){(t_vec3){-2.0, 0, 4.0}, 1.0, (t_vec3){0, 255, 0}, 10};
	(r->figures)[3] = (t_sphere){(t_vec3){0, -5001.0, 0}, 5000, (t_vec3){255, 255, 0}, 1000};
	(r->lights)[0] = (t_light){'a', 0.2, (t_vec3){0, 0, 0}, (t_vec3){0, 0, 0}};
	(r->lights)[1] = (t_light){'p', 0.6, (t_vec3){2, 1, 0}, (t_vec3){0, 0, 0}};
	(r->lights)[2] = (t_light){'d', 0.2, (t_vec3){0, 0, 0}, (t_vec3){1, 4, 4}};
	r->camera = (t_transform){(t_vec3){0, 0, 0}, (t_vec3){0, 0, 0}};
    r->o = (t_vec3){0, 0, 0};
}

void	rtv_app_run(t_app *app)
{
	int	run;
	int j;
	t_retr r;

	init_struct(&r);
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
		if (j == 0)
			raytracing(&r, app);
		j++;
		/*if (j == 0 || j == 1000)
			while (i < WIN_WIDTH * WIN_HEIGHT)
			{
				if (j == 0)
					((int*)(app->vulkan.buf.mem_ptr))[i] = 255;
				else if (j == 1000)
					((int*)(app->vulkan.buf.mem_ptr))[i] = 255 << 8;
				i++;
			}*/
        if (vku_draw_frame(&(app->vulkan)) == 0)
			handle_error("Draw frame error");
		/*j++;
		if (j == 2000)
			j = 0;*/
    }
	rtv_app_destroy(&(app->vulkan));
    SDL_Quit();
}
