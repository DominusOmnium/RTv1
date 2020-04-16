/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 10:50:02 by marvin            #+#    #+#             */
/*   Updated: 2020/04/15 10:50:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double	string_to_double(char *str)
{
	int		n;
	double	res;
	double	t;

	t = 0.1;
	res = 0;
	n = ft_atoi(&str);
	(*str)++;
	while (*str <= '9' && *str >= '0')
	{
		res += t * (*str - '0');
		t /= 10;
		(*str)++;
	}
	res += (double)n;
	return (res);
}

void	parse_sphere(char *str, t_retr *r, int i)
{
	r->figures->type = obj_sphere;
	if (ft_strstr(str, "position") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.x = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "color") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.x = ft_atoi(str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.radius = string_to_double(str);
	}
}

void	parse_plane(char *str, t_retr *r, int i)
{
	r->figures->type = obj_plane;
	if (ft_strstr(str, "position") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.x = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "direction") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.x = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "color") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.x = ft_atoi(str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].specular = ft_atoi(str);
	}
}

void	parse_cone(char *str, t_retr *r, int i)
{
	r->figures->type = obj_cone;
	if (ft_strstr(str, "position") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.x = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "color") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.x = ft_atoi(str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.radius = string_to_double(str);
	}
	else if (ft_strstr(str, "height") != 0)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.height = string_to_double(str);
	}
	else if (ft_strstr(str, "vertex") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.ver.x = ft_atoi(str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.ver.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.ver.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "direction") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.x = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.z = ft_atoi(&str);
	}
}

void	parse_cylinder(char *str, t_retr *r, int i)
{
	r->figures->type = obj_cylinder;
	if (ft_strstr(str, "position") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.x = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].transform.position.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "color") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.x = ft_atoi(str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].color.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "direction") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.x = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.y = ft_atoi(&str);
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.direction.z = ft_atoi(&str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.radius = string_to_double(str);
	}
}

void	parse_string(int type, char *str, t_retr *r, int i)
{
	if (type == 0)
		return ;
	else if (type == 1)
		parse_sphere(str, r, i);
	else if (type == 2)
		parse_plane(str, r, i);
	else if (type == 3)
		parse_cone(str, r, i);
	else if (type == 4)
		parse_cylinder(str, r, i);
	
	else if (type == 5)
		parse_cylinder(str, r, i);
	else if (type == 6)
		parse_cylinder(str, r, i);
	else if (type == 7)
		parse_cylinder(str, r, i);
}

int		parse_type(char *str)
{
	if (ft_strstr(str, "sphere") != NULL)
		return (1);
	if (ft_strstr(str, "plane") != NULL)
		return (2);
	if (ft_strstr(str, "cone") != NULL)
		return (3);
	if (ft_strstr(str, "cylinder") != NULL)
		return (4);
	if (ft_strstr(str, "light_ambient") != NULL)
		return (5);
	if (ft_strstr(str, "light_point") != NULL)
		return (6);
	if (ft_strstr(str, "light_directional") != NULL)
		return (7);
}

int		read_scene(char *fname, t_retr *r)
{
	int		fd;
	char	*str;
	int		gnl;
	int		type;
	int		i;

	i = -1;
	type = 0;
	if ((fd = open(fname, O_RDONLY)) == -1)
		return (0);
	while ((gnl = get_next_line(fd, &str)) != 0)
	{
		if (gnl == -1)
			return (NULL);
		if (ft_strstr(str, "type") != NULL)
		{
			type = parse_type(str);
			i++;
		}
		else if (ft_strstr(str, "figures") != NULL)
		{
			while (!(*str >= '0' && *str <= '9'))
				(*str)++;
			r->n_fig = ft_atoi(str);
			r->figures = (t_object*)ft_memalloc(sizeof(t_object) * r->n_fig);
		}
		else if (ft_strstr(str, "lights") != NULL)
		{
			while (!(*str >= '0' && *str <= '9'))
				(*str)++;
			r->n_lig = ft_atoi(str);
			r->lights = (t_light*)ft_memalloc(sizeof(t_light) * r->n_lig);
		}
		else
			parse_string(type, str, r, i);
	}
}
