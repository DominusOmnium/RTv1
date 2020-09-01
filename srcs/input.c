/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celva <celva@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 18:56:35 by celva             #+#    #+#             */
/*   Updated: 2020/09/01 19:25:20 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

static int	keyboard_input_right_left(SDL_Event evt, t_app *app)
{
	if (evt.key.keysym.sym == SDLK_LEFT)
	{
		app->r.camera.transform.position =
					vec4_add_vec4(app->r.camera.transform.position,
					(t_vec4){-0.1, 0, 0, 0});
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_RIGHT)
	{
		app->r.camera.transform.position =
					vec4_add_vec4(app->r.camera.transform.position,
					(t_vec4){0.1, 0, 0, 0});
		return (0);
	}
	return (-1);
}

static int	keyboard_input_up_down(SDL_Event evt, t_app *app)
{
	if (evt.key.keysym.sym == SDLK_UP)
	{
		app->r.camera.transform.position =
					vec4_add_vec4(app->r.camera.transform.position,
					vec4_mul_f(app->r.camera.direction,
					vec4_mod(app->r.camera.direction)));
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_DOWN)
	{
		app->r.camera.transform.position =
					vec4_sub_vec4(app->r.camera.transform.position,
					vec4_mul_f(app->r.camera.direction,
					vec4_mod(app->r.camera.direction)));
		return (0);
	}
	return (-1);
}

static int	keyboard_input_q_e_8_2(SDL_Event evt, t_app *app)
{
	if (evt.key.keysym.sym == SDLK_q)
	{
		p3d_rotate_y(&app->r.camera.direction, -10);
		app->r.camera.transform.rotation.y -= RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_e)
	{
		p3d_rotate_y(&app->r.camera.direction, 10);
		app->r.camera.transform.rotation.y += RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_KP_8)
	{
		p3d_rotate_x(&app->r.camera.direction, 10);
		app->r.camera.transform.rotation.x += RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_KP_2)
	{
		p3d_rotate_x(&app->r.camera.direction, -10);
		app->r.camera.transform.rotation.x -= RAD(10);
		return (0);
	}
	return (-1);
}

static int	keyboard_input_4_6(SDL_Event evt, t_app *app)
{
	if (evt.key.keysym.sym == SDLK_KP_4)
	{
		p3d_rotate_z(&app->r.camera.direction, -10);
		app->r.camera.transform.rotation.z -= RAD(10);
		return (0);
	}
	if (evt.key.keysym.sym == SDLK_KP_6)
	{
		p3d_rotate_z(&app->r.camera.direction, 10);
		app->r.camera.transform.rotation.z += RAD(10);
		return (0);
	}
	return (-1);
}

int			handling_keyboard_input(SDL_Event evt, t_app *app)
{
	if ((keyboard_input_up_down(evt, app) == 0) ||
		(keyboard_input_right_left(evt, app) == 0) ||
		(keyboard_input_q_e_8_2(evt, app) == 0) ||
		(keyboard_input_4_6(evt, app) == 0))
		return (0);
	return (-1);
}
