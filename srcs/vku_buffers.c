/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_buffers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/11 17:01:03 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/18 16:02:04 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static uint32_t				find_memory_type(VkPhysicalDevice dev,
												uint32_t type_filter,
												VkMemoryPropertyFlags prop)
{
	VkPhysicalDeviceMemoryProperties	mem_properties;
	uint32_t							i;

	vkGetPhysicalDeviceMemoryProperties(dev, &mem_properties);
	i = 0;
	while (i < mem_properties.memoryTypeCount)
	{
		if ((type_filter & (1 << i)) &&
			(mem_properties.memoryTypes[i].propertyFlags & prop) == prop)
			return (i);
		i++;
	}
	return (-1);
}

static VkBufferCreateInfo	get_buffer_info(t_vulkan *v,
											VkBufferUsageFlags usage,
											uint32_t index)
{
	VkBufferCreateInfo		buffer_info;

	buffer_info = (VkBufferCreateInfo){
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = (v->sbo_buffers)[index].buf_size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE,
		.queueFamilyIndexCount = 1,
		.pQueueFamilyIndices = &(v->phys_device.family_index)
	};
	return (buffer_info);
}

void						vku_create_buffer(t_vulkan *v, uint32_t index,
											VkBufferUsageFlags usage,
											VkMemoryPropertyFlags properties)
{
	VkBufferCreateInfo		buffer_info;
	VkMemoryAllocateInfo	alloc_info;
	VkMemoryRequirements	mem_requirements;

	buffer_info = get_buffer_info(v, usage, index);
	if (vkCreateBuffer(v->device, &buffer_info, NULL,
						&((v->sbo_buffers)[index].buffer)) != VK_SUCCESS)
		handle_error("Buffer creation error!");
	vkGetBufferMemoryRequirements(v->device, (v->sbo_buffers)[index].buffer,
														&mem_requirements);
	alloc_info = (VkMemoryAllocateInfo) {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = mem_requirements.size,
		.memoryTypeIndex = find_memory_type(v->phys_device.device,
								mem_requirements.memoryTypeBits, properties)
	};
	if (vkAllocateMemory(v->device, &alloc_info, NULL,
							&(v->sbo_buffers[index].dev_mem)) != VK_SUCCESS)
		handle_error("Memory allocation error!");
	if (vkBindBufferMemory(v->device, v->sbo_buffers[index].buffer,
							v->sbo_buffers[index].dev_mem, 0) != VK_SUCCESS)
		handle_error("Bind Buffer Memory error!");
}
