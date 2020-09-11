/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 18:56:35 by celva             #+#    #+#             */
/*   Updated: 2020/09/11 16:41:09 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	keyboard_input_right_left(SDL_Event evt, t_camera *camera)
{
	if (evt.key.keysym.sym == SDLK_LEFT)
	{
		camera->transform.position = vec4_add_vec4(camera->transform.position,
				vec4_mul_f(vec4_cross(camera->direction, camera->up), 0.1f));
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_RIGHT)
	{
		camera->transform.position = vec4_add_vec4(camera->transform.position,
				vec4_mul_f(vec4_cross(camera->direction, camera->up), -0.1f));
		return (0);
	}
	return (-1);
}

static int	keyboard_input_up_down(SDL_Event evt, t_camera *camera)
{
	if (evt.key.keysym.sym == SDLK_UP)
	{
		camera->transform.position = vec4_add_vec4(camera->transform.position,
										vec4_mul_f(camera->direction, 0.1f));
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_DOWN)
	{
		camera->transform.position = vec4_sub_vec4(camera->transform.position,
										vec4_mul_f(camera->direction, 0.1f));
		return (0);
	}
	return (-1);
}

static int	keyboard_input_q_e_8_2(SDL_Event evt, t_camera *camera)
{
	if (evt.key.keysym.sym == SDLK_q)
	{
		camera->transform.rotation.y -= RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_e)
	{
		camera->transform.rotation.y += RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_KP_8)
	{
		camera->transform.rotation.x += RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_KP_2)
	{
		camera->transform.rotation.x -= RAD(10);
		return (0);
	}
	return (-1);
}

static int	keyboard_input_4_6(SDL_Event evt, t_camera *camera)
{
	if (evt.key.keysym.sym == SDLK_KP_4)
	{
		camera->transform.rotation.z -= RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_KP_6)
	{
		camera->transform.rotation.z += RAD(10);
		return (0);
	}
	return (-1);
}

int			handling_keyboard_input(SDL_Event evt, t_camera *camera)
{
	if ((keyboard_input_up_down(evt, camera) == 0) ||
		(keyboard_input_right_left(evt, camera) == 0) ||
		(keyboard_input_q_e_8_2(evt, camera) == 0) ||
		(keyboard_input_4_6(evt, camera) == 0))
	{
		camera->direction = (t_vec4){0, 0, 1, 0};
		rotate_x(&camera->direction, camera->transform.rotation.x);
		rotate_y(&camera->direction, camera->transform.rotation.y);
		rotate_z(&camera->direction, camera->transform.rotation.z);
		camera->up = (t_vec4){0, 1, 0, 0};
		rotate_x(&camera->up, camera->transform.rotation.x);
		rotate_y(&camera->up, camera->transform.rotation.y);
		rotate_z(&camera->up, camera->transform.rotation.z);
		return (0);
	}
	return (1);
}
