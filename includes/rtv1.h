/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:24:56 by celva             #+#    #+#             */
/*   Updated: 2020/07/31 23:35:18 by dkathlee         ###   ########.fr       */
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
# include <SDL.h>
# include <SDL_vulkan.h>
# include "vec_lib.h"
# include "ft_printf.h"

#include <shaderc/shaderc.h>

# include <vulkan/vulkan.h>
# include <vulkan/vk_sdk_platform.h>
# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <float.h>
# define WIN_WIDTH	1500
# define WIN_HEIGHT	1000
/*# define PRESENT_MODE_MAILBOX_IMAGE_COUNT 3
# define PRESENT_MODE_DEFAULT_IMAGE_COUNT 2*/
# ifdef _WIN32
# include <float.h>
typedef int32_t u_int32_t;
# endif

typedef t_vec3_double t_vec3;
/*
	Vulkan structures
*/
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
	VkPipeline					pipeline;
	VkPipelineLayout			pipelineLayout;
	VkRenderPass				renderpass;
	SDL_Window					*window;
	uint32_t					sc_image_count;
	VkImage						*sc_images;
	VkImageView					sc_image_views[3];
	VkFramebuffer				frame_buffers[3];
	VkFormat					image_format;
	VkCommandBuffer				*command_buffers;
	VkCommandPool				commandpool;
	VkQueue						queue;
	t_buffer					buf;
}								t_vulkan;
/*
	Objects utils
*/
typedef struct					s_transform
{
	t_vec3						rotation;
	t_vec3						position;
}								t_transform;
typedef enum
{
	obj_null, obj_sphere, obj_plane, obj_cone, obj_cylinder
}	t_obj_type;
/*
	Objects
*/
typedef	struct					s_figure
{
	double 				radius;
	t_vec3				direction;
	double				height;
	t_vec3				ver;
	t_vec3				vertices[4];

}								t_figure;
typedef struct					s_object
{
	t_obj_type			type;
	t_transform			transform;
	t_vec3				color;
	int					specular;
	t_figure			figures;
}								t_object;
/*typedef struct					s_sphere
{
	double				radius;
}								t_sphere;
typedef struct					s_plane
{
	t_vec3				direction;
	t_vec3				vertices[4];
}								t_plane;
typedef struct					s_cylinder
{
	double				radius;
	t_vec3				direction;
}								t_cylinder;
typedef struct					s_cone
{
	double				radius;
	double				height;
	t_vec3				ver;
	t_vec3				direction;
}								t_cone;*/
/*
	Light
*/
typedef enum
{
	light_ambient, light_point, light_directional
}	t_light_type;
typedef struct					s_light
{
	t_light_type				type;
	double						intensity;
	t_vec3						position;
	t_vec3						direction;
}								t_light;
/*
	Utils
*/
typedef struct					s_interRS
{
	double						t1;
	double						t2;
	t_vec3						center;
	double						radius;
}								t_interRS;
typedef struct					s_closest
{
	double						t_min;
	double						t_max;
	double						closest_t;
}								t_closest;
typedef struct					s_retr
{
	t_vec3						ds;
	double						vw;
	double						vh;
	double						d;
	double						t_min;
	double						t_max;
	t_object					*figures;
	t_light						*lights;
	int							n_fig;
	int							n_lig;
	t_closest					t_c;
	t_transform					camera;
}								t_retr;
typedef struct					s_app
{
	t_vulkan					vulkan;
	char						*appname;

}								t_app;


enum {
    Kb = (1 << 10),
    Mb = (1 << 20),
    MAX_DEVICE_COUNT = 8,
    MAX_QUEUE_COUNT = 4, //ATM there should be at most transfer, graphics, compute, graphics+compute families
    MAX_PRESENT_MODE_COUNT = 6, // At the moment in spec
    MAX_SWAPCHAIN_IMAGES = 3,
    FRAME_COUNT = 2,
    PRESENT_MODE_MAILBOX_IMAGE_COUNT = 3,
    PRESENT_MODE_DEFAULT_IMAGE_COUNT = 2,
    UPLOAD_REGION_SIZE = 64 * Kb,
    UPLOAD_BUFFER_SIZE = FRAME_COUNT * UPLOAD_REGION_SIZE,
    STATIC_BUFFER_SIZE = 64 * Kb,
};

enum {
    VULKAN_MEM_DEVICE_READBACK,
    VULKAN_MEM_DEVICE_UPLOAD,
    VULKAN_MEM_DEVICE_LOCAL,
    VULKAN_MEM_COUNT
};

VkFence frameFences[2];
VkSemaphore imageAvailableSemaphores[2];
VkSemaphore renderFinishedSemaphores[2];

VkBuffer uploadBuffer;
VkDeviceMemory uploadBufferMemory;
void* uploadBufferPtr;
VkBuffer staticBuffer;
VkDeviceMemory staticBufferMemory;
VkPhysicalDeviceMemoryProperties deviceMemProperties;
uint32_t compatibleMemTypes[VULKAN_MEM_COUNT];


int		rtv_app_create(t_app *app);
void	rtv_app_run(t_app *app, char *fname);
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
int		read_scene(char *fname, t_retr *r);
size_t	load_shader_file(char *fname, char **shader);
VkShaderModule	vku_createShaderModule(t_vulkan *v, char *code, uint32_t codeL);
int init_render(t_vulkan *v);
void draw_frame(t_vulkan *v, t_retr *r);
//void	shader_compile(char *code);
#endif
