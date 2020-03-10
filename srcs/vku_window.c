/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_window.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 13:59:58 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/10 11:35:50 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void vku_window_destroy(t_app *app)
{
    vkDestroySurfaceKHR(app->vulkan.inst, app->vulkan.surface, 0);
    //SDL_DestroyWindow(app->vulkan.window);
}

int	vku_window_create(t_app *app)
{
	VkResult result;
	
	app->vulkan.window = SDL_CreateWindow(app->appname, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_VULKAN);
	result = SDL_Vulkan_CreateSurface(app->vulkan.window, app->vulkan.inst,
													&(app->vulkan.surface));
	return (app->vulkan.window != 0 && result == VK_SUCCESS);
}
