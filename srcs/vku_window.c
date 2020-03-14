/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_window.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 13:59:58 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/14 16:27:39 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void vku_window_destroy(t_app *app)
{
    vkDestroySurfaceKHR(app->vulkan.inst, app->vulkan.surface, 0);
    SDL_DestroyWindow(app->vulkan.window);
}

int	vku_window_create(t_app *app)
{
	app->vulkan.window = SDL_CreateWindow(app->appname, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_VULKAN);
	if (app->vulkan.window == NULL)
		return (0);
	if (!SDL_Vulkan_CreateSurface(app->vulkan.window, app->vulkan.inst,
													&(app->vulkan.surface)))
	return (1);
}
