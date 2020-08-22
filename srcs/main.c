/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:25 by celva             #+#    #+#             */
/*   Updated: 2020/08/20 14:56:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	init_struct(t_rt *r, char *fname)
{
	read_scene(fname, r);
	r->camera.d = 1.0;
}

int	main(int ac, char **av)
{
	t_app	app;

	if (ac == 1)
		return (0);
	app.appname = "RTv1";
	init_struct(&app.r, av[1]);
	rtv_app_create(&app);
	app.r.camera.vh = 1.0;
	app.r.camera.vw = app.r.win_width
								/ app.r.win_height;
	ft_printf("App created\n");
	rtv_app_run(&app);
	return (0);
}
