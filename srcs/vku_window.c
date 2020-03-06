/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vku_window.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 13:59:58 by dkathlee          #+#    #+#             */
/*   Updated: 2020/03/06 14:41:19 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int	vku_window_create(t_app *app)
{
	SDL_Window* window = SDL_CreateWindow(app->appname, SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT, SDL_WINDOW_VULKAN);
	SDL_Vulkan_CreateSurface(window, app->vulkan.inst, &(app->vulkan.surface));
}
