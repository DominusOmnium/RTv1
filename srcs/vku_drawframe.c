/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_drawframe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 17:56:35 by dkathlee          #+#    #+#             */
/*   Updated: 2020/07/24 00:28:32 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

uint32_t bit_ffs32(uint32_t word)
{
    return __builtin_ffs(word) - 1;
}

int		vku_record_cmb(t_vulkan *v)
{
	uint32_t					i;
	VkCommandBufferBeginInfo	begin_info;
	VkBufferImageCopy			region;

	begin_info = (VkCommandBufferBeginInfo){
		.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
		.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT
	};
	region = (VkBufferImageCopy){
		.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
		.imageSubresource.layerCount = 1,
		.imageExtent = (VkExtent3D){WIN_WIDTH, WIN_HEIGHT, 1}
	};
	i = 0;
	while (i < v->sc_image_count)
	{
		if (vkBeginCommandBuffer((v->command_buffers)[i], &begin_info) != VK_SUCCESS)
			return (0);
		vkCmdCopyBufferToImage((v->command_buffers)[i], v->buf.buffer, v->sc_images[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		if (vkEndCommandBuffer((v->command_buffers)[i]) != VK_SUCCESS)
			return (0);
		i++;
	}
	return (1);
}

int		vku_draw_frame(t_vulkan *v)
{
	uint32_t			image_ind;
	VkSubmitInfo		submit_info;
	VkPresentInfoKHR	present_info;
 
    if (vkAcquireNextImageKHR(v->device, v->swapchain, UINT64_MAX, NULL, NULL,
										&image_ind) != VK_SUCCESS)
		return (0);
	submit_info = (VkSubmitInfo){
		.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
		.commandBufferCount = 1,
		.pCommandBuffers = &((v->command_buffers)[image_ind])
	};
	if (vkQueueSubmit(v->queue, 1, &submit_info, NULL) != VK_SUCCESS)
		return (0);
	present_info = (VkPresentInfoKHR){
		.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
		.swapchainCount = 1,
		.pSwapchains = &(v->swapchain),
		.pImageIndices = &image_ind
	};
    if (vkQueuePresentKHR(v->queue, &present_info) != VK_SUCCESS)
		return (0);
	return (1);
}

int createRenderPass(t_vulkan *v)
{
    VkRenderPassCreateInfo renderPassCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = 1,
        .pAttachments = &(VkAttachmentDescription) {
            .format = v->phys_device.surface_formats->format,
            .samples = VK_SAMPLE_COUNT_1_BIT,
            .loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
            .storeOp = VK_ATTACHMENT_STORE_OP_STORE,
            .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
            .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
            .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
            .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        },
        .subpassCount = 1,
        .pSubpasses = &(VkSubpassDescription) {
            .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
            .colorAttachmentCount = 1,
            .pColorAttachments = &(VkAttachmentReference) {
                .attachment = 0,
                .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            },
        }
    };

    VkResult res = vkCreateRenderPass(v->device, &renderPassCreateInfo, 0, &(v->renderpass));
	ft_printf("vkCreateRenderPass: %d\n", res);
    return 1;
}

int createFramebuffers(t_vulkan *v)
{
    for (uint32_t i = 0; i < v->sc_image_count; ++i)
    {
        VkImageViewCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = (v->sc_images)[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = v->phys_device.surface_formats->format,
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1,
            },
        };
        VkResult res = vkCreateImageView(v->device, &createInfo, 0, &((v->sc_image_views)[i]));
		ft_printf("vkCreateImageView: %d\n", res);

        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = v->renderpass,
            .attachmentCount = 1,
            .pAttachments = &((v->sc_image_views)[i]),
            .width = v->phys_device.surface_cap.currentExtent.width,
            .height = v->phys_device.surface_cap.currentExtent.height,
            .layers = 1,
        };
        res = vkCreateFramebuffer(v->device, &framebufferCreateInfo, 0, &((v->frame_buffers)[i]));
		ft_printf("vkCreateFramebuffer: %d\n", res);
    }

    return 1;
}

typedef struct tagVertexP2C
{
    float x, y;
    uint32_t rgba;
} VertexP2C;

int createPipeline(t_vulkan *v)
{
	char *code;
	
	size_t code_len = load_shader_file("shaders/test.spv", &code);
    VkShaderModule fragmentShader = vku_createShaderModule(v, code, code_len);

	code_len = load_shader_file("shaders/testver.spv", &code);
    VkShaderModule vertexShader = vku_createShaderModule(v, code, code_len);

	//load_shader_file("shaders/raytracing.frag", &code);
	//shader_compile(code);

    const VkPipelineShaderStageCreateInfo stages[] = {
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_VERTEX_BIT,
            .module = vertexShader,
            .pName = "main",
        },
        {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
            .stage = VK_SHADER_STAGE_FRAGMENT_BIT,
            .module = fragmentShader,
            .pName = "main",
        }
    };

    VkPipelineVertexInputStateCreateInfo vertexInputState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
        .vertexBindingDescriptionCount = 1,
        .pVertexBindingDescriptions = (VkVertexInputBindingDescription[]) {
            {.binding = 0, .stride = sizeof(VertexP2C), .inputRate = VK_VERTEX_INPUT_RATE_VERTEX}
        },
        .vertexAttributeDescriptionCount = 2,
        .pVertexAttributeDescriptions = (VkVertexInputAttributeDescription[]) {
            {.location = 0, .binding = 0, .format = VK_FORMAT_R32G32_SFLOAT,  .offset = offsetof(VertexP2C, x)},
            {.location = 1, .binding = 0, .format = VK_FORMAT_R8G8B8A8_UNORM, .offset = offsetof(VertexP2C, rgba)}
        },
    };
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };
    VkPipelineRasterizationStateCreateInfo rasterizationState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        .depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,
    };
    VkPipelineViewportStateCreateInfo viewportState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .viewportCount = 1,
        .pViewports = 0,
        .scissorCount = 1,
        .pScissors = 0,
    };
    VkPipelineMultisampleStateCreateInfo multisampleState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
        .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = 0,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,
    };
    VkPipelineColorBlendStateCreateInfo colorBlendState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = (VkPipelineColorBlendAttachmentState[]) {
            {
                .blendEnable = VK_FALSE,
                .srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                .colorBlendOp = VK_BLEND_OP_ADD,
                .srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                .dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                .alphaBlendOp = VK_BLEND_OP_ADD,
                .colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
            },
        },
    };
    VkPipelineDynamicStateCreateInfo dynamicState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = 2,
        .pDynamicStates = (VkDynamicState[]) { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR },
    };

    VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
		.pushConstantRangeCount = 1,
        .pPushConstantRanges = &(VkPushConstantRange) {
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = 20,
        }
    };
    VkResult res = vkCreatePipelineLayout(v->device, &pipelineLayoutCreateInfo, 0, &(v->pipelineLayout));
	ft_printf("vkCreatePipelineLayout: %d\n", res);
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = stages,
        .pVertexInputState = &vertexInputState,
        .pInputAssemblyState = &inputAssemblyState,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizationState,
        .pMultisampleState = &multisampleState ,
        .pColorBlendState = &colorBlendState,
        .pDynamicState = &dynamicState,
        .layout = v->pipelineLayout,
        .renderPass = v->renderpass,
    };

	ft_printf("\n\n\nBefore pipeline creation\n");
	ft_printf("Device: %p\n", v->device);
	ft_printf("Pipeline layout: %p\n", v->pipelineLayout);
	ft_printf("Renderpass: %p\n", v->renderpass);
    res = vkCreateGraphicsPipelines(v->device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, 0, &(v->pipeline));
	ft_printf("After pipeline creation: %d\n", res);
    vkDestroyShaderModule(v->device, fragmentShader, 0);

    return v->pipeline != 0;
}

uint32_t frameIndex = 0;

void draw_frame(t_vulkan *v)
{
    uint32_t index = frameIndex % 2;
    vkWaitForFences(v->device, 1, &frameFences[index], VK_TRUE, UINT64_MAX);
    vkResetFences(v->device, 1, &frameFences[index]);

	size_t uploadOffset = index * UPLOAD_REGION_SIZE;
    size_t uploadLimit = uploadOffset + UPLOAD_REGION_SIZE;
    uint8_t* uploadPtr = (uint8_t*)uploadBufferPtr;

    uint32_t mask = (SDL_GetTicks() >> 3) & 0x1FF;
    mask = mask > 0xFF ? 0x1FF - mask : mask;
    mask = (mask << 16) | (mask << 8) | mask;
    VertexP2C dynamicVertices[] = {
        { -0.5f, -1.0f, 0xFF0000FF|mask },
        {  0.0f,  0.0f, 0xFF00FF00|mask },
        { -1.0f,  0.0f, 0xFFFF0000|mask }
    };
    memcpy(uploadPtr+uploadOffset, dynamicVertices, sizeof(dynamicVertices));
    uploadOffset += sizeof(dynamicVertices);

    VkBufferCopy bufferCopyInfo;
    if (frameIndex == 0)
    {
        VertexP2C staticVertices[] = {
            { 0.5f, 0.0f, 0xFF0000FF },
            { 1.0f, 1.0f, 0xFF00FF00 },
            { 0.0f, 1.0f, 0xFFFF0000 }
        };
        memcpy(uploadPtr + uploadOffset, staticVertices, sizeof(staticVertices));
        bufferCopyInfo = (VkBufferCopy){
            .srcOffset = uploadOffset,
            .dstOffset = 0,
            .size = sizeof(staticVertices),
        };
    }

    uint32_t imageIndex;
    vkAcquireNextImageKHR(v->device, v->swapchain, UINT64_MAX, imageAvailableSemaphores[index], VK_NULL_HANDLE, &imageIndex);

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    vkBeginCommandBuffer((v->command_buffers)[index], &beginInfo);

	if (frameIndex == 0)
    {
        vkCmdCopyBuffer((v->command_buffers)[index], uploadBuffer, staticBuffer, 1, &bufferCopyInfo);
        vkCmdPipelineBarrier((v->command_buffers)[index],
            VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_VERTEX_INPUT_BIT, 0, 
            1, &(VkMemoryBarrier){
                .sType = VK_STRUCTURE_TYPE_MEMORY_BARRIER,
                .srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT,
                .dstAccessMask = VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT
            },
            0, NULL, 0, NULL
        );
    }

    vkCmdBeginRenderPass((v->command_buffers)[index],
        &(VkRenderPassBeginInfo) {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = v->renderpass,
        .framebuffer = (v->frame_buffers)[imageIndex],
        .clearValueCount = 1,
        .pClearValues = &(VkClearValue) { 0.0f, 0.1f, 0.2f, 1.0f },
        .renderArea.offset = (VkOffset2D) { .x = 0,.y = 0 },
        .renderArea.extent = v->phys_device.surface_cap.currentExtent,
        },
        VK_SUBPASS_CONTENTS_INLINE
    );

	static int px = 0, py = 0;
    int x = 0, y = 0;
    if (SDL_GetRelativeMouseState(&x, &y) & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
        px += x; py += y;
        px = px < 0 ? 0 : px; px = px > (int)v->phys_device.surface_cap.currentExtent.width ? v->phys_device.surface_cap.currentExtent.width : px;
        py = py < 0 ? 0 : py; py = py >(int)v->phys_device.surface_cap.currentExtent.height ? v->phys_device.surface_cap.currentExtent.height : py;
    }

    float fragmentConstants[5] = { (float)v->phys_device.surface_cap.currentExtent.width, (float)v->phys_device.surface_cap.currentExtent.height, (float)px, (float)py, SDL_GetTicks() / 1000.0f };

    vkCmdPushConstants((v->command_buffers)[index], v->pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(fragmentConstants), fragmentConstants);
	
    vkCmdSetViewport((v->command_buffers)[index], 0, 1, &(VkViewport){ 0.0f, 0.0f, (float)v->phys_device.surface_cap.currentExtent.width, (float)v->phys_device.surface_cap.currentExtent.height, 0.0f, 1.0f});
    vkCmdSetScissor((v->command_buffers)[index], 0, 1, &(VkRect2D){ {0, 0}, v->phys_device.surface_cap.currentExtent});
	vkCmdBindPipeline((v->command_buffers)[index], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipeline);
    
	vkCmdBindVertexBuffers((v->command_buffers)[index], 0, 1, &uploadBuffer, (VkDeviceSize[]) { 0 });
    vkCmdDraw((v->command_buffers)[index], 3, 1, 0, 0);

    vkCmdBindVertexBuffers((v->command_buffers)[index], 0, 1, &staticBuffer, (VkDeviceSize[]) { 0 });
    vkCmdDraw((v->command_buffers)[index], 3, 1, 0, 0);
	
    vkCmdEndRenderPass((v->command_buffers)[index]);

    vkEndCommandBuffer((v->command_buffers)[index]);

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
		.pWaitSemaphores = &imageAvailableSemaphores[index],
        .pWaitDstStageMask = (VkPipelineStageFlags[]) { VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT },
        .commandBufferCount = 1,
        .pCommandBuffers = &((v->command_buffers)[index]),
        .signalSemaphoreCount = 1,
		.pSignalSemaphores = &renderFinishedSemaphores[index]
    };
    vkQueueSubmit(v->queue, 1, &submitInfo, frameFences[index]);

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &renderFinishedSemaphores[index],
        .swapchainCount = 1,
        .pSwapchains = &(v->swapchain),
        .pImageIndices = &imageIndex,
    };
    vkQueuePresentKHR(v->queue, &presentInfo);

    ++frameIndex;
}

int createUploadBuffer(t_vulkan *v)
{
    VkBufferCreateInfo bufferCreateInfo;
    VkMemoryAllocateInfo allocInfo;
    VkMemoryRequirements memoryRequirements;
    uint32_t memoryIndex;

    bufferCreateInfo = (VkBufferCreateInfo){
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = UPLOAD_BUFFER_SIZE,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
                | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
                | VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = &(v->family_index),
    };
    VkResult res = vkCreateBuffer(v->device, &bufferCreateInfo, NULL, &uploadBuffer);
	printf("vkCreateBuffer: %d\n", res);
    vkGetBufferMemoryRequirements(v->device, uploadBuffer, &memoryRequirements);
	printf("vkGetBufferMemoryRequirements: %d\n", res);

    memoryIndex = bit_ffs32(compatibleMemTypes[VULKAN_MEM_DEVICE_UPLOAD] & memoryRequirements.memoryTypeBits);
    allocInfo = (VkMemoryAllocateInfo){
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = memoryIndex,
    };
    res = vkAllocateMemory(v->device, &allocInfo, NULL, &uploadBufferMemory);
	printf("vkAllocateMemory: %d\n", res);
    vkBindBufferMemory(v->device, uploadBuffer, uploadBufferMemory, 0);
    vkMapMemory(v->device, uploadBufferMemory, 0, VK_WHOLE_SIZE, 0, &uploadBufferPtr);

    bufferCreateInfo = (VkBufferCreateInfo){
        .sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
        .size = STATIC_BUFFER_SIZE,
        .usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT
        | VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT | VK_BUFFER_USAGE_STORAGE_BUFFER_BIT
        | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .queueFamilyIndexCount = 1,
        .pQueueFamilyIndices = &(v->family_index),
    };
    vkCreateBuffer(v->device, &bufferCreateInfo, NULL, &staticBuffer);
    vkGetBufferMemoryRequirements(v->device, staticBuffer, &memoryRequirements);

    memoryIndex = bit_ffs32(compatibleMemTypes[VULKAN_MEM_DEVICE_LOCAL] & memoryRequirements.memoryTypeBits);
    allocInfo = (VkMemoryAllocateInfo){
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = memoryIndex,
    };
    vkAllocateMemory(v->device, &allocInfo, NULL, &staticBufferMemory);
    vkBindBufferMemory(v->device, staticBuffer, staticBufferMemory, 0);

    return 1;
}

int init_render(t_vulkan *v)
{
    VkCommandPoolCreateInfo commandPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = v->family_index,
    };

    VkResult res = vkCreateCommandPool(v->device, &commandPoolCreateInfo, 0, &(v->commandpool));

	ft_printf("vkCreateCommandPool: %d\n", res);
    VkCommandBufferAllocateInfo commandBufferAllocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = v->commandpool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = 2,            ///////////////////////////////////////////
    };

	v->command_buffers = (VkCommandBuffer*)ft_memalloc(sizeof(VkCommandBuffer) * v->sc_image_count);
    res = vkAllocateCommandBuffers(v->device, &commandBufferAllocInfo, v->command_buffers);
	ft_printf("vkAllocateCommandBuffers: %d\n", res);

	    VkSemaphoreCreateInfo semaphoreCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &imageAvailableSemaphores[0]);
    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &imageAvailableSemaphores[1]);
    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &renderFinishedSemaphores[0]);
    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &renderFinishedSemaphores[1]);

    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    vkCreateFence(v->device, &fenceCreateInfo, 0, &frameFences[0]);
    vkCreateFence(v->device, &fenceCreateInfo, 0, &frameFences[1]);

    createRenderPass(v);
    createFramebuffers(v);
    createPipeline(v);
	createUploadBuffer(v);

    return 1;
}
