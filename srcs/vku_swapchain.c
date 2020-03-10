/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_swapchain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 01:11:01 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/10 15:04:01 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void vku_swapchain_destroy(t_app *app) //ещё нигде не вызывается
{
    vkDestroySwapchainKHR(app->vulkan.device, app->vulkan.swapchain, 0);
}

int vku_swapchain_create(t_app *app)
{
	VkSurfaceFormatKHR			*surf_form;
	VkPresentModeKHR			*presentModes;
	uint32_t					presentModeCount;
	VkPresentModeKHR			presentMode;
	VkExtent2D					swapchainExtent;
	VkResult					result;
	int							i;
	
	surf_form = app->vulkan.phys_device.surface_formats;
	surf_form->format =	surf_form->format ==
	VK_FORMAT_UNDEFINED ? VK_FORMAT_B8G8R8A8_UNORM : surf_form->format;
	
	presentModeCount = 0;
	vkGetPhysicalDeviceSurfacePresentModesKHR(app->vulkan.phys_device.device,
	app->vulkan.surface, &presentModeCount, NULL);
	presentModes = (VkPresentModeKHR*)malloc(sizeof(VkPresentModeKHR) *
														presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(app->vulkan.phys_device.device,
	app->vulkan.surface, &presentModeCount, presentModes);
	
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
	app->vulkan.swapchainImageCount = presentMode == VK_PRESENT_MODE_MAILBOX_KHR ?
	PRESENT_MODE_MAILBOX_IMAGE_COUNT : PRESENT_MODE_DEFAULT_IMAGE_COUNT;
	free(presentModes);
	
	app->vulkan.swapchainImages =
	(VkImage*)malloc(sizeof(VkImage) * app->vulkan.swapchainImageCount);
	/*vkGetPhysicalDeviceSurfaceCapabilitiesKHR(app->vulkan.phys_device.device,
	app->vulkan.surface, &(app->vulkan.phys_device.surface_cap));*/
	swapchainExtent = app->vulkan.phys_device.surface_cap.currentExtent;
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
        .surface = app->vulkan.surface,
        .minImageCount = app->vulkan.swapchainImageCount,
        .imageFormat = surf_form->format,
        .imageColorSpace = surf_form->colorSpace,
        .imageExtent = swapchainExtent,
        .imageArrayLayers = 1, // 2 for stereo
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT |
							VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = app->vulkan.phys_device.surface_cap.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
		.queueFamilyIndexCount = 0
    };

	result = vkCreateSwapchainKHR(app->vulkan.device, &swapChainCreateInfo, NULL, &(app->vulkan.swapchain));
    if (result != VK_SUCCESS)
	{
		printf("Create Swapchain Error: %d\n", result);
        return (0);
	}
	vkGetSwapchainImagesKHR(app->vulkan.device, app->vulkan.swapchain,
								&app->vulkan.swapchainImageCount, NULL);
    vkGetSwapchainImagesKHR(app->vulkan.device, app->vulkan.swapchain,
		&app->vulkan.swapchainImageCount, app->vulkan.swapchainImages);

    return (1);
}
