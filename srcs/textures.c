/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 09:52:27 by marvin            #+#    #+#             */
/*   Updated: 2020/09/30 09:52:27 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static void	load_texture(char *fname, t_texture *texture, void *buffer,
														uint32_t offset)
{
	SDL_Surface			*an_surf;
	SDL_Surface			*surf;
	SDL_PixelFormat		*fmt;

	if ((an_surf = IMG_Load(fname)) == NULL)
		handle_error("Loading image error!");
	if ((fmt = ft_memalloc(sizeof(SDL_PixelFormat))) == NULL)
		handle_error(ERROR_MEM_ALLOC);
	ft_memcpy(fmt, an_surf->format, sizeof(SDL_PixelFormat));
	fmt->BytesPerPixel = 4;
	fmt->BitsPerPixel = 32;
	fmt->Rmask = RMASK;
	fmt->Gmask = GMASK;
	fmt->Bmask = BMASK;
	fmt->Amask = AMASK;
	if ((surf = SDL_ConvertSurface(an_surf, fmt, an_surf->flags)) == NULL)
		handle_error("Surface convertation error");
	SDL_FreeSurface(an_surf);
	ft_memdel((void **)&fmt);
	texture->height = surf->h;
	texture->width = surf->w;
	texture->offset_in_buffer = offset;
	ft_memcpy(buffer + offset, surf->pixels, surf->h * surf->pitch);
	SDL_FreeSurface(surf);
}

void		vku_load_textures(t_rt *r, t_vulkan *v, void *buffer)
{
	uint32_t	i;
	uint32_t	buf_index;
	uint32_t	offset;
	void		*data;

	buf_index = 0;
	while (buf_index < v->framebuffer.sc_image_count)
	{	
		if (vkMapMemory(v->device, v->sbo_buffers[buf_index].dev_mem, 0,
						TEXTURES_BUFFER_SIZE, 0, &data) != VK_SUCCESS)
			handle_error("Map Memory error!");
		offset = 0;
		i = 0;
		while (i < r->n_fig)
		{
			load_texture(r->texture_files[i], &((r->sbo_figures)[i].texture), data, offset);
			offset += r->sbo_figures[i].texture.width * r->sbo_figures[i].texture.height;
			i++;
		}
		buf_index++;
		vkUnmapMemory(v->device, v->sbo_buffers[buf_index].dev_mem);
	}
}
