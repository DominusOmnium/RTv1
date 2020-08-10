/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_drawframe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 17:56:35 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/10 19:03:07 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

uint32_t bit_ffs32(uint32_t word)
{
    return __builtin_ffs(word) - 1;
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

	VkSubpassDependency dependency;
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &dependency;

    VkResult res = vkCreateRenderPass(v->device, &renderPassCreateInfo, 0, &(v->renderpass));
    return 1;
}

int createFramebuffers(t_vulkan *v)
{
    for (uint32_t i = 0; i < v->framebuffer.sc_image_count; ++i)
    {
        VkImageViewCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = (v->framebuffer.sc_images)[i],
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
        VkResult res = vkCreateImageView(v->device, &createInfo, 0, &((v->framebuffer.sc_image_views)[i]));

        VkFramebufferCreateInfo framebufferCreateInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = v->renderpass,
            .attachmentCount = 1,
            .pAttachments = &((v->framebuffer.sc_image_views)[i]),
            .width = v->phys_device.surface_cap.currentExtent.width,
            .height = v->phys_device.surface_cap.currentExtent.height,
            .layers = 1,
        };
        res = vkCreateFramebuffer(v->device, &framebufferCreateInfo, 0, &((v->framebuffer.frame_buffers)[i]));
    }

    return 1;
}

typedef struct tagVertexP2C
{
    float x, y;
    uint32_t rgba;
} VertexP2C;

int createPipeline(t_vulkan *v, t_retr *r)
{
	char *code;
	
	size_t code_len = load_shader_file("shaders/rtv1-frag.spv", &code);
    VkShaderModule fragmentShader = vku_createShaderModule(v, code, code_len);

	code_len = load_shader_file("shaders/testver.spv", &code);
    VkShaderModule vertexShader = vku_createShaderModule(v, code, code_len);

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

    /*VkPipelineVertexInputStateCreateInfo vertexInputState = {
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
    };*/
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
        .topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
        .primitiveRestartEnable = VK_FALSE,
    };
	
    VkPipelineRasterizationStateCreateInfo rasterizationState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        /*.depthClampEnable = VK_FALSE,
        .rasterizerDiscardEnable = VK_FALSE,
        .polygonMode = VK_POLYGON_MODE_FILL,
        .cullMode = VK_CULL_MODE_BACK_BIT,
        .frontFace = VK_FRONT_FACE_CLOCKWISE,
        .depthBiasEnable = VK_FALSE,
        .depthBiasConstantFactor = 0.0f,
        .depthBiasClamp = 0.0f,
        .depthBiasSlopeFactor = 0.0f,
        .lineWidth = 1.0f,*/
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
        /*.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
        .sampleShadingEnable = VK_FALSE,
        .minSampleShading = 1.0f,
        .pSampleMask = 0,
        .alphaToCoverageEnable = VK_FALSE,
        .alphaToOneEnable = VK_FALSE,*/
    };
    VkPipelineColorBlendStateCreateInfo colorBlendState = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
        .logicOpEnable = VK_FALSE,
        .attachmentCount = 1,
        .pAttachments = (VkPipelineColorBlendAttachmentState[]) {
            {
                //.blendEnable = VK_FALSE,
                //.srcColorBlendFactor = VK_BLEND_FACTOR_ONE,
                //.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO,
                //.colorBlendOp = VK_BLEND_OP_ADD,
                //.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
                //.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO,
                //.alphaBlendOp = VK_BLEND_OP_ADD,
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
		.setLayoutCount = 1,
		.pSetLayouts = &(v->descriptor.set_layout),
        .pPushConstantRanges = &(VkPushConstantRange) {
            .stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT,
            .offset = 0,
            .size = 32,
        }
    };
    VkResult res = vkCreatePipelineLayout(v->device, &pipelineLayoutCreateInfo, 0, &(v->pipelineLayout));
	ft_printf("vkCreatePipelineLayout: %d\n", res);
    VkGraphicsPipelineCreateInfo pipelineCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
        .stageCount = 2,
        .pStages = stages,
        //.pVertexInputState = &vertexInputState,
        .pInputAssemblyState = &inputAssemblyState,
        .pViewportState = &viewportState,
        .pRasterizationState = &rasterizationState,
        .pMultisampleState = &multisampleState ,
        .pColorBlendState = &colorBlendState,
        .pDynamicState = &dynamicState,
        .layout = v->pipelineLayout,
        .renderPass = v->renderpass,
    };

    res = vkCreateGraphicsPipelines(v->device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, 0, &(v->pipeline));
	ft_printf("vkCreateGraphicsPipelines: %d\n", res);
    vkDestroyShaderModule(v->device, fragmentShader, 0);
    vkDestroyShaderModule(v->device, vertexShader, 0);

    return v->pipeline != 0;
}

