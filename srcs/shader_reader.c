/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_reader.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 19:07:59 by dkathlee          #+#    #+#             */
/*   Updated: 2020/07/17 13:04:05 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static size_t	read_shader_file(char *fname, char **shader)
{
	int		fd;
	size_t	r;
	size_t	nor;
	char	line[16];
	char	*res;

	if ((fd = open(fname, O_RDONLY)) == -1)
		return (0);
	nor = 0;
	while ((r = read(fd, line, 16)) != 0)
	{
		*shader = ft_realloc(*shader, nor, nor + r);
		ft_memcpy(*shader + nor, line, r);
		nor += r;
	}
	close(fd);
	return (nor);
}

size_t			load_shader_file(char *fname, char **shader)
{
	int	code_len;

	*shader = ft_memalloc(0);
	if ((code_len = read_shader_file(fname, shader)) == 0)
		return (0);
	return (code_len);
}
