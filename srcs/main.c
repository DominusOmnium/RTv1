/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celva <celva@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:25:25 by celva             #+#    #+#             */
/*   Updated: 2020/03/06 10:25:26 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int	main(int ac, char **av)
{
	t_app	app;

	app.appname = "RTv1";
	rtv_app_create(&app);
	rtv_app_run(&app);
	rtv_app_destroy(&app);
	return (0);
}
