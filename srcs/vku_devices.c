/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_devices.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 23:37:45 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/14 17:20:05 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			vku_get_physical_device(t_vulkan *v)
{
	VkPhysicalDevice			*d;
	VkPhysicalDeviceProperties	*d_prop;
	VkQueueFamilyProperties		*qf_prop;
	VkBool32					*sup_pres;
	u_int32_t					num_d;
	u_int32_t					num;
	u_int32_t					i;
	u_int32_t					j;


	vkEnumeratePhysicalDevices(v->inst, &num_d, NULL);
	d = malloc(sizeof(VkPhysicalDevice) * num_d);
	d_prop = malloc(sizeof(VkPhysicalDeviceProperties) * num_d);
	vkEnumeratePhysicalDevices(v->inst, &num_d, d);
	i = 0;
	while (i < num_d)
	{
		vkGetPhysicalDeviceProperties(d[i], &(d_prop[i]));
		vkGetPhysicalDeviceQueueFamilyProperties(d[i], &num, NULL);
		qf_prop = malloc(sizeof(VkQueueFamilyProperties) * num);
		sup_pres = malloc(sizeof(VkBool32) * num);
		vkGetPhysicalDeviceQueueFamilyProperties(d[i], &num, qf_prop);
		j = -1;
		while (++j < num)
		{
			vkGetPhysicalDeviceSurfaceSupportKHR(d[i], j, v->surface, &(sup_pres[j]));
			if (qf_prop[j].queueFlags & VK_QUEUE_GRAPHICS_BIT)
				if (sup_pres[j])
				{
					v->phys_device.dev_prop = d_prop[i];
					v->phys_device.device = d[i];
					v->phys_device.q_family_prop = qf_prop;
					v->phys_device.q_supports_present = sup_pres;
					v->phys_device.num_families = num;
					vkGetPhysicalDeviceSurfaceFormatsKHR(d[i], v->surface, &(num), NULL);
					v->phys_device.surface_formats = malloc(sizeof(VkSurfaceFormatKHR) * num);
					v->phys_device.num_formats = num;
					v->family_index = j;
					vkGetPhysicalDeviceSurfaceFormatsKHR(d[i], v->surface, &num, v->phys_device.surface_formats);
					vkGetPhysicalDeviceSurfaceCapabilitiesKHR(d[i], v->surface, &(v->phys_device.surface_cap));
					free((void*)d);
					free((void*)d_prop);
					return (1);
				}
			free((void*)qf_prop);
			free((void*)sup_pres);
		}
		i++;
	}
	free((void*)d);
	free((void*)d_prop);
	return (0);
}

int			vku_create_logical_device(t_vulkan *v)
{
	float					queue_priorities;
    VkDeviceQueueCreateInfo	q_create_info;
	VkDeviceCreateInfo		dev_info;
	const char				*dev_ext[1]; 

	queue_priorities = 1.0f;
	q_create_info = (VkDeviceQueueCreateInfo){
		.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
		.queueFamilyIndex = v->family_index,
		.queueCount = 1,
		.pQueuePriorities = &queue_priorities
	};
    dev_ext[0] = VK_KHR_SWAPCHAIN_EXTENSION_NAME;
    dev_info = (VkDeviceCreateInfo){
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.enabledExtensionCount = 1,
		.ppEnabledExtensionNames = dev_ext,
		.queueCreateInfoCount = 1,
		.pQueueCreateInfos = &q_create_info
	};
    if (vkCreateDevice(v->phys_device.device, &dev_info, NULL, &(v->device)) != VK_SUCCESS)
		return (0);
	vkGetDeviceQueue(v->device, v->family_index, 0, &(v->queue));
	return (1);
}
