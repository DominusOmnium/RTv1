/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 16:39:47 by dkathlee          #+#    #+#             */
/*   Updated: 2020/09/10 14:33:18 by marvin           ###   ########.fr       */
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

float		string_to_float(char **str)
{
	int32_t	n;
	float	res;
	float	t;

	t = 0.1;
	res = 0;
	n = ft_atoi(*str);
	while ((**str <= '9' && **str >= '0') || **str == '-')
		(*str)++;
	while (**str == ' ' || **str == '.' || **str == '\t')
		(*str)++;
	while (**str <= '9' && **str >= '0')
	{
		res += t * (**str - '0');
		t /= 10;
		(*str)++;
	}
	res += (float)n;
	return (res);
}

t_vec4		string_to_vector(char *str)
{
	t_vec4 res;
	
	while ((!((*str >= '0' && *str <= '9') && (*(str + 1) != ']'))) && (*str != '-'))
		str++;
	res.x = string_to_float(&str);
	while (*str == ' ' || *str == ',' || *str == '\t')
		str++;
	res.y = string_to_float(&str);
	while (*str == ' ' || *str == ',' || *str == '\t')
		str++;
	res.z = string_to_float(&str);
	return (res);
}
