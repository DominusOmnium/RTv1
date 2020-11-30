/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unique_parser.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 17:15:26 by marvin            #+#    #+#             */
/*   Updated: 2020/09/24 17:18:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	parse_unique(char *str, t_object *obj)
{
	if (ft_strstr(str, "mirror") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		obj->f_reflection = string_to_float(&str);
	}
}
