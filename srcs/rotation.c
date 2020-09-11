/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rotation.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 19:35:49 by celva             #+#    #+#             */
/*   Updated: 2020/09/11 16:14:32 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void				rotate_x(t_vec4 *t, float angle)
{
	float		prev_y;
	float		prev_z;

	prev_y = t->y;
	prev_z = t->z;
	t->y = (prev_y * cos(angle) - prev_z * sin(angle));
	t->z = (prev_y * sin(angle) + prev_z * cos(angle));
}

void				rotate_y(t_vec4 *t, float angle)
{
	float		prev_x;
	float		prev_z;

	prev_x = t->x;
	prev_z = t->z;
	t->x = (prev_x * cos(angle) + prev_z * sin(angle));
	t->z = (prev_z * cos(angle) - prev_x * sin(angle));
}

void				rotate_z(t_vec4 *t, float angle)
{
	float		prev_x;
	float		prev_y;

	prev_x = t->x;
	prev_y = t->y;
	t->x = (prev_x * cos(angle) - prev_y * sin(angle));
	t->y = (prev_x * sin(angle) + prev_y * cos(angle));
}
