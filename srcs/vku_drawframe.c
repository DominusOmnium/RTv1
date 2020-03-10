#include "rtv1.h"

void	vku_record_cmb(t_vulkan *v)
{
	uint32_t	i;

	VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
	VkClearColorValue clearColor = { 164.0f/256.0f, 30.0f/256.0f, 34.0f/256.0f, 0.0f };
	/*VkClearValue clearValue = {};
	clearValue.color = clearColor;*/
	VkImageSubresourceRange imageRange = {};
	imageRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	imageRange.levelCount = 1;
	imageRange.layerCount = 1;
	i = 0;
	while (i < v->swapchainImageCount)
	{
		vkBeginCommandBuffer((v->commandBuffers)[i], &beginInfo);
        vkCmdClearColorImage((v->commandBuffers)[i], (v->swapchainImages)[i], VK_IMAGE_LAYOUT_GENERAL, &clearColor, 1, &imageRange);
        vkEndCommandBuffer((v->commandBuffers)[i]);
		i++;
	}
}

void	vku_draw_frame(t_vulkan *v)
{
	uint32_t ImageIndex = 0;
 
    VkResult res = vkAcquireNextImageKHR(v->device, v->swapchain, UINT64_MAX, NULL, NULL, &ImageIndex);
	VkSubmitInfo submitInfo = {};
	submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount   = 1;
	submitInfo.pCommandBuffers      = &((v->commandBuffers)[ImageIndex]);
	res = vkQueueSubmit(v->queue, 1, &submitInfo, NULL);
	VkPresentInfoKHR presentInfo = {};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = &v->swapchain;
    presentInfo.pImageIndices      = &ImageIndex;
 
    res = vkQueuePresentKHR(v->queue, &presentInfo);
}