uint32_t frameIndex = 0;

void draw_frame(t_vulkan *v, t_retr *r)
{
    uint32_t index = frameIndex % FRAME_COUNT;
    vkWaitForFences(v->device, 1, &(v->sync.frame_fences[index]), VK_TRUE, UINT64_MAX);
    vkResetFences(v->device, 1, &(v->sync.frame_fences[index]));

	/*size_t uploadOffset = index * UPLOAD_REGION_SIZE;
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
    }*/

    uint32_t imageIndex;
    vkAcquireNextImageKHR(v->device, v->swapchain, UINT64_MAX, v->sync.image_available_semaphores[index], VK_NULL_HANDLE, &imageIndex);

    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT
    };
    vkBeginCommandBuffer((v->command_buffers)[index], &beginInfo);

	/*if (frameIndex == 0)
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
    }*/

	

    vkCmdBeginRenderPass((v->command_buffers)[index],
        &(VkRenderPassBeginInfo) {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = v->renderpass,
        .framebuffer = (v->framebuffer.frame_buffers)[imageIndex],
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

    float fragmentConstants[10] = { (float)r->camera.position.x, (float)r->camera.position.y, (float)r->camera.position.z,
									(float)v->phys_device.surface_cap.currentExtent.width,
									(float)r->camera.rotation.x, (float)r->camera.rotation.y, (float)r->camera.rotation.z,
									(float)v->phys_device.surface_cap.currentExtent.height, r->d, (float)r->n_fig};

	vkCmdPushConstants((v->command_buffers)[index], v->pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(fragmentConstants), fragmentConstants);
	//vkCmdPushConstants((v->command_buffers)[index], v->pipelineLayout, VK_SHADER_STAGE_FRAGMENT_BIT, sizeof(fragmentConstants), r->n_fig * sizeof(t_figure), r->figures);
	
    vkCmdSetViewport((v->command_buffers)[index], 0, 1, &(VkViewport){ 0.0f, 0.0f, (float)v->phys_device.surface_cap.currentExtent.width, (float)v->phys_device.surface_cap.currentExtent.height, 0.0f, 1.0f});
    vkCmdSetScissor((v->command_buffers)[index], 0, 1, &(VkRect2D){ {0, 0}, v->phys_device.surface_cap.currentExtent});
	vkCmdBindPipeline((v->command_buffers)[index], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipeline);
    
	//vkCmdBindVertexBuffers((v->command_buffers)[index], 0, 1, &uploadBuffer, (VkDeviceSize[]) { 0 });
    //vkCmdDraw((v->command_buffers)[index], 3, 1, 0, 0);

    //vkCmdBindVertexBuffers((v->command_buffers)[index], 0, 1, &staticBuffer, (VkDeviceSize[]) { 0 });
    vkCmdDraw((v->command_buffers)[index], 3, 1, 0, 0);
	
    vkCmdEndRenderPass((v->command_buffers)[index]);

    vkEndCommandBuffer((v->command_buffers)[index]);

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
		.pWaitSemaphores = &(v->sync.image_available_semaphores[index]),
        .pWaitDstStageMask = (VkPipelineStageFlags[]) { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT },
        .commandBufferCount = 1,
        .pCommandBuffers = &((v->command_buffers)[index]),
        .signalSemaphoreCount = 1,
		.pSignalSemaphores = &(v->sync.render_finished_semaphores[index])
    };
    vkQueueSubmit(v->queue, 1, &submitInfo, v->sync.frame_fences[index]);

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &(v->sync.render_finished_semaphores[index]),
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
        .pQueueFamilyIndices = &(v->phys_device.family_index),
    };
    VkResult res = vkCreateBuffer(v->device, &bufferCreateInfo, NULL, &uploadBuffer);
    vkGetBufferMemoryRequirements(v->device, uploadBuffer, &memoryRequirements);

    memoryIndex = bit_ffs32(v->compatible_mem_types[VULKAN_MEM_DEVICE_UPLOAD] & memoryRequirements.memoryTypeBits);
    allocInfo = (VkMemoryAllocateInfo){
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = memoryIndex,
    };
    res = vkAllocateMemory(v->device, &allocInfo, NULL, &uploadBufferMemory);
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
        .pQueueFamilyIndices = &(v->phys_device.family_index),
    };
    vkCreateBuffer(v->device, &bufferCreateInfo, NULL, &staticBuffer);
    vkGetBufferMemoryRequirements(v->device, staticBuffer, &memoryRequirements);

    memoryIndex = bit_ffs32(v->compatible_mem_types[VULKAN_MEM_DEVICE_LOCAL] & memoryRequirements.memoryTypeBits);
    allocInfo = (VkMemoryAllocateInfo){
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = memoryIndex,
    };
    vkAllocateMemory(v->device, &allocInfo, NULL, &staticBufferMemory);
    vkBindBufferMemory(v->device, staticBuffer, staticBufferMemory, 0);

    return 1;
}

