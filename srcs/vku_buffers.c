/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_buffers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 17:01:03 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/11 22:58:49 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

uint32_t findMemoryType(VkPhysicalDevice dev, uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(dev, &memProperties);
	printf("memProperties.memoryTypeCount = %d\n", memProperties.memoryTypeCount);
	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++)
	{
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
			return i;
	}
	return (-1);
}

int		vku_create_buffer(t_vulkan *v)
{
	VkBufferCreateInfo buf_info = {};
	VkMemoryAllocateInfo mem_info = {};
	VkMemoryRequirements memRequirements;
	
	v->buf.buf_size = WIN_HEIGHT * WIN_WIDTH * 4;
	buf_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	buf_info.size = v->buf.buf_size;
	buf_info.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	write(1, "0\n", 2);
	if (vkCreateBuffer(v->device, &buf_info, NULL, &(v->buf.buffer)) != VK_SUCCESS)
	{
		write(1, "1\n", 2);
		return (0);
	}
	write(1, "1\n", 2);
	vkGetBufferMemoryRequirements(v->device, v->buf.buffer, &memRequirements);
	write(1, "2\n", 2);
	mem_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	mem_info.allocationSize = memRequirements.size;
	mem_info.memoryTypeIndex = findMemoryType(v->phys_device.device, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	if (vkAllocateMemory(v->device, &mem_info, NULL, &(v->buf.dev_mem)) != VK_SUCCESS)
	{
		write(1, "2\n", 2);
		return (0);
	}
	write(1, "4\n", 2);
	VkResult res;
	res = vkBindBufferMemory(v->device, v->buf.buffer, v->buf.dev_mem, 0);
	if (res != VK_SUCCESS)
	{
		write(1, "3\n", 2);
		printf("error bind: %d\n", res);
		return (0);
	}
	if (vkMapMemory(v->device, v->buf.dev_mem, 0, v->buf.buf_size, 0, &(v->buf.mem_ptr)) != VK_SUCCESS)
	{
		write(1, "4\n", 2);
		return (0);
	}
	return (1);
}
