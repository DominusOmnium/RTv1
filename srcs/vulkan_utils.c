#include "rtv1.h"

void	vkl_create_instance(t_vulkan *v)
{
	VkApplicationInfo		appInfo;
	VkInstanceCreateInfo	instInfo;
	VkResult				res;
	const char				*inst_ext[2];

	appInfo = (VkApplicationInfo){};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = v->appname;
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
	inst_ext[0] = VK_KHR_SURFACE_EXTENSION_NAME;
	//inst_ext[1] = VK_KHR_XCB_SURFACE_EXTENSION_NAME;
	instInfo.enabledExtensionCount = ARRAY_SIZE_IN_ELEMENTS(inst_ext);
    instInfo.ppEnabledExtensionNames = inst_ext;
	res = vkCreateInstance(&instInfo, NULL, &(v->inst));
    CHECK_VULKAN_ERROR("vkCreateInstance %d\n", res);
}

void	vkl_get_physical_devices(const VkInstance *inst, const VkSurfaceKHR *surf, t_physical_devices *phys_devices)
{
	u_int32_t	num_devices;
	u_int32_t	numq_family;
	u_int32_t	num_formats;
    VkResult	res;
	int			i;
	int			j;

	res = vkEnumeratePhysicalDevices(inst, &num_devices, NULL);
    CHECK_VULKAN_ERROR("vkEnumeratePhysicalDevices error %d\n", res);
	phys_devices->devices = malloc(sizeof(VkPhysicalDevice) * num_devices);
	phys_devices->dev_prop = malloc(sizeof(VkPhysicalDeviceProperties) * num_devices);
	phys_devices->q_family_prop = malloc(sizeof(VkQueueFamilyProperties*) * num_devices);
	phys_devices->q_supports_present = malloc(sizeof(VkBool32*) * num_devices);
	phys_devices->surface_caps = malloc(sizeof(VkSurfaceFormatKHR*) * num_devices);
	phys_devices->surface_formats = malloc(sizeof(VkSurfaceCapabilitiesKHR) * num_devices);
	res = vkEnumeratePhysicalDevices(inst, &num_devices, &((phys_devices->devices)[0]));
	CHECK_VULKAN_ERROR("vkEnumeratePhysicalDevices error %d\n", res);
	i = 0;
	while (i < num_devices)
	{
		vkGetPhysicalDeviceProperties((phys_devices->devices)[i], &((phys_devices->dev_prop)[i]));
		vkGetPhysicalDeviceQueueFamilyProperties((phys_devices->devices)[i], &numq_family, NULL);
		(phys_devices->q_family_prop)[i] = malloc(sizeof(VkQueueFamilyProperties) * numq_family);
		(phys_devices->q_supports_present)[i] = malloc(sizeof(VkBool32) * numq_family);
		vkGetPhysicalDeviceQueueFamilyProperties((phys_devices->devices)[i], &numq_family, &((phys_devices->q_family_prop)[i][0]));
		j = 0;
		while (j < numq_family)
		{
			res = vkGetPhysicalDeviceSurfaceSupportKHR((phys_devices->devices)[i], j, surf, &((phys_devices->q_supports_present)[i][j]));
			CHECK_VULKAN_ERROR("vkGetPhysicalDeviceSurfaceSupportKHR error %d\n", res);
			j++;
		}
		vkGetPhysicalDeviceSurfaceFormatsKHR((phys_devices->devices)[i], surf, &num_formats, NULL);
		phys_devices->surface_formats[i] = malloc(sizeof(VkSurfaceFormatKHR) * num_formats);
		res = vkGetPhysicalDeviceSurfaceFormatsKHR((phys_devices->devices)[i], surf, &num_formats, &((phys_devices->surface_formats)[i][0]));
		CHECK_VULKAN_ERROR("vkGetPhysicalDeviceSurfaceFormatsKHR error %d\n", res);
		res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR((phys_devices->devices)[i], surf, &((phys_devices->surface_caps)[i]));
        CHECK_VULKAN_ERROR("vkGetPhysicalDeviceSurfaceCapabilitiesKHR error %d\n", res);
		i++;
	}
}
