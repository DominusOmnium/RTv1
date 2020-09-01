/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 09:16:26 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/28 13:26:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VKU_H
# define VKU_H
# include "vec_lib.h"
# include <vulkan.h>
# include <vk_sdk_platform.h>

enum {
	VULKAN_MEM_DEVICE_READBACK,
	VULKAN_MEM_DEVICE_UPLOAD,
	VULKAN_MEM_DEVICE_LOCAL,
	VULKAN_MEM_COUNT
};

enum {
	Kb = (1 << 10),
	Mb = (1 << 20),
	MAX_SWAPCHAIN_IMAGES = 3,
	FRAME_COUNT = 2,
	PRESENT_MODE_MAILBOX_IMAGE_COUNT = 3,
	PRESENT_MODE_DEFAULT_IMAGE_COUNT = 2,
	UPLOAD_REGION_SIZE = 64 * Kb,
	UPLOAD_BUFFER_SIZE = FRAME_COUNT * UPLOAD_REGION_SIZE,
	STATIC_BUFFER_SIZE = 64 * Kb
};

typedef struct							s_buffer
{
	VkDeviceMemory				dev_mem;
	VkBuffer					buffer;
	void						*mem_ptr;
	VkDeviceSize				buf_size;
}										t_buffer;

typedef struct							s_framebuffer
{
	uint32_t					sc_image_count;
	VkImage						sc_images[MAX_SWAPCHAIN_IMAGES];
	VkImageView					sc_image_views[MAX_SWAPCHAIN_IMAGES];
	VkFramebuffer				frame_buffers[MAX_SWAPCHAIN_IMAGES];
}										t_framebuffer;

typedef struct							s_physical_device
{
	uint32_t					num_families;
	uint32_t					num_formats;
	uint32_t					family_index;
	VkPhysicalDevice			device;
	VkPhysicalDeviceProperties	dev_prop;
	VkQueueFamilyProperties		*q_family_prop;
	VkBool32					*q_supports_present;
	VkSurfaceFormatKHR			*surface_formats;
	VkSurfaceCapabilitiesKHR	surface_cap;
}										t_physical_device;

typedef struct							s_syncronization
{
	VkFence						frame_fences[MAX_SWAPCHAIN_IMAGES];
	VkSemaphore					image_available_sem[MAX_SWAPCHAIN_IMAGES];
	VkSemaphore					render_finished_sem[MAX_SWAPCHAIN_IMAGES];
}										t_syncronization;

typedef struct							s_descriptor
{
	VkDescriptorPool			pool;
	VkDescriptorSetLayout		set_layout;
	VkDescriptorSet				sets[MAX_SWAPCHAIN_IMAGES];
}										t_descriptor;

typedef struct							s_vulkan
{
	VkInstance					inst;
	VkDevice					device;
	VkSurfaceKHR				surface;
	VkSurfaceFormatKHR			surface_format;
	VkSwapchainKHR				swapchain;
	VkPipeline					pipeline;
	VkPipelineLayout			pipeline_layout;
	VkRenderPass				renderpass;
	VkCommandBuffer				command_buffers[MAX_SWAPCHAIN_IMAGES];
	VkCommandPool				commandpool;
	VkQueue						queue;
	t_physical_device			phys_device;
	t_framebuffer				framebuffer;
	t_syncronization			sync;
	uint32_t					compatible_mem_types[VULKAN_MEM_COUNT];
	t_descriptor				descriptor;
	t_buffer					sbo_buffers[MAX_SWAPCHAIN_IMAGES];
}										t_vulkan;

void									vku_create_buffer(t_vulkan *v,
											t_uint32 index,
											VkBufferUsageFlags usage,
											VkMemoryPropertyFlags properties);
void									vku_create_descriptor_pool(t_vulkan *v);
void									vku_create_descriptor_set_layout(
																t_vulkan *v);
void									vku_create_descriptor_sets(t_vulkan *v,
															uint32_t size);
void									vku_destroy_sync_objects(t_vulkan *v);
VkPipelineVertexInputStateCreateInfo	vertex_input_state(void);
VkPipelineRasterizationStateCreateInfo	rasterization_state(void);
VkPipelineMultisampleStateCreateInfo	multisample_state(void);
VkPipelineViewportStateCreateInfo		viewport_state(void);
#endif