void createDescriptorPool(t_vulkan *v)
{
	VkDescriptorPoolSize poolSizes;
	poolSizes.type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	poolSizes.descriptorCount = FRAME_COUNT;

	VkDescriptorPoolCreateInfo poolInfo = {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSizes;
	poolInfo.maxSets = FRAME_COUNT;

	if (vkCreateDescriptorPool(v->device, &poolInfo, NULL, &v->descriptor.pool) != VK_SUCCESS)
		handle_error("Descriptor pool creation error!");
}

void createDescriptorSetLayout(t_vulkan *v)
{
	VkDescriptorSetLayoutBinding sboLayoutBinding = {};
	sboLayoutBinding.binding = 0;
	sboLayoutBinding.descriptorCount = 1;
	sboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
	sboLayoutBinding.pImmutableSamplers = NULL;
	sboLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

	/*VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
	samplerLayoutBinding.binding = 1;
	samplerLayoutBinding.descriptorCount = 1;
	samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
	samplerLayoutBinding.pImmutableSamplers = NULL;
	samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;*/

	VkDescriptorSetLayoutCreateInfo layoutInfo = {};
	layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutInfo.bindingCount = 1;
	layoutInfo.pBindings = &sboLayoutBinding;

	if (vkCreateDescriptorSetLayout(v->device, &layoutInfo, NULL, &(v->descriptor.set_layout)) != VK_SUCCESS)
		handle_error("Descriptor set layout creation error!");
}

void createDescriptorSets(t_vulkan *v, uint32_t size)
{
	size_t	i;
	
	VkDescriptorSetLayout layouts[] = {v->descriptor.set_layout, v->descriptor.set_layout};
	VkDescriptorSetAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	allocInfo.descriptorPool = v->descriptor.pool;
	allocInfo.descriptorSetCount = FRAME_COUNT;
	allocInfo.pSetLayouts = layouts;

	if (vkAllocateDescriptorSets(v->device, &allocInfo, v->descriptor.sets) != VK_SUCCESS)
		handle_error("Descriptor sets allocation error!");

	i = 0;
	ft_printf("\t1\n");
	while (i < FRAME_COUNT)
	{
		VkDescriptorBufferInfo bufferInfo = {};
		bufferInfo.buffer = (v->sbo_buffers)[i].buffer;
		bufferInfo.offset = 0;
		bufferInfo.range = size;

		VkWriteDescriptorSet	descriptorWrite = {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = (v->descriptor.sets)[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		ft_printf("\t2\n");
		vkUpdateDescriptorSets(v->device, 1, &descriptorWrite, 0, NULL);
		ft_printf("\t3\n");
		i++;
	}
}

void createBuffers(t_vulkan *v, VkDeviceSize size)
{
	t_uint32	i;

	i = 0;
	ft_printf("\t1\n");
	while (i < FRAME_COUNT)
	{
		(v->sbo_buffers)[i].buf_size = size;
		ft_printf("\t2\n");
		vku_create_buffer(v, i, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
		ft_printf("\t3\n");
		i++;
	}
}

void createCommandBuffers(t_vulkan *v)
{
	t_uint32	i;
	VkCommandBufferAllocateInfo allocInfo;
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = v->commandpool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = FRAME_COUNT;

	if (vkAllocateCommandBuffers(v->device, &allocInfo, v->command_buffers) != VK_SUCCESS)
		handle_error("Command Buffers Allocation error!");

	ft_printf("\t1\n");
	i = 0;
	while (i < FRAME_COUNT)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		ft_printf("\t\t1\n");
		if (vkBeginCommandBuffer(v->command_buffers[i], &beginInfo) != VK_SUCCESS)
			handle_error("Begin Command Buffer error!");

		VkRenderPassBeginInfo renderPassInfo = (VkRenderPassBeginInfo) {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = v->renderpass,
        .framebuffer = (v->framebuffer.frame_buffers)[i],
        .clearValueCount = 1,
        .pClearValues = &(VkClearValue) { 0.0f, 0.1f, 0.2f, 1.0f },
        .renderArea.offset = (VkOffset2D) { .x = 0,.y = 0 },
        .renderArea.extent = v->phys_device.surface_cap.currentExtent,
		};

		ft_printf("\t\t2\n");
		vkCmdBeginRenderPass(v->command_buffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			vkCmdSetViewport((v->command_buffers)[i], 0, 1, &(VkViewport){ 0.0f, 0.0f, (float)v->phys_device.surface_cap.currentExtent.width, (float)v->phys_device.surface_cap.currentExtent.height, 0.0f, 1.0f});
    		vkCmdSetScissor((v->command_buffers)[i], 0, 1, &(VkRect2D){ {0, 0}, v->phys_device.surface_cap.currentExtent});
	
			vkCmdBindPipeline(v->command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipeline);

			/*VkBuffer vertexBuffers[] = {vertexBuffer};
			VkDeviceSize offsets[] = {0};
			vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);

			vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);*/

			vkCmdBindDescriptorSets(v->command_buffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, v->pipelineLayout, 0, 1, &(v->descriptor.sets[i]), 0, NULL);
			
    		vkCmdDraw(v->command_buffers[i], 3, 1, 0, 0);
			//vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

		vkCmdEndRenderPass(v->command_buffers[i]);

		ft_printf("\t\t3\n");
		if (vkEndCommandBuffer(v->command_buffers[i]) != VK_SUCCESS)
			handle_error("End Command Buffer error!");
		ft_printf("\t\t4\n");
		i++;
	}
}

int init_render(t_vulkan *v, t_retr *r)
{
    VkCommandPoolCreateInfo commandPoolCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        //.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = v->phys_device.family_index,
    };

    VkResult res = vkCreateCommandPool(v->device, &commandPoolCreateInfo, 0, &(v->commandpool));

    VkCommandBufferAllocateInfo commandBufferAllocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = v->commandpool,
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,
        .commandBufferCount = FRAME_COUNT
    };

    res = vkAllocateCommandBuffers(v->device, &commandBufferAllocInfo, v->command_buffers);

	VkSemaphoreCreateInfo semaphoreCreateInfo = { VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO };

    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &(v->sync.image_available_semaphores[0]));
    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &(v->sync.image_available_semaphores[1]));
    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &(v->sync.render_finished_semaphores[0]));
    vkCreateSemaphore(v->device, &semaphoreCreateInfo, 0, &(v->sync.render_finished_semaphores[1]));

    VkFenceCreateInfo fenceCreateInfo = {
        .sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,
        .flags = VK_FENCE_CREATE_SIGNALED_BIT
    };

    vkCreateFence(v->device, &fenceCreateInfo, 0, &(v->sync.frame_fences[0]));
    vkCreateFence(v->device, &fenceCreateInfo, 0, &(v->sync.frame_fences[1]));

    createRenderPass(v);
	createDescriptorSetLayout(v);
    createPipeline(v, r);
	ft_printf("1\n");
    createFramebuffers(v);
	ft_printf("2\n");
	createBuffers(v, STATIC_BUFFER_SIZE);
	ft_printf("3\n");
	createDescriptorPool(v);
	ft_printf("4\n");
	createDescriptorSets(v, STATIC_BUFFER_SIZE);
	ft_printf("5\n");
	createCommandBuffers(v);
	ft_printf("6\n");
	//createUploadBuffer(v);

    return 1;
}

