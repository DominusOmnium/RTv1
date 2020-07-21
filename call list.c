vkCreateInstance(&createInfo, 0, &instance);

SDL_Window* window = SDL_CreateWindow(
        "Vulkan Sample",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        0
    );

SDL_GetWindowWMInfo(window, &info);

vkCreateWin32SurfaceKHR(instance, &surfaceCreateInfo, NULL, &surface);

vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, 0);
vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, deviceHandles);

vkGetPhysicalDeviceQueueFamilyProperties(deviceHandles[i], &queueFamilyCount, NULL);
vkGetPhysicalDeviceQueueFamilyProperties(deviceHandles[i], &queueFamilyCount, queueFamilyProperties);

vkGetPhysicalDeviceSurfaceSupportKHR(deviceHandles[i], j, surface, &supportsPresent);

vkCreateDevice(physicalDevice, &deviceCreateInfo, NULL, &device);

vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, 0);
vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, &surfaceFormat);

vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, NULL);
vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes);

vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

vkCreateSwapchainKHR(device, &swapChainCreateInfo, 0, &swapchain);

vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, NULL);
vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, swapchainImages);

vkCreateCommandPool(device, &commandPoolCreateInfo, 0, &commandPool);

vkAllocateCommandBuffers(device, &commandBufferAllocInfo, commandBuffers);

vkCreateSemaphore(device, &semaphoreCreateInfo, 0, &imageAvailableSemaphores[0]);
vkCreateSemaphore(device, &semaphoreCreateInfo, 0, &imageAvailableSemaphores[1]);
vkCreateSemaphore(device, &semaphoreCreateInfo, 0, &renderFinishedSemaphores[0]);
vkCreateSemaphore(device, &semaphoreCreateInfo, 0, &renderFinishedSemaphores[1]);

vkCreateFence(device, &fenceCreateInfo, 0, &frameFences[0]);
vkCreateFence(device, &fenceCreateInfo, 0, &frameFences[1]);

vkCreateRenderPass(device, &renderPassCreateInfo, 0, &renderPass);

vkCreateImageView(device, &createInfo, 0, &swapchainImageViews[i]);
vkCreateFramebuffer(device, &framebufferCreateInfo, 0, &framebuffers[i]);

vkCreateShaderModule(device, &shaderModuleCreateInfo, 0, &shaderModule);
