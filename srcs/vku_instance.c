/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_instance.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/05 23:23:27 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/05 23:36:55 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	vku_get_supported_extentions(VkExtensionProperties **p, uint32_t *c)
{
    if (vkEnumerateInstanceExtensionProperties(NULL, c, NULL) != VK_SUCCESS)
		return (1);
	if ((*p = malloc(sizeof(VkExtensionProperties) * *c)) == NULL)
		return (1);
    if (vkEnumerateInstanceExtensionProperties(NULL, &c, *p) != VK_SUCCESS)
		return (1);
	return (0);
}

int			vku_instance_create(t_app *app)
{
	VkApplicationInfo		appInfo;
	VkInstanceCreateInfo	instInfo;
	const char				**inst_ext;
	int						count_ext;
	int						i;

	appInfo = (VkApplicationInfo){};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = app->appname;
    appInfo.engineVersion = 1;
    appInfo.apiVersion = VK_API_VERSION_1_0;
	instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
	if (vkl_get_supported_extentions(&(app->vulkan.ext_prop), &count_ext) == 1)
		return (1);
	if ((inst_ext = malloc(sizeof(char*) * count_ext)) == NULL)
		return (1);
	i = -1;
	while (++i < count_ext)
		inst_ext[i] = (app->vulkan.ext_prop)[i].extensionName;
	instInfo.enabledExtensionCount = count_ext;
    instInfo.ppEnabledExtensionNames = inst_ext;
	if (vkCreateInstance(&instInfo, NULL, &(app->vulkan.inst)) != VK_SUCCESS)
		return (1);
	return (0);
}

void		vku_instance_delete()
{

}
