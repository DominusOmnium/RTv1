/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:20 by celva             #+#    #+#             */
/*   Updated: 2020/08/24 10:39:37 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	rtv_app_create(t_app *app)
{
	vku_instance_create(app);
	ft_printf("Instance creation done\n");
	vku_window_create(app);
	ft_printf("Window creation done\n");
	vku_get_physical_device(&(app->vulkan));
	ft_printf("Get physical device done: %s\n",
						app->vulkan.phys_device.dev_prop.deviceName);
	vku_create_logical_device(&(app->vulkan));
	ft_printf("Logical device creation done\n");
	vku_swapchain_create(&(app->vulkan), &(app->r));
	ft_printf("Swapchain creation done\n");
	vku_init_render(&(app->vulkan));
	ft_printf("Render initialization done\n");
}

void	rtv_app_destroy(t_vulkan *v)
{
	vkWaitForFences(v->device, 3, &(v->sync.frame_fences[0]), VK_TRUE, UINT64_MAX);
	vkDestroyFramebuffer(v->device, v->framebuffer.frame_buffers[0], NULL);
	vkDestroyFramebuffer(v->device, v->framebuffer.frame_buffers[1], NULL);
	vkDestroyFramebuffer(v->device, v->framebuffer.frame_buffers[2], NULL);
	vkFreeCommandBuffers(v->device, v->commandpool, FRAME_COUNT, v->command_buffers);
	vkDestroyPipeline(v->device, v->pipeline, NULL);
	vkDestroyPipelineLayout(v->device, v->pipelineLayout, NULL);
	vkDestroyRenderPass(v->device, v->renderpass, NULL);
	vkDestroySwapchainKHR(v->device, v->swapchain, NULL);
	vkDestroyBuffer(v->device, v->sbo_buffers[0].buffer, NULL);
	vkFreeMemory(v->device, v->sbo_buffers[0].dev_mem, NULL);
	vkDestroyBuffer(v->device, v->sbo_buffers[1].buffer, NULL);
	vkFreeMemory(v->device, v->sbo_buffers[1].dev_mem, NULL);
	vkDestroyBuffer(v->device, v->sbo_buffers[2].buffer, NULL);
	vkFreeMemory(v->device, v->sbo_buffers[2].dev_mem, NULL);
	vkDestroyDescriptorPool(v->device, v->descriptor.pool, NULL);
	vkDestroyImageView(v->device, v->framebuffer.sc_image_views[0], NULL);
	vkDestroyImageView(v->device, v->framebuffer.sc_image_views[1], NULL);
	vkDestroyImageView(v->device, v->framebuffer.sc_image_views[2], NULL);
	vkDestroyDescriptorSetLayout(v->device, v->descriptor.set_layout, NULL);
	vkDestroySemaphore(v->device, v->sync.image_available_sem[0], NULL);
	vkDestroySemaphore(v->device, v->sync.image_available_sem[1], NULL);
	vkDestroySemaphore(v->device, v->sync.image_available_sem[2], NULL);
	vkDestroySemaphore(v->device, v->sync.render_finished_sem[0], NULL);
	vkDestroySemaphore(v->device, v->sync.render_finished_sem[1], NULL);
	vkDestroySemaphore(v->device, v->sync.render_finished_sem[2], NULL);
	vkDestroyFence(v->device, v->sync.frame_fences[0], NULL);
	vkDestroyFence(v->device, v->sync.frame_fences[1], NULL);
	vkDestroyFence(v->device, v->sync.frame_fences[2], NULL);
	vkDestroyCommandPool(v->device, v->commandpool, NULL);
	vkDestroyDevice(v->device, NULL);
	vkDestroySurfaceKHR(v->inst, v->surface, NULL);
	vkDestroyInstance(v->inst, NULL);
}

t_vec4	rotation_axis1(double angle, t_vec4 axis, t_vec4 p)
{
	t_vec4 res;
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	res.x = p.x * (c + axis.x * axis.x * (1 - c)) + p.y * (axis.y * axis.x * (1 - c) + axis.z * s) + p.z * (axis.z * axis.x * (1 - c) - axis.y * s);
	res.y = p.x * (axis.x * axis.y * (1 - c) - axis.z * s) + p.y * (c + axis.y * axis.y * (1 - c)) + p.z * (axis.z * axis.y * (1 - c) + axis.x * s);
	res.z = p.x * (axis.x * axis.z * (1 - c) + axis.y * s) + p.y * (axis.y * axis.z* (1 - c) - axis.x * s) + p.z * (c + axis.z * axis.z * (1 - c));
	return (res);
}

void				p3d_rotate_x(t_vec4 *t, float angle)
{
	float		prev_y;
	float		prev_z;

	prev_y = t->y;
	prev_z = t->z;
	t->y = (prev_y * cos(RAD(angle)) - prev_z * sin(RAD(angle)));
	t->z = (prev_y * sin(RAD(angle)) + prev_z * cos(RAD(angle)));
}

void				p3d_rotate_y(t_vec4 *t, float angle)
{
	float		prev_x;
	float		prev_z;

	prev_x = t->x;
	prev_z = t->z;
	t->x = (prev_x * cos(RAD(angle)) + prev_z * sin(RAD(angle)));
	t->z = (prev_z * cos(RAD(angle)) - prev_x * sin(RAD(angle)));
}

void				p3d_rotate_z(t_vec4 *t, float angle)
{
	float		prev_x;
	float		prev_y;

	prev_x = t->x;
	prev_y = t->y;
	t->x = (prev_x * cos(RAD(angle)) - prev_y * sin(RAD(angle)));
	t->y = (prev_x * sin(RAD(angle)) + prev_y * cos(RAD(angle)));
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
			else if (evt.type == SDL_KEYDOWN)
			{
				if (evt.key.keysym.sym == SDLK_UP)
				{
					app->r.camera.transform.position = vec4_add_vec4(app->r.camera.transform.position, vec4_mul_f(app->r.camera.direction, vec4_mod(app->r.camera.direction)));
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_DOWN)
				{
					app->r.camera.transform.position = vec4_sub_vec4(app->r.camera.transform.position, vec4_mul_f(app->r.camera.direction, vec4_mod(app->r.camera.direction)));
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_LEFT)
				{
					app->r.camera.transform.position = vec4_add_vec4(app->r.camera.transform.position, (t_vec4){-0.1, 0, 0, 0});
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_RIGHT)
				{
					app->r.camera.transform.position = vec4_add_vec4(app->r.camera.transform.position, (t_vec4){0.1, 0, 0, 0});
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_q)
				{
					p3d_rotate_y(&app->r.camera.direction, -10);
					app->r.camera.transform.rotation.y -= RAD(10);
					//app->r.camera.transform.rotation.x = -atanf(app->r.camera.direction.y / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.y = -atanf(app->r.camera.direction.x / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.z = -atanf(app->r.camera.direction.y / app->r.camera.direction.x);
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_e)
				{
					p3d_rotate_y(&app->r.camera.direction, 10);
					app->r.camera.transform.rotation.y += RAD(10);
					//app->r.camera.transform.rotation.x = -atanf(app->r.camera.direction.y / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.y = -atanf(app->r.camera.direction.x / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.z = -atanf(app->r.camera.direction.y / app->r.camera.direction.x);
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_KP_8)
				{
					p3d_rotate_x(&app->r.camera.direction, 10);
					app->r.camera.transform.rotation.x += RAD(10);
					//app->r.camera.transform.rotation.x = -atanf(app->r.camera.direction.y / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.y = -atanf(app->r.camera.direction.x / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.z = -atanf(app->r.camera.direction.y / app->r.camera.direction.x);
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_KP_2)
				{
					p3d_rotate_x(&app->r.camera.direction, -10);
					app->r.camera.transform.rotation.x -= RAD(10);
					//app->r.camera.transform.rotation.x = -atanf(app->r.camera.direction.y / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.y = -atanf(app->r.camera.direction.x / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.z = -atanf(app->r.camera.direction.y / app->r.camera.direction.x);
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_KP_4)
				{
					p3d_rotate_z(&app->r.camera.direction, -10);
					app->r.camera.transform.rotation.z -= RAD(10);
					//app->r.camera.transform.rotation.x = -atanf(app->r.camera.direction.y / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.y = -atanf(app->r.camera.direction.x / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.z = -atanf(app->r.camera.direction.y / app->r.camera.direction.x);
					j = 0;
				}
				if (evt.key.keysym.sym == SDLK_KP_6)
				{
					p3d_rotate_z(&app->r.camera.direction, 10);
					app->r.camera.transform.rotation.z += RAD(10);
					//app->r.camera.transform.rotation.x = -atanf(app->r.camera.direction.y / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.y = -atanf(app->r.camera.direction.x / app->r.camera.direction.z);
					//app->r.camera.transform.rotation.z = -atanf(app->r.camera.direction.y / app->r.camera.direction.x);
					j = 0;
				}
			}
        }
		if (j == 0)
		{
			draw_frame(&(app->vulkan), &(app->r));
			j = 1;
		}
    }
	rtv_app_destroy(&(app->vulkan));
    SDL_Quit();
}
