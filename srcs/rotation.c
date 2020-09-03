/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p3d_rotate.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celva <celva@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 19:35:49 by celva             #+#    #+#             */
/*   Updated: 2020/09/01 19:36:21 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void				rotate_x(t_vec4 *t, float angle)
{
	float		prev_y;
	float		prev_z;

	prev_y = t->y;
	prev_z = t->z;
	t->y = (prev_y * cos(RAD(angle)) - prev_z * sin(RAD(angle)));
	t->z = (prev_y * sin(RAD(angle)) + prev_z * cos(RAD(angle)));
}

void				rotate_y(t_vec4 *t, float angle)
{
	float		prev_x;
	float		prev_z;

	prev_x = t->x;
	prev_z = t->z;
	t->x = (prev_x * cos(RAD(angle)) + prev_z * sin(RAD(angle)));
	t->z = (prev_z * cos(RAD(angle)) - prev_x * sin(RAD(angle)));
}

void				rotate_z(t_vec4 *t, float angle)
{
	float		prev_x;
	float		prev_y;

	prev_x = t->x;
	prev_y = t->y;
	t->x = (prev_x * cos(RAD(angle)) - prev_y * sin(RAD(angle)));
	t->y = (prev_x * sin(RAD(angle)) + prev_y * cos(RAD(angle)));
}
