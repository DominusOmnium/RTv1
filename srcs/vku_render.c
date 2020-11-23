/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 10:36:02 by celva             #+#    #+#             */
/*   Updated: 2020/08/28 13:28:31 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		vku_destroy_sync_objects(t_vulkan *v)
{
	uint32_t				i;

	i = 0;
	while (i < v->framebuffer.sc_image_count)
	{
		vkDestroySemaphore(v->device, v->sync.image_available_sem[i], NULL);
		vkDestroySemaphore(v->device, v->sync.render_finished_sem[i], NULL);
		vkDestroyFence(v->device, v->sync.frame_fences[i], NULL);
		i++;
	}
}

static void	create_sync_objects(t_vulkan *v)
{
	uint32_t				i;
	VkSemaphoreCreateInfo	semaphore_create_info;
	VkFenceCreateInfo		fence_create_info;

	semaphore_create_info = (VkSemaphoreCreateInfo){
		.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
		.pNext = NULL};
	fence_create_info = (VkFenceCreateInfo){
		.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
		.flags = VK_FENCE_CREATE_SIGNALED_BIT
	};
	i = 0;
	while (i < v->framebuffer.sc_image_count)
	{
		if (vkCreateSemaphore(v->device, &semaphore_create_info, 0,
				&(v->sync.image_available_sem[i])) != VK_SUCCESS)
			handle_error("Image Availible Semaphore creation error!");
		if (vkCreateSemaphore(v->device, &semaphore_create_info, 0,
				&(v->sync.render_finished_sem[i])) != VK_SUCCESS)
			handle_error("Render Finished Semaphore creation error!");
		if (vkCreateFence(v->device, &fence_create_info, 0,
							&(v->sync.frame_fences[i])) != VK_SUCCESS)
			handle_error("Fence creation error!");
		i++;
	}
}

static void	create_buffers(t_vulkan *v)
{
	t_uint32	i;

	i = 0;
	while (i < v->framebuffer.sc_image_count)
	{
		(v->sbo_buffers)[i].buf_size = STORAGE_BUFFER_SIZE;
		vku_create_buffer(v, (v->sbo_buffers) + i, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
								VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
								VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		(v->texture_buffers)[i].buf_size = TEXTURES_BUFFER_SIZE;
		vku_create_buffer(v, (v->texture_buffers) + i, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
								VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
								VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		i++;
	}
}

void		load_texture(t_vulkan *v, t_rt *r)
{
	SDL_Surface			*an_surf;
	SDL_Surface			*surf;
	SDL_Surface			*an_surf_normal;
	SDL_Surface			*surf_normal;
	SDL_PixelFormat		fmt;

	if ((an_surf = IMG_Load("Subway_Tiles_002_basecolor.jpg")) == NULL)
		handle_error("Loading image error!");
	if ((an_surf_normal = IMG_Load("Subway_Tiles_002_normal.jpg")) == NULL)
		handle_error("Loading image error!");
	/*if ((fmt = ft_memalloc(sizeof(SDL_PixelFormat))) == NULL)
		handle_error(ERROR_MEM_ALLOC);*/
	ft_memcpy(&fmt, an_surf->format, sizeof(SDL_PixelFormat));
	fmt.BytesPerPixel = 4;
	fmt.BitsPerPixel = 32;
	fmt.Rmask = RMASK;
	fmt.Gmask = GMASK;
	fmt.Bmask = BMASK;
	fmt.Amask = AMASK;
	if ((surf = SDL_ConvertSurface(an_surf, &fmt, an_surf->flags)) == NULL)
		handle_error("Surface convertation error");
	if ((surf_normal = SDL_ConvertSurface(an_surf_normal, &fmt, an_surf_normal->flags)) == NULL)
		handle_error("Surface convertation error");
	SDL_FreeSurface(an_surf);
	SDL_FreeSurface(an_surf_normal);
	//ft_memdel((void **)&fmt);
	uint32_t i;
	i = 0;
	while (i < r->n_fig)
	{
		r->sbo_figures[i].texture.height = surf->h;
		r->sbo_figures[i].texture.width = surf->w;
		r->sbo_figures[i].texture.offset_in_buffer = 0;
		r->sbo_figures[i].normal_map.height = surf_normal->h;
		r->sbo_figures[i].normal_map.width = surf_normal->w;
		r->sbo_figures[i].normal_map.offset_in_buffer = surf->h * surf->pitch;
		i++;
	}
	void *data;
	i = 0;
	while (i < v->framebuffer.sc_image_count)
	{
		if (vkMapMemory(v->device, v->texture_buffers[i].dev_mem, 0,
							TEXTURES_BUFFER_SIZE, 0, &data) != VK_SUCCESS)
			handle_error("Map Memory error!");
		ft_memcpy(data, surf->pixels, surf->h * surf->pitch);
		ft_memcpy(data + surf->h * surf->pitch, surf_normal->pixels, surf_normal->h * surf_normal->pitch);
		vkUnmapMemory(v->device, v->texture_buffers[i].dev_mem);
		i++;
	}
	SDL_FreeSurface(surf);
	SDL_FreeSurface(surf_normal);
}

void		vku_init_render(t_vulkan *v, t_rt *r)
{
	VkCommandPoolCreateInfo		commandpool_create_info;

	commandpool_create_info = (VkCommandPoolCreateInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
		.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
		.queueFamilyIndex = v->phys_device.family_index,
	};
	if (vkCreateCommandPool(v->device, &commandpool_create_info, 0,
								&(v->commandpool)) != VK_SUCCESS)
		handle_error("Commandpool creation error!");
	create_sync_objects(v);
	create_buffers(v);
	vku_create_render_pass(v);
	vku_create_descriptor_set_layout(v);
	vku_create_pipeline(v);
	vku_create_framebuffers(v);
	vku_create_descriptor_pool(v);
	vku_create_descriptor_sets(v);
	vku_create_command_buffers(v);
	load_texture(v, r);
}
