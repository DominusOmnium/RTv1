/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:25 by celva             #+#    #+#             */
/*   Updated: 2020/08/04 18:17:49 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	init_struct(t_retr *r, char *fname)
{
	read_scene(fname, r);
    r->vw = 1.5;
    r->vh = 1;
    r->d = 1;
}

int	main(int ac, char **av)
{
	t_app	app;

	if (ac == 1)
		return (0);
	app.appname = "RTv1";
	init_struct(&app.r, av[1]);
	rtv_app_create(&app);
	ft_printf("App created\n");
	rtv_app_run(&app, av[1]);
	return (0);
}
