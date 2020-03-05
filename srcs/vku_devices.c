/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_devices.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 23:37:45 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/06 00:52:08 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	vku_get_pdev_prop(const VkSurfaceKHR *surf, t_physical_devices *pd)
{
	u_int32_t	num;
	int			i;
	int			j;

	i = 0;
	while (i < pd->num_devices)
	{
		vkGetPhysicalDeviceProperties((pd->devices)[i], &((pd->dev_prop)[i]));
		vkGetPhysicalDeviceQueueFamilyProperties((pd->devices)[i], &num, NULL);
		(pd->q_family_prop)[i] = malloc(sizeof(VkQueueFamilyProperties) * num);
		(pd->q_supports_present)[i] = malloc(sizeof(VkBool32) * num);
		vkGetPhysicalDeviceQueueFamilyProperties((pd->devices)[i], &num, (pd->q_family_prop)[i]);
		j = -1;
		while (++j < num)
			vkGetPhysicalDeviceSurfaceSupportKHR((pd->devices)[i], j, surf, &((pd->q_supports_present)[i][j]));
		vkGetPhysicalDeviceSurfaceFormatsKHR((pd->devices)[i], surf, &num, NULL);
		(pd->surface_formats)[i] = malloc(sizeof(VkSurfaceFormatKHR) * num);
		j = -1;
		while (++j < num)
			vkGetPhysicalDeviceSurfaceSupportKHR((pd->devices)[i], j, surf, &((pd->q_supports_present)[i][j]));
		vkGetPhysicalDeviceSurfaceFormatsKHR((pd->devices)[i], surf, &num, (pd->surface_formats)[i]);
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR((pd->devices)[i], surf, &((pd->surface_caps)[i]));
		i++;
	}
}

int			vku_get_physical_devices(const VkInstance *inst, const VkSurfaceKHR *surf, t_physical_devices *pd)
{
	u_int32_t	num;
	int			i;
	int			j;

	vkEnumeratePhysicalDevices(inst, &(pd->num_devices), NULL);
	pd->devices = malloc(sizeof(VkPhysicalDevice) * pd->num_devices);
	pd->dev_prop = malloc(sizeof(VkPhysicalDeviceProperties) * pd->num_devices);
	pd->q_family_prop = malloc(sizeof(VkQueueFamilyProperties*) * pd->num_devices);
	pd->q_supports_present = malloc(sizeof(VkBool32*) * pd->num_devices);
	pd->surface_caps = malloc(sizeof(VkSurfaceFormatKHR*) * pd->num_devices);
	pd->surface_formats = malloc(sizeof(VkSurfaceCapabilitiesKHR) * pd->num_devices);
	vkEnumeratePhysicalDevices(inst, &(pd->num_devices), pd->devices);
	vku_get_pdev_prop(surf, pd);
}

int			vku_create_logical_device(t_app *app)
{
	float					qPriorities;
    VkDeviceQueueCreateInfo	qInfo;

	qPriorities = 1.0f;
	qInfo = (VkDeviceQueueCreateInfo){};
    qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    //////////////////////////////////////////////////qInfo.queueFamilyIndex = ;
    qInfo.queueCount = 1;
    qInfo.pQueuePriorities = &qPriorities;
 
    const char* pDevExt[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
 
    VkDeviceCreateInfo devInfo = {};
    devInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    devInfo.enabledExtensionCount = ARRAY_SIZE_IN_ELEMENTS(pDevExt);
    devInfo.ppEnabledExtensionNames = pDevExt;
    devInfo.queueCreateInfoCount = 1;
    devInfo.pQueueCreateInfos = &qInfo;
 
    VkResult res = vkCreateDevice(GetPhysDevice(), &devInfo, NULL, &(app->vulkan.device));

    printf("Device created\n");
}
