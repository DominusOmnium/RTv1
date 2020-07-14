/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:25 by celva             #+#    #+#             */
/*   Updated: 2020/07/14 14:53:24 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int	main(int ac, char **av)
{
	t_app	app;

	if (ac == 1)
		return (0);
	app.appname = "RTv1";
	rtv_app_create(&app);
	ft_printf("App created\n");
	rtv_app_run(&app, av[1]);
	return (0);
}
