/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_instance.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 23:23:27 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/24 18:26:30 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	vku_get_supported_extentions(VkExtensionProperties **p, uint32_t *c)
{
    if (vkEnumerateInstanceExtensionProperties(NULL, c, NULL) != VK_SUCCESS)
		return (0);
	if ((*p = ft_memalloc(sizeof(VkExtensionProperties) * *c)) == NULL)
		return (0);
    if (vkEnumerateInstanceExtensionProperties(NULL, c, *p) != VK_SUCCESS)
		return (0);
	return (1);
}

int			vku_instance_create(t_app *app)
{
	VkApplicationInfo		appInfo;
	VkInstanceCreateInfo	instInfo;
	const char				**inst_ext;
	int						count_ext;
	int						i;

	appInfo = (VkApplicationInfo){
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = app->appname,
		.engineVersion = 1,
		.apiVersion = VK_API_VERSION_1_0
	};
	instInfo = (VkInstanceCreateInfo){
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo
	};
	if (vku_get_supported_extentions(&(app->vulkan.ext_prop), &count_ext) == 0)
		return (0);
	if ((inst_ext = ft_memalloc(sizeof(char*) * count_ext)) == NULL)
		return (0);
	i = -1;
	while (++i < count_ext)
		inst_ext[i] = (app->vulkan.ext_prop)[i].extensionName;
	instInfo.enabledExtensionCount = count_ext;
    instInfo.ppEnabledExtensionNames = inst_ext;
	if (vkCreateInstance(&instInfo, NULL, &(app->vulkan.inst)) != VK_SUCCESS)
		return (0);
	free(inst_ext);
	return (1);
}

void		vku_instance_destroy(t_vulkan *v)
{
	vkDestroyInstance(v->inst, NULL);
	free(v->ext_prop);
}
