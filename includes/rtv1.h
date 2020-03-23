/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:24:56 by celva             #+#    #+#             */
/*   Updated: 2020/03/23 09:59:39 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# ifdef __APPLE__
# define VK_USE_PLATFORM_MACOS_MVK
# elif _WIN32
# define VK_USE_PLATFORM_WIN32_KHR
# define SDL_MAIN_HANDLED
# else
# define VK_USE_PLATFORM_XCB_KHR
# endif
# include "SDL.h"
# include "SDL_vulkan.h"
# include <vulkan/vulkan.h>
# include <vulkan/vk_sdk_platform.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# define WIN_WIDTH	1500
# define WIN_HEIGHT	1000
# define PRESENT_MODE_MAILBOX_IMAGE_COUNT 3
# define PRESENT_MODE_DEFAULT_IMAGE_COUNT 2
# ifdef _WIN32
# include <float.h>
typedef int32_t u_int32_t;
# endif

typedef struct					s_d
{
	double						x;
	double						y;
	double						z;
}								t_d;
typedef struct					s_coord
{
	double						x;
	double						y;
	double						z;
}								t_coord;
typedef struct					s_buffer
{
	VkDeviceMemory				dev_mem;
	VkBuffer					buffer;
	void						*mem_ptr;
	VkDeviceSize				buf_size;
}								t_buffer;

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
typedef struct					s_vulkan
{
	VkExtensionProperties		*ext_prop;
	VkInstance					inst;
	VkDevice					device;
	VkSurfaceKHR				surface;
	u_int32_t					family_index;
	t_physical_device			phys_device;
	VkSwapchainKHR				swapchain;
	SDL_Window					*window;
	uint32_t					sc_image_count;
	VkImage						*sc_images;
	VkCommandBuffer				*command_buffers;
	VkQueue						queue;
	t_buffer					buf;
}								t_vulkan;
typedef struct					s_sphere
{
	t_coord						center;
	double						radius;
	t_coord						color;
	int							specular;
}								t_sphere;
typedef struct					s_light
{
	char						type;
	double						intensity;
	t_coord						position;
	t_coord						direction;
}								t_light;
typedef struct					s_retr
{
	t_coord						ds;
    t_coord						o;
	double						vw;
	double						vh;
	double						d;
	double						t_min;
	double						t_max;
	t_sphere					*figures;
	t_light						*lights;
	int							n_fig;
	int							n_lig;
}								t_retr;
typedef struct					s_app
{
	t_vulkan					vulkan;
	char						*appname;

}								t_app;

int		rtv_app_create(t_app *app);
void	rtv_app_run(t_app *app);
int		vku_instance_create(t_app *app);
int		vku_get_physical_device(t_vulkan *v);
int		vku_create_logical_device(t_vulkan *v);
int		vku_window_create(t_app *app);
int		vku_init_render(t_app *app);
int		vku_record_cmb(t_vulkan *v);
int		vku_draw_frame(t_vulkan *v);
int		vku_swapchain_create(t_vulkan *v);
int		vku_create_buffer(t_vulkan *v);
void	handle_error(char *msg);
void    raytracing(t_retr *r, t_app *app);
#endif
