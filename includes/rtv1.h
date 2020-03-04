#ifndef RTV1_H
# define RTV1_H
# ifdef __APPLE__
# define VK_USE_PLATFORM_MACOS_MVK
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

typedef struct					s_physical_devices
{
    VkPhysicalDevice			*devices;
    VkPhysicalDeviceProperties	*dev_prop;
    VkQueueFamilyProperties		**q_family_prop;
    VkBool32					**q_supports_present;
    VkSurfaceFormatKHR			**surface_formats;
    VkSurfaceCapabilitiesKHR	*surface_caps;
}								t_physical_devices;
typedef struct			s_vulkan
{
	VkInstance			inst;
	VkDevice			device;
	VkSurfaceKHR		surface;
	t_physical_devices	phys_devices;
	char				*appname;
}						t_vulkan;

#endif
