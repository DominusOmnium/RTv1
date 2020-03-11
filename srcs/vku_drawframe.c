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
		VkBufferImageCopy region = {};
		region.bufferOffset = 0;
		region.bufferRowLength = 0;
		region.bufferImageHeight = 0;
		region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		region.imageSubresource.mipLevel = 0;
		region.imageSubresource.baseArrayLayer = 0;
		region.imageSubresource.layerCount = 1;
		region.imageOffset = (VkOffset3D){0, 0, 0};
		region.imageExtent = (VkExtent3D){
			WIN_WIDTH,
			WIN_HEIGHT,
			1
		};
		vkCmdCopyBufferToImage((v->commandBuffers)[i], v->buf.buffer, v->swapchainImages[i], VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);
		vkEndCommandBuffer((v->commandBuffers)[i]);
		i++;
	}
}

void	vku_draw_frame(t_vulkan *v)
{
	uint32_t ImageIndex = 0;
 
    vkAcquireNextImageKHR(v->device, v->swapchain, UINT64_MAX, NULL, NULL, &ImageIndex);
	VkSubmitInfo submitInfo = {};
	submitInfo.sType                = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount   = 1;
	submitInfo.pCommandBuffers      = &((v->commandBuffers)[ImageIndex]);
	vkQueueSubmit(v->queue, 1, &submitInfo, NULL);
	VkPresentInfoKHR presentInfo = {};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = &v->swapchain;
    presentInfo.pImageIndices      = &ImageIndex;
    vkQueuePresentKHR(v->queue, &presentInfo);
}
