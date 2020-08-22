/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_devices.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 23:37:45 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/19 14:53:14 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static uint32_t	find_comp_mem_type(VkPhysicalDeviceMemoryProperties* mem_prop,
									VkMemoryPropertyFlags flags)
{
    uint32_t	res;
	uint32_t	i;
	
	res = 0;
	i = 0;
	while (i < mem_prop->memoryTypeCount)
	{
        res |= ((mem_prop->memoryTypes[i].propertyFlags & flags) == flags) << i;
		i++;
	}
    return (res);
}

void			vku_get_physical_device(t_vulkan *v)
{
	VkPhysicalDevice			*d;
	VkPhysicalDeviceProperties	*d_prop;
	VkQueueFamilyProperties		*qf_prop;
	VkBool32					*sup_pres;
	uint32_t					num_d;
	uint32_t					num;
	uint32_t					i;
	uint32_t					j;

	if (vkEnumeratePhysicalDevices(v->inst, &num_d, NULL) != VK_SUCCESS)
		handle_error("Enumerate Physical Devices error!");
	d = ft_memalloc(sizeof(VkPhysicalDevice) * num_d);
	d_prop = ft_memalloc(sizeof(VkPhysicalDeviceProperties) * num_d);
	if (vkEnumeratePhysicalDevices(v->inst, &num_d, d) != VK_SUCCESS)
		handle_error("Enumerate Physical Devices error!");
	i = 0;
	while (i < num_d)
	{
		vkGetPhysicalDeviceProperties(d[i], &(d_prop[i]));
		vkGetPhysicalDeviceQueueFamilyProperties(d[i], &num, NULL);
		qf_prop = ft_memalloc(sizeof(VkQueueFamilyProperties) * num);
		sup_pres = ft_memalloc(sizeof(VkBool32) * num);
		vkGetPhysicalDeviceQueueFamilyProperties(d[i], &num, qf_prop);
		j = -1;
		while (++j < num)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(d[i], j, v->surface, &(sup_pres[j]));
			if (qf_prop[j].queueFlags & VK_QUEUE_GRAPHICS_BIT/* && d_prop[i].deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU*/)
				if (sup_pres[j])
				{
					v->phys_device.dev_prop = d_prop[i];
					v->phys_device.device = d[i];
					v->phys_device.q_family_prop = qf_prop;
					v->phys_device.q_supports_present = sup_pres;
					v->phys_device.num_families = num;
					vkGetPhysicalDeviceSurfaceFormatsKHR(d[i], v->surface, &(num), NULL);
					v->phys_device.surface_formats = ft_memalloc(sizeof(VkSurfaceFormatKHR) * num);
					v->phys_device.num_formats = num;
					v->phys_device.family_index = j;
					vkGetPhysicalDeviceSurfaceFormatsKHR(d[i], v->surface, &num, v->phys_device.surface_formats);
					vkGetPhysicalDeviceSurfaceCapabilitiesKHR(d[i], v->surface, &(v->phys_device.surface_cap));
					ft_memdel((void**)&d);
					ft_memdel((void**)&d_prop);
					break ;
				}
			ft_memdel((void**)&qf_prop);
			ft_memdel((void**)&sup_pres);
		}
		i++;
	}
	ft_memdel((void**)&d);
	ft_memdel((void**)&d_prop);
}

void			vku_create_logical_device(t_vulkan *v)
{
	float					queue_priorities;
	VkDeviceCreateInfo		dev_info;
	const char				*dev_ext[1]; 
	VkPhysicalDeviceMemoryProperties	device_mem_prop;

	queue_priorities = 1.0f;
    dev_ext[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    dev_info = (VkDeviceCreateInfo){
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.enabledExtensionCount = 1,
		.ppEnabledExtensionNames = dev_ext,
		.pEnabledFeatures = NULL,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &(VkDeviceQueueCreateInfo){
			.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.queueFamilyIndex = v->phys_device.family_index,
			.queueCount = 1,
			.pQueuePriorities = &queue_priorities
		}
	};
    if (vkCreateDevice(v->phys_device.device, &dev_info, NULL, &(v->device)) != VK_SUCCESS)
		handle_error("Device creation error!");
	vkGetDeviceQueue(v->device, v->phys_device.family_index, 0, &(v->queue));
	vkGetPhysicalDeviceMemoryProperties(v->phys_device.device, &device_mem_prop);
    v->compatible_mem_types[VULKAN_MEM_DEVICE_READBACK]
        = find_comp_mem_type(&device_mem_prop, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                                            | VK_MEMORY_PROPERTY_HOST_CACHED_BIT);
    v->compatible_mem_types[VULKAN_MEM_DEVICE_UPLOAD]
        = find_comp_mem_type(&device_mem_prop, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
                                                            | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    v->compatible_mem_types[VULKAN_MEM_DEVICE_LOCAL]
        = find_comp_mem_type(&device_mem_prop, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
}
