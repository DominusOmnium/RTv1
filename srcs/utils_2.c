/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/18 22:01:41 by celva             #+#    #+#             */
/*   Updated: 2021/01/18 22:01:41 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec4		json_parse_vec3(cJSON *value)
{
	cJSON	*vec_x;
	cJSON	*vec_y;
	cJSON	*vec_z;
	t_vec4	vec;

	vec_x = cJSON_GetArrayItem(value, 0);
	vec_y = cJSON_GetArrayItem(value, 1);
	vec_z = cJSON_GetArrayItem(value, 2);
	if (vec_x != NULL && vec_y != NULL && vec_z != NULL)
	{
		vec = (t_vec4){.x = vec_x->valuedouble,
					.y = vec_y->valuedouble, .z = vec_z->valuedouble, 0};
	}
	else
		handle_error("Wrong vector in .json!");
	return (vec);
}

t_vec2		json_parse_vec2(cJSON *value)
{
	cJSON	*vec_x;
	cJSON	*vec_y;
	t_vec2	vec;

	vec_x = cJSON_GetArrayItem(value, 0);
	vec_y = cJSON_GetArrayItem(value, 1);
	if (vec_x != NULL && vec_y != NULL)
	{
		vec = (t_vec2){.x = vec_x->valuedouble,
					.y = vec_y->valuedouble};
	}
	else
		handle_error("Wrong vector in .json!");
	return (vec);
}
