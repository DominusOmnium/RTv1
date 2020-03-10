/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_swapchain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celva <celva@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 01:11:01 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/10 16:02:34 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void vku_swapchain_destroy(t_vulkan *v) //ещё нигде не вызывается
{
    vkDestroySwapchainKHR(v->device, v->swapchain, 0);
}

int vku_swapchain_create(t_vulkan *v)
{
	VkSurfaceFormatKHR			*surf_form;
	VkPresentModeKHR			*presentModes;
	uint32_t					presentModeCount;
	VkPresentModeKHR			presentMode;
	VkExtent2D					swapchainExtent;
	VkResult					result;
	int							i;
	
	surf_form = v->phys_device.surface_formats;
	surf_form->format =	surf_form->format ==
	VK_FORMAT_UNDEFINED ? VK_FORMAT_B8G8R8A8_UNORM : surf_form->format;
	
	presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(v->phys_device.device,
	v->surface, &presentModeCount, NULL);
	presentModes = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) *
														presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(v->phys_device.device,
	v->surface, &presentModeCount, presentModes);
	
	presentMode = VK_PRESENT_MODE_FIFO_KHR;
	i = 0;
	while (i < presentModeCount)
	{
		if (presentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		i++;
	}
	v->swapchainImageCount = presentMode == VK_PRESENT_MODE_MAILBOX_KHR ?
	PRESENT_MODE_MAILBOX_IMAGE_COUNT : PRESENT_MODE_DEFAULT_IMAGE_COUNT;
	free(presentModes);
	
	v->swapchainImages =
	(VkImage*)malloc(sizeof(VkImage) * v->swapchainImageCount);
	/*vkGetPhysicalDeviceSurfaceCapabilitiesKHR(app->vulkan.phys_device.device,
	app->vulkan.surface, &(app->vulkan.phys_device.surface_cap));*/
	swapchainExtent = v->phys_device.surface_cap.currentExtent;
	if (swapchainExtent.width == UINT32_MAX)
    {
        /*swapchainExtent.width = clamp_u32(WIN_WIDTH,
		surfaceCapabilities.minImageExtent.width,
		surfaceCapabilities.maxImageExtent.width);
        swapchainExtent.height = clamp_u32(WIN_HEIGHT,
		surfaceCapabilities.minImageExtent.height,
		surfaceCapabilities.maxImageExtent.height);*/
		swapchainExtent.width = WIN_WIDTH;
		swapchainExtent.height = WIN_HEIGHT;
    }

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = v->surface,
        .minImageCount = v->swapchainImageCount,
        .imageFormat = surf_form->format,
        .imageColorSpace = surf_form->colorSpace,
        .imageExtent = swapchainExtent,
        .imageArrayLayers = 1, // 2 for stereo
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
							VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = v->phys_device.surface_cap.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
		.queueFamilyIndexCount = 0
    };

	result = vkCreateSwapchainKHR(v->device, &swapChainCreateInfo, NULL, &(v->swapchain));
    if (result != VK_SUCCESS)
	{
		printf("Create Swapchain Error: %d\n", result);
        return (0);
	}
	vkGetSwapchainImagesKHR(v->device, v->swapchain,
								&v->swapchainImageCount, NULL);
    vkGetSwapchainImagesKHR(v->device, v->swapchain,
		&v->swapchainImageCount, v->swapchainImages);

    return (1);
}
