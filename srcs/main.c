/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:25 by celva             #+#    #+#             */
/*   Updated: 2020/09/10 20:08:52 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	init_struct(t_rt *r, char *fname)
{
	*r = (t_rt){};
	read_scene(fname, r);
	r->camera.d = 1.0;
}

int		main(int ac, char **av)
{
	t_app	app;

	if (ac == 1)
		return (0);
	//setenv("VK_ICD_FILENAMES", VK_ICD_FILENAMES, 1);
	//setenv("VK_LAYER_PATH", VK_LAYER_PATH, 1);
	app.appname = "RTv1";
	init_struct(&app.r, av[1]);
	printf("app.r.sbo_figures[0].f_radius: %f\n", app.r.sbo_figures[0].f_radius);
	printf("app.r.sbo_figures[1].f_radius: %f\n", app.r.sbo_figures[1].f_radius);
	printf("app.r.sbo_figures[2].f_radius: %f\n", app.r.sbo_figures[2].f_radius);
	printf("app.r.sbo_figures[0].f_radius: %d\n", app.r.sbo_figures[0].type);
	printf("app.r.sbo_figures[1].f_radius: %d\n", app.r.sbo_figures[1].type);
	printf("app.r.sbo_figures[2].f_radius: %d\n", app.r.sbo_figures[2].type);
	rtv_app_create(&app);
	app.r.camera.vh = 1.0;
	app.r.camera.vw = app.r.win_width
								/ app.r.win_height;
	printf("App created\n");
	rtv_app_run(&app);
	return (0);
}
