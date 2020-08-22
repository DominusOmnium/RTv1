/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_commandbuffers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 13:16:45 by marvin            #+#    #+#             */
/*   Updated: 2020/08/18 13:16:45 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	vku_create_command_buffers(t_vulkan *v)
{
	t_uint32					i;
	VkCommandBufferAllocateInfo	allocInfo;

	allocInfo = (VkCommandBufferAllocateInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
		.commandPool = v->commandpool,
		.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
		.commandBufferCount = v->framebuffer.sc_image_count	
	};
	if (vkAllocateCommandBuffers(v->device, &allocInfo, v->command_buffers) != VK_SUCCESS)
		handle_error("Command Buffers Allocation error!");
	i = 0;
	while (i < v->framebuffer.sc_image_count)
	{
		if (vkBeginCommandBuffer(v->command_buffers[i], &(VkCommandBufferBeginInfo){.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO}) != VK_SUCCESS)
			handle_error("Begin Command Buffer error!");
		VkRenderPassBeginInfo renderPassInfo = (VkRenderPassBeginInfo) {
			.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
			.renderPass = v->renderpass,
			.framebuffer = (v->framebuffer.frame_buffers)[i],
			.clearValueCount = 1,
			.pClearValues = &(VkClearValue){{{ 0.0f, 0.1f, 0.2f, 1.0f }}},
			.renderArea = (VkRect2D){
				.offset = (VkOffset2D) { .x = 0, .y = 0 },
				.extent = v->phys_device.surface_cap.currentExtent
			}
		};
		vkCmdBeginRenderPass(v->command_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdSetViewport((v->command_buffers)[i], 0, 1, &(VkViewport){ 0.0f, 0.0f, (float)v->phys_device.surface_cap.currentExtent.width, (float)v->phys_device.surface_cap.currentExtent.height, 0.0f, 1.0f});
		vkCmdSetScissor((v->command_buffers)[i], 0, 1, &(VkRect2D){ {0, 0}, v->phys_device.surface_cap.currentExtent});
		vkCmdBindPipeline(v->command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipeline);
		vkCmdBindDescriptorSets(v->command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipelineLayout, 0, 1, &(v->descriptor.sets[i]), 0, NULL);
		vkCmdDraw(v->command_buffers[i], 3, 1, 0, 0);
		vkCmdEndRenderPass(v->command_buffers[i]);
		if (vkEndCommandBuffer(v->command_buffers[i]) != VK_SUCCESS)
			handle_error("End Command Buffer error!");
		i++;
	}
}
