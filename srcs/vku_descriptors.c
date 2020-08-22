/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_descriptors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/18 09:37:51 by marvin            #+#    #+#             */
/*   Updated: 2020/08/18 09:37:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	vku_create_descriptor_pool(t_vulkan *v)
{
	VkDescriptorPoolSize		pool_sizes;
	VkDescriptorPoolCreateInfo	pool_info;
	
	pool_sizes = (VkDescriptorPoolSize){
		.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		.descriptorCount = v->framebuffer.sc_image_count
	};
	pool_info = (VkDescriptorPoolCreateInfo){
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
		.poolSizeCount = 1,
		.pPoolSizes = &pool_sizes,
		.maxSets = v->framebuffer.sc_image_count
	};
	if (vkCreateDescriptorPool(v->device, &pool_info, NULL, &v->descriptor.pool) != VK_SUCCESS)
		handle_error("Descriptor pool creation error!");
}

void	vku_create_descriptor_set_layout(t_vulkan *v)
{
	VkDescriptorSetLayoutBinding	sbo_layout_binding;
	VkDescriptorSetLayoutCreateInfo	layout_info;

	sbo_layout_binding = (VkDescriptorSetLayoutBinding){
		.binding = 0,
		.descriptorCount = 1,
		.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
		.pImmutableSamplers = NULL,
		.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT
	};
	layout_info = (VkDescriptorSetLayoutCreateInfo){
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
		.bindingCount = 1,
		.pBindings = &sbo_layout_binding
	};
	if (vkCreateDescriptorSetLayout(v->device, &layout_info, NULL, &(v->descriptor.set_layout)) != VK_SUCCESS)
		handle_error("Descriptor set layout creation error!");
}

static void	update_descriptor_sets(t_vulkan *v, uint32_t size)
{
	size_t						i;
	VkDescriptorBufferInfo		buffer_info;
	VkWriteDescriptorSet		descriptor_write;

	i = 0;
	while (i < v->framebuffer.sc_image_count)
	{
		buffer_info = (VkDescriptorBufferInfo){
			.buffer = (v->sbo_buffers)[i].buffer,
			.offset = 0,
			.range = size
		};
		descriptor_write = (VkWriteDescriptorSet){
			.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
			.dstSet = (v->descriptor.sets)[i],
			.dstBinding = 0,
			.dstArrayElement = 0,
			.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
			.descriptorCount = 1,
			.pBufferInfo = &buffer_info
		};
		vkUpdateDescriptorSets(v->device, 1, &descriptor_write, 0, NULL);
		i++;
	}
}

void	vku_create_descriptor_sets(t_vulkan *v, uint32_t size)
{
	VkDescriptorSetAllocateInfo	alloc_info;
	VkDescriptorSetLayout		layouts[MAX_SWAPCHAIN_IMAGES];
	
	layouts[0] = v->descriptor.set_layout;
	layouts[1] = v->descriptor.set_layout;
	layouts[2] = v->descriptor.set_layout;
	alloc_info = (VkDescriptorSetAllocateInfo){
		.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
		.descriptorPool = v->descriptor.pool,
		.descriptorSetCount = v->framebuffer.sc_image_count,
		.pSetLayouts = layouts
	};
	if (vkAllocateDescriptorSets(v->device, &alloc_info, v->descriptor.sets) != VK_SUCCESS)
		handle_error("Descriptor sets allocation error!");
	update_descriptor_sets(v, size);
}

