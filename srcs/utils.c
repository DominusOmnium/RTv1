/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 16:39:47 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/18 16:44:57 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		handle_error(char *msg)
{
	ft_putendl(msg);
	exit(0);
}

uint32_t	clamp_u32(uint32_t d, uint32_t min, uint32_t max)
{
	uint32_t	t;
	
	t = d < min ? min : d;
	return (t > max ? max : t);
}
