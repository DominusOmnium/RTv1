#include "rtv1.h"



int	main()
{
	const char *extensionNames[2];
	VkInstance inst;
	SDL_Window *window;
	VkSurfaceKHR surf;

	SDL_Init(SDL_INIT_VIDEO);

	extensionNames[0] = VK_KHR_SURFACE_EXTENSION_NAME;
	//extensionNames[1] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
	VkInstanceCreateInfo instanceCreateInfo = {
	VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
	NULL,
	0,
	NULL,
	0,
	NULL,
	1,
	extensionNames,
	};
	vkCreateInstance(&instanceCreateInfo, NULL, &inst);

	int					error;
	VkPhysicalDevice	phys[4];
	uint32_t			physCount = 4;
	vkEnumeratePhysicalDevices(inst, &physCount, phys);

	VkDeviceCreateInfo	devCreateInfo = { 0 };
	VkDevice			dev;
	vkCreateDevice(phys[0], &devCreateInfo, NULL, &dev);

	//#ifdef VK_USE_PLATFORM_MACOS_MVK
	window = SDL_CreateWindow("My App", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_VULKAN);
	SDL_Vulkan_CreateSurface(window, inst, &surf);
	/*VkMacOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_METAL_SURFACE_CREATE_INFO_EXT;
    surfaceCreateInfo.pView = window;
    VkResult res = vkCreateMacOSSurfaceMVK(inst, &surfaceCreateInfo, NULL, &surf);*/
	/*#else
	xcb_connection_t*	m_pXCBConn;
    xcb_screen_t*		m_pXCBScreen;
    xcb_window_t		m_xcbWindow;

	m_pXCBConn = xcb_connect(NULL, NULL);
	if (error = xcb_connection_has_error(m_pXCBConn))
	{
		printf("Error opening xcb connection error %d\n", error);
        return (0);
	}
	const xcb_setup_t *pSetup = xcb_get_setup(m_pXCBConn);
	xcb_screen_iterator_t iter = xcb_setup_roots_iterator(pSetup);
	m_pXCBScreen = iter.data;
	m_xcbWindow = xcb_generate_id(m_pXCBConn);
	xcb_create_window(m_pXCBConn,             					// соединение с сервером XWindow
						XCB_COPY_FROM_PARENT,                   // глубина цвета
						m_xcbWindow,                            // XID нового окна
						m_pXCBScreen->root,                     // родительское окно
						0,                                      // координата X
						0,                                      // координата Y
						WIN_WIDTH,                              // ширина окна
						WIN_HEIGHT,                             // высота окна
						0,                                      // ширина границы
						XCB_WINDOW_CLASS_INPUT_OUTPUT,          // класс окна, не смог найти документации
						m_pXCBScreen->root_visual,              // определяет отображения цвета
						0,
						0);
	xcb_map_window(m_pXCBConn, m_xcbWindow);
    xcb_flush(m_pXCBConn);
 
	VkXcbSurfaceCreateInfoKHR surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.connection = m_pXCBConn;
    surfaceCreateInfo.window = m_xcbWindow;
    VkResult res = vkCreateXcbSurfaceKHR(inst, &surfaceCreateInfo, NULL, &surf);
	#endif*/
	VkImage images[4];
	uint32_t swapCount;
	VkSwapchainKHR swap;
	uint32_t currentSwapImage;
	vkGetSwapchainImagesKHR(dev, swap, &swapCount, images);
	vkAcquireNextImageKHR(dev, swap, UINT64_MAX, VK_NULL_HANDLE, NULL, &currentSwapImage);



	getchar();
	return (0);
}

