/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 10:36:02 by celva             #+#    #+#             */
/*   Updated: 2020/03/24 18:26:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int vku_init_render(t_app *app)
{
	VkCommandPool				command_pool;
	VkCommandPoolCreateInfo		cp_create_info;
	VkCommandBufferAllocateInfo	cp_buffer_alloc_info;

	cp_create_info = (VkCommandPoolCreateInfo){
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = app->vulkan.family_index,
    };
	if (vkCreateCommandPool(app->vulkan.device, &cp_create_info, 0, &command_pool))
		return (0);
	cp_buffer_alloc_info = (VkCommandBufferAllocateInfo){
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = command_pool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = app->vulkan.sc_image_count,
    };
	app->vulkan.command_buffers = (VkCommandBuffer*)ft_memalloc(sizeof(VkCommandBuffer) * app->vulkan.sc_image_count);
	if (app->vulkan.command_buffers == NULL ||
		vkAllocateCommandBuffers(app->vulkan.device, &cp_buffer_alloc_info, app->vulkan.command_buffers))
		return (0);
	return (1);
}
