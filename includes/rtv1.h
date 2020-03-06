/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:24:56 by celva             #+#    #+#             */
/*   Updated: 2020/03/06 11:41:08 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# ifdef __APPLE__
# define VK_USE_PLATFORM_MACOS_MVK
# elif _WIN32
# define VK_USE_PLATFORM_WIN32_KHR
# else
# define VK_USE_PLATFORM_XCB_KHR
# endif
# include <SDL2/SDL.h>
# include <SDL2/SDL_vulkan.h>
# include <vulkan/vulkan.h>
# include <vulkan/vk_sdk_platform.h>
# include <stdio.h>
# define WIN_WIDTH	1500
# define WIN_HEIGHT	1000

typedef struct					s_physical_device
{
    VkPhysicalDevice			device;
    VkPhysicalDeviceProperties	dev_prop;
    VkQueueFamilyProperties		*q_family_prop;
    VkBool32					*q_supports_present;
    VkSurfaceFormatKHR			*surface_formats;
    VkSurfaceCapabilitiesKHR	surface_cap;
}								t_physical_device;
typedef struct			s_vulkan
{
	VkExtensionProperties	*ext_prop;
	VkInstance				inst;
	VkDevice				device;
	VkSurfaceKHR			surface;
	t_physical_device		phys_device;
}						t_vulkan;
typedef struct			s_app
{
	t_vulkan			vulkan;
	char				*appname;
}						t_app;

int		rtv_app_create(t_app *app);
int		rtv_app_destroy(t_app *app);
void	rtv_app_run(t_app *app);
int		vku_instance_create(t_app *app);
#endif
