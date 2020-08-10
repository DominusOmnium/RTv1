/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_buffers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 17:01:03 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/10 15:38:20 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

uint32_t findMemoryType(VkPhysicalDevice dev, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(dev, &memProperties);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}
	return (-1);
}
/*
int		vku_create_buffer(t_vulkan *v)
{
	VkBufferCreateInfo		buf_info;
	VkMemoryAllocateInfo	mem_info;
	VkMemoryRequirements	mem_requirements;
	
	v->buf.buf_size = WIN_HEIGHT * WIN_WIDTH * 4;
	buf_info = (VkBufferCreateInfo){
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = v->buf.buf_size,
		.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT
	};
	if (vkCreateBuffer(v->device, &buf_info, NULL, &(v->buf.buffer)) != VK_SUCCESS)
		return (0);
	vkGetBufferMemoryRequirements(v->device, v->buf.buffer, &mem_requirements);
	mem_info = (VkMemoryAllocateInfo){
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = mem_requirements.size,
		.memoryTypeIndex = findMemoryType(v->phys_device.device, mem_requirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT)
	};
	if (mem_info.memoryTypeIndex == -1 ||
		vkAllocateMemory(v->device, &mem_info, NULL, &(v->buf.dev_mem)) != VK_SUCCESS ||
		vkBindBufferMemory(v->device, v->buf.buffer, v->buf.dev_mem, 0) != VK_SUCCESS ||
		vkMapMemory(v->device, v->buf.dev_mem, 0, v->buf.buf_size, 0, &(v->buf.mem_ptr)) != VK_SUCCESS)
		return (0);
	return (1);
}
*/
void	vku_create_buffer(t_vulkan *v, t_uint32 index, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties)
{
	VkBufferCreateInfo		buffer_info;
	VkMemoryAllocateInfo	alloc_info;
	VkMemoryRequirements	mem_requirements;
	
	buffer_info = (VkBufferCreateInfo){
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = (v->sbo_buffers)[index].buf_size,
        .usage = usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = &(v->phys_device.family_index)
    };
	if (vkCreateBuffer(v->device, &buffer_info, NULL, &((v->sbo_buffers)[index].buffer)) != VK_SUCCESS)
		handle_error("Buffer creation error!");
	vkGetBufferMemoryRequirements(v->device, (v->sbo_buffers)[index].buffer, &mem_requirements);
	alloc_info = (VkMemoryAllocateInfo) {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = mem_requirements.size,
		.memoryTypeIndex = findMemoryType(v->phys_device.device, mem_requirements.memoryTypeBits, properties)
	};
	alloc_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	alloc_info.allocationSize = mem_requirements.size;
	alloc_info.memoryTypeIndex = findMemoryType(v->phys_device.device, mem_requirements.memoryTypeBits, properties);
	if (vkAllocateMemory(v->device, &alloc_info, NULL, &(v->sbo_buffers[index].dev_mem)) != VK_SUCCESS)
		handle_error("Memory allocation error!");
	vkBindBufferMemory(v->device, v->sbo_buffers[index].buffer, v->sbo_buffers[index].dev_mem, 0);
}
