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
