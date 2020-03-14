/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_drawframe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 17:56:35 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/14 17:57:03 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int		vku_record_cmb(t_vulkan *v)
{
	uint32_t					i;
	VkCommandBufferBeginInfo	begin_info;
	VkBufferImageCopy			region;

	begin_info = (VkCommandBufferBeginInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
	};
	region = (VkBufferImageCopy){
		.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.imageSubresource.layerCount = 1,
		.imageExtent = (VkExtent3D){WIN_WIDTH, WIN_HEIGHT, 1}
	};
	i = 0;
	while (i < v->sc_image_count)
	{
		if (vkBeginCommandBuffer((v->command_buffers)[i], &begin_info) != VK_SUCCESS)
			return (0);
		vkCmdCopyBufferToImage((v->command_buffers)[i], v->buf.buffer, v->sc_images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		if (vkEndCommandBuffer((v->command_buffers)[i]) != VK_SUCCESS)
			return (0);
		i++;
	}
	return (1);
}

int		vku_draw_frame(t_vulkan *v)
{
	uint32_t			image_ind;
	VkSubmitInfo		submit_info;
	VkPresentInfoKHR	present_info;
 
    if (vkAcquireNextImageKHR(v->device, v->swapchain, UINT64_MAX, NULL, NULL,
										&image_ind) != VK_SUCCESS)
		return (0);
	submit_info = (VkSubmitInfo){
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &((v->command_buffers)[image_ind])
	};
	if (vkQueueSubmit(v->queue, 1, &submit_info, NULL) != VK_SUCCESS)
		return (0);
	present_info = (VkPresentInfoKHR){
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.swapchainCount = 1,
		.pSwapchains = &(v->swapchain),
		.pImageIndices = &image_ind
	};
    if (vkQueuePresentKHR(v->queue, &present_info) != VK_SUCCESS)
		return (0);
	return (1);
}
