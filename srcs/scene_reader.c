#include "rtv1.h"

int	read_scene(char *fname, t_retr *r)
{
	int	fd;

	if ((fd = open(fname, O_RDONLY)) == -1)
		return (0);
	
}
