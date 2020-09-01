/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   app_core.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celva <celva@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:20 by celva             #+#    #+#             */
/*   Updated: 2020/09/01 19:36:17 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void				rtv_app_create(t_app *app)
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

void				rtv_app_destroy(t_vulkan *v)
{
	vkWaitForFences(v->device, 3, &(v->sync.frame_fences[0]),
								VK_TRUE, UINT64_MAX);
	vkDestroyFramebuffer(v->device, v->framebuffer.frame_buffers[0], NULL);
	vkDestroyFramebuffer(v->device, v->framebuffer.frame_buffers[1], NULL);
	vkDestroyFramebuffer(v->device, v->framebuffer.frame_buffers[2], NULL);
	vkFreeCommandBuffers(v->device, v->commandpool, FRAME_COUNT,
								v->command_buffers);
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
	vku_destroy_sync_objects(v);
	vkDestroyCommandPool(v->device, v->commandpool, NULL);
	vkDestroyDevice(v->device, NULL);
	vkDestroySurfaceKHR(v->inst, v->surface, NULL);
	vkDestroyInstance(v->inst, NULL);
}

t_vec4				rotation_axis1(double angle, t_vec4 axis, t_vec4 p)
{
	t_vec4 res;
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	res.x = p.x * (c + axis.x * axis.x * (1 - c)) + p.y * (axis.y * axis.x *
		(1 - c) + axis.z * s) + p.z * (axis.z * axis.x * (1 - c) - axis.y * s);
	res.y = p.x * (axis.x * axis.y * (1 - c) - axis.z * s) + p.y * (c + axis.y
		* axis.y * (1 - c)) + p.z * (axis.z * axis.y * (1 - c) + axis.x * s);
	res.z = p.x * (axis.x * axis.z * (1 - c) + axis.y * s) + p.y * (axis.y *
		axis.z * (1 - c) - axis.x * s) + p.z * (c + axis.z * axis.z * (1 - c));
	return (res);
}

void				rtv_app_run(t_app *app)
{
	SDL_Event	evt;
	int			run;
	int			j;

	run = 1;
	j = 0;
	while (run)
	{
		while (SDL_PollEvent(&evt))
		{
			if (evt.type == SDL_QUIT)
				run = 0;
			else if (evt.type == SDL_KEYDOWN)
			{
				j = handling_keyboard_input(evt, app);
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
