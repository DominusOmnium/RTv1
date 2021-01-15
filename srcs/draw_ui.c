/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_ui.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/21 03:02:52 by marvin            #+#    #+#             */
/*   Updated: 2020/12/21 03:02:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	draw_ui(t_app *app)
{
	SDL_Surface	*win_surf;
	//SDL_Surface	*img_surf;

	app->vulkan.surface
	win_surf = SDL_GetWindowSurface(app->window);
	//img_surf = load_texture("images/UI.png");
    SDL_LockSurface(win_surf);
	uint32_t *win_pixels = win_surf->pixels;
	//uint32_t *img_pixels = img_surf->pixels;

    /*for (int dy = 0; dy < img_surf->h; dy++) {
        for (int dx = 0; dx < img_surf->w; dx++) {
			if (img_pixels[dy * img_surf->w + dx] & 0x000000ff)
			{
				win_pixels[dx + dy * win_surf->w] = img_pixels[dy * img_surf->w + dx];
			}
        }	
    }*/
    for (int dy = 0; dy < 10; dy++) {
        for (int dx = 0; dx < 10; dx++) {
			win_pixels[dx + dy * win_surf->w] = 255;
        }
    }
    //SDL_BlitSurface(win_pixels, NULL, win_surf, NULL);
	SDL_UpdateWindowSurface(app->window);
    //memcpy(surface->pixels, pixels.data(), surface->pitch * surface->h);
    SDL_UnlockSurface(win_surf);
}
