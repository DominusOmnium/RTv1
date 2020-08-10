/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_instance.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 23:23:27 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/04 12:42:42 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static const char**	vku_get_supported_extentions(uint32_t *c,
											VkExtensionProperties **ext_prop)
{
	const char				**inst_ext;
	int32_t					i;
	
    if (vkEnumerateInstanceExtensionProperties(NULL, c, NULL) != VK_SUCCESS)
		return (0);
	if ((*ext_prop = ft_memalloc(sizeof(VkExtensionProperties) * *c)) == NULL)
		return (0);
    if (vkEnumerateInstanceExtensionProperties(NULL, c, *ext_prop) != VK_SUCCESS)
		return (0);
	if ((inst_ext = ft_memalloc(sizeof(char*) * *c)) == NULL)
		return (0);
	i = -1;
	while (++i < *c)
		inst_ext[i] = (*ext_prop)[i].extensionName;
	return (inst_ext);
}

int			vku_instance_create(t_app *app)
{
	VkExtensionProperties	*ext_prop;
	VkInstanceCreateInfo	inst_info;
	const char				**inst_ext;
	int						count_ext;
	int						i;

	/*if (vku_get_supported_extentions(&ext_prop, &count_ext) == 0)
		return (0);
	if ((inst_ext = ft_memalloc(sizeof(char*) * count_ext)) == NULL)
		return (0);
	i = -1;
	while (++i < count_ext)
		inst_ext[i] = ext_prop[i].extensionName;*/
	inst_ext = vku_get_supported_extentions(&count_ext, &ext_prop);
	inst_info = (VkInstanceCreateInfo){
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &(VkApplicationInfo){
			.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
			.pApplicationName = app->appname,
			.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
			.pEngineName = "No Engine",
			.engineVersion = 1,
			.apiVersion = VK_API_VERSION_1_0
		},
		.enabledExtensionCount = count_ext,
		.ppEnabledExtensionNames = inst_ext
	};
	VkResult res;
	if (res = vkCreateInstance(&inst_info, NULL, &(app->vulkan.inst)) != VK_SUCCESS)
	{
		ft_printf("%d\n", res);
		return (0);
	}
	ft_memdel((void**)&ext_prop);
	ft_memdel((void**)&inst_ext);
	return (1);
}

void		vku_instance_destroy(t_vulkan *v)
{
	vkDestroyInstance(v->inst, NULL);
}
