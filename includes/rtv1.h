/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:24:56 by celva             #+#    #+#             */
/*   Updated: 2020/03/06 13:54:14 by dkathlee         ###   ########.fr       */
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
# define PRESENT_MODE_MAILBOX_IMAGE_COUNT 3
# define PRESENT_MODE_DEFAULT_IMAGE_COUNT 2

typedef struct					s_physical_device
{
	u_int32_t					num_families;
	u_int32_t					num_formats;
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
	u_int32_t				family_index;
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
int		vku_get_physical_device(t_vulkan *v);
int		vku_create_logical_device(t_vulkan *v);
#endif