void updateUniformBuffer(t_vulkan *v, uint32_t index)
{
	void* data;
	
	vkMapMemory(v->device, v->sbo_buffers[index].dev_mem, 0, STATIC_BUFFER_SIZE, 0, &data);
	memcpy(data, &q, sizeof(q));
	vkUnmapMemory(v->device, v->sbo_buffers[index].dev_mem);
}

void draw_frame1(t_vulkan *v, t_retr *r)
{
    uint32_t index = frameIndex % FRAME_COUNT;
    vkWaitForFences(v->device, 1, &(v->sync.frame_fences[index]), VK_TRUE, UINT64_MAX);
    vkResetFences(v->device, 1, &(v->sync.frame_fences[index]));

	uint32_t imageIndex;
    vkAcquireNextImageKHR(v->device, v->swapchain, UINT64_MAX, v->sync.image_available_semaphores[index], VK_NULL_HANDLE, &imageIndex);

	/*if (result == VK_ERROR_OUT_OF_DATE_KHR) {
		recreateSwapChain();
		return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}*/

	updateUniformBuffer(v, imageIndex);

	VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = 1,
		.pWaitSemaphores = &(v->sync.image_available_semaphores[index]),
        .pWaitDstStageMask = (VkPipelineStageFlags[]) { VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT },
        .commandBufferCount = 1,
        .pCommandBuffers = &((v->command_buffers)[index]),
        .signalSemaphoreCount = 1,
		.pSignalSemaphores = &(v->sync.render_finished_semaphores[index])
    };
    vkQueueSubmit(v->queue, 1, &submitInfo, v->sync.frame_fences[index]);

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = 1,
        .pWaitSemaphores = &(v->sync.render_finished_semaphores[index]),
        .swapchainCount = 1,
        .pSwapchains = &(v->swapchain),
        .pImageIndices = &imageIndex,
    };
    vkQueuePresentKHR(v->queue, &presentInfo);

    ++frameIndex;

	/*if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
		vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
	}
	imagesInFlight[imageIndex] = inFlightFences[currentFrame];*/

	/*VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	vkResetFences(device, 1, &inFlightFences[currentFrame]);

	if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = {swapChain};
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;

	presentInfo.pImageIndices = &imageIndex;

	result = vkQueuePresentKHR(presentQueue, &presentInfo);

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
		framebufferResized = false;
		recreateSwapChain();
	} else if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}

	currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;*/
}
