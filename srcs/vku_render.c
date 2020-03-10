/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_render.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celva <celva@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 10:36:02 by celva             #+#    #+#             */
/*   Updated: 2020/03/10 10:59:42 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int vku_init_render(t_app *app)
{
	VkCommandPool commandPool;
	
	VkCommandPoolCreateInfo commandPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = app->vulkan.family_index,
    };
	vkCreateCommandPool(app->vulkan.device, &commandPoolCreateInfo, 0,
																&commandPool);
	VkCommandBufferAllocateInfo commandBufferAllocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = commandPool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = app->vulkan.swapchainImageCount,
    };
	app->vulkan.commandBuffers = (VkCommandBuffer*)malloc(sizeof(VkCommandBuffer) * app->vulkan.swapchainImageCount);
	vkAllocateCommandBuffers(app->vulkan.device, &commandBufferAllocInfo, app->vulkan.commandBuffers);
	
}
