/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:20 by celva             #+#    #+#             */
/*   Updated: 2020/05/26 14:08:55 by dkathlee         ###   ########.fr       */
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

void	init_struct(t_retr *r, char *fname)
{
	read_scene(fname, r);
	/*-r->n_fig = 6;
	r->n_lig = 3;
	r->figures = (t_object*)ft_memalloc(sizeof(t_object) * r->n_fig);
	r->lights = (t_light*)ft_memalloc(sizeof(t_light) * r->n_lig);*/
    r->vw = 1.5;
    r->vh = 1;
    r->d = 1;
	/*(r->figures)[0] = (t_object){
		.type = obj_sphere,
		.transform = {.position = (t_vec3){0, 0, 3.0}},
		.color = (t_vec3){255, 0, 0},
		.specular = 500,
		.figures = (t_figure){.radius = 1.0}
	};
	(r->figures)[1] = (t_object){
		.type = obj_sphere,
		.transform = {.position = (t_vec3){2.0, 0, 4.0}},
		.color = (t_vec3){0, 0, 255},
		.specular = 500,
		.figures = (t_figure){.radius = 1.0}
	};
	(r->figures)[2] = (t_object){
		.type = obj_sphere,
		.transform = {.position = (t_vec3){-2.0, 0, 4.0}},
		.color = (t_vec3){0, 255, 0},
		.specular = 10,
		.figures = (t_figure){.radius = 1.0}
	};
	(r->figures)[3] = (t_object){
		.type = obj_plane,
		.transform = {.position = (t_vec3){0.0, -1.0, 0.0}},
		.color = (t_vec3){0, 135, 0},
		.specular = 100,
		.figures = (t_figure){
			.direction = (t_vec3){0, 1.0, 0.0},
			.vertices[0] = (t_vec3){-2, -1, 4},
			.vertices[1] = (t_vec3){2, -1, 4},
			.vertices[2] = (t_vec3){2, -1, 2},
			.vertices[3] = (t_vec3){-2, -1, 2}}
	};
	(r->figures)[4] = (t_object){
		.type = obj_cylinder,
		.transform = {.position = (t_vec3){0.0, 0, 3.0}},
		.color = (t_vec3){255, 255, 0},
		.specular = 100,
		.figures = (t_figure){
			.direction = (t_vec3){0, 1.0, 0.0},
			.radius = 0.5}
	};
	(r->figures)[5] = (t_object){
		.type = obj_cone,
		.transform = {.position = (t_vec3){0.0, -1.0, 3.0}},
		.color = (t_vec3){35, 135, 75},
		.specular = 100,
		.figures = (t_figure){
			.direction = (t_vec3){0, 1.0, 0.0},
			.radius = 1.0,
			.height = 1,
			.ver = (t_vec3){0, 0, 3}}
	};*/
	/*void *o = ft_memalloc(sizeof(t_sphere));
	((t_sphere*)o)->radius = 1.0;
	(r->figures)[0] = (t_object){obj_sphere, (t_transform){.position = (t_vec3){0, 0, 3.0}}, (t_vec3){255, 0, 0}, 500, o};
	o = ft_memalloc(sizeof(t_sphere));
	((t_sphere*)o)->radius = 1.0;
	(r->figures)[1] = (t_object){obj_sphere, (t_transform){.position = (t_vec3){2.0, 0, 4.0}}, (t_vec3){0, 0, 255}, 500, o};
	o = ft_memalloc(sizeof(t_sphere));
	((t_sphere*)o)->radius = 1.0;
	(r->figures)[2] = (t_object){obj_sphere, (t_transform){.position = (t_vec3){-2.0, 0, 4.0}}, (t_vec3){0, 255, 0}, 10, o};
	o = ft_memalloc(sizeof(t_plane));
	((t_plane*)o)->normal = (t_vec3){0, 1.0, 0.0};
	((t_plane*)o)->vertices[0] = (t_vec3){-2, -1, 4};
	((t_plane*)o)->vertices[1] = (t_vec3){2, -1, 4};
	((t_plane*)o)->vertices[2] = (t_vec3){2, -1, 2};
	((t_plane*)o)->vertices[3] = (t_vec3){-2, -1, 2};
	(r->figures)[3] = (t_object){obj_plane, (t_transform){.position = (t_vec3){0.0, -1.0, 0.0}}, (t_vec3){0, 135, 0}, 100, o};
	o = ft_memalloc(sizeof(t_cylinder));
	((t_cylinder*)o)->direction = (t_vec3){0, 1, 0.0};
	((t_cylinder*)o)->radius = 0.5;
	(r->figures)[4] = (t_object){obj_cylinder, (t_transform){.position = (t_vec3){0, 0, 3}}, (t_vec3){255, 255, 0}, 100, o};
	o = ft_memalloc(sizeof(t_cone));
	((t_cone*)o)->direction = (t_vec3){0, 1.0, 0.0};
	((t_cone*)o)->radius = 1;
	((t_cone*)o)->height = 1;
	((t_cone*)o)->ver = (t_vec3){0, 0, 3};
	(r->figures)[5] = (t_object){obj_cone, (t_transform){.position = (t_vec3){0, -1, 3}}, (t_vec3){35, 135, 75}, 100, o};
	o = ft_memalloc(sizeof(t_plane));
	((t_plane*)o)->normal = (t_vec3){0, 0, -1.0};
	//(r->figures)[6] = (t_object){obj_plane, (t_transform){.position = (t_vec3){0.0, 0.0, 4.0}}, (t_vec3){35, 135, 75}, 100, o};
	o = ft_memalloc(sizeof(t_plane));
	((t_plane*)o)->normal = (t_vec3){0, -1.0, 0.0};*/
	//(r->figures)[7] = (t_object){obj_plane, (t_transform){.position = (t_vec3){0.0, 1.2, 0}}, (t_vec3){35, 135, 75}, 100, o};
	
	/*(r->lights)[0] = (t_light){light_ambient, 0.2, (t_vec3){0, 0, 0}, (t_vec3){0, 0, 0}};
	(r->lights)[1] = (t_light){light_point, 0.6, (t_vec3){0, 2, 1}, (t_vec3){0, 0, 0}};
	(r->lights)[2] = (t_light){light_directional, 0.2, (t_vec3){0, 0, 0}, (t_vec3){1, 4, 4}};*/
	//r->camera = (t_transform){(t_vec3){1, 0, 0}, (t_vec3){0, 0, 0}};
    //r->o = (t_vec3){0, 0, 0};
}

void	rtv_app_run(t_app *app, char *fname)
{
	int	run;
	int j;
	t_retr r;

	init_struct(&r, fname);
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
			else if (evt.type == SDL_KEYDOWN)
			{
				if (evt.key.keysym.sym == SDLK_UP)
				{
					r.camera.position = vec3d_add_vec3d(r.camera.position, (t_vec3){0, 1, 0});
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_DOWN)
				{
					r.camera.position = vec3d_add_vec3d(r.camera.position, (t_vec3){0, -1, 0});
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_LEFT)
				{
					r.camera.position = vec3d_add_vec3d(r.camera.position, (t_vec3){-1, 0, 0});
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_RIGHT)
				{
					r.camera.position = vec3d_add_vec3d(r.camera.position, (t_vec3){1, 0, 0});
					j = 0;
				}
			}
        }
		int i = 0;
		if (j == 0)
		{
			//printf("Draw cam pos: (%f, %f, %f)\n", r.o.x, r.o.y, r.o.z);
			raytracing(&r, app);
			if (vku_draw_frame(&(app->vulkan)) == 0)
				handle_error("Draw frame error");
		}
		j++;
    }
	rtv_app_destroy(&(app->vulkan));
    SDL_Quit();
}
