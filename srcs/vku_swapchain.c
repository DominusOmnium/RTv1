/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_swapchain.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 01:11:01 by dkathlee          #+#    #+#             */
/*   Updated: 2020/07/23 21:54:44 by dkathlee         ###   ########.fr       */
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
	VkPresentModeKHR			*pres_modes;
	uint32_t					pres_mode_count;
	VkPresentModeKHR			presentMode;
	VkExtent2D					swapchainExtent;
	VkResult					result;
	int							i;
	
	surf_form = v->phys_device.surface_formats;
	if (surf_form->format == VK_FORMAT_UNDEFINED)
		surf_form->format = VK_FORMAT_B8G8R8A8_UNORM;
	vkGetPhysicalDeviceSurfacePresentModesKHR(v->phys_device.device,
	v->surface, &pres_mode_count, NULL);
	pres_modes = (VkPresentModeKHR*)ft_memalloc(sizeof(VkPresentModeKHR) * pres_mode_count);
	vkGetPhysicalDeviceSurfacePresentModesKHR(v->phys_device.device, v->surface, &pres_mode_count, pres_modes);
	presentMode = VK_PRESENT_MODE_FIFO_KHR;
	i = 0;
	while (i < pres_mode_count)
	{
		if (pres_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
		{
			presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
			break;
		}
		i++;
	}
	v->sc_image_count = presentMode == VK_PRESENT_MODE_MAILBOX_KHR ?
	PRESENT_MODE_MAILBOX_IMAGE_COUNT : PRESENT_MODE_DEFAULT_IMAGE_COUNT;
	free(pres_modes);
	
	v->sc_images = (VkImage*)ft_memalloc(sizeof(VkImage) * v->sc_image_count);
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
        .minImageCount = v->sc_image_count,
        .imageFormat = surf_form->format,
        .imageColorSpace = surf_form->colorSpace,
        .imageExtent = swapchainExtent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = v->phys_device.surface_cap.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
    };

	result = vkCreateSwapchainKHR(v->device, &swapChainCreateInfo, NULL, &(v->swapchain));
    if (result != VK_SUCCESS)
	{
		printf("Create Swapchain Error: %d\n", result);
        return (0);
	}
	vkGetSwapchainImagesKHR(v->device, v->swapchain, &v->sc_image_count, NULL);
    vkGetSwapchainImagesKHR(v->device, v->swapchain, &v->sc_image_count, v->sc_images);

    return (1);
}
