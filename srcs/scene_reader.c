/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 10:50:02 by marvin            #+#    #+#             */
/*   Updated: 2020/04/20 20:51:03 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

double	string_to_double(char **str)
{
	int		n;
	double	res;
	double	t;

	t = 0.1;
	res = 0;
	n = ft_atoi(*str);
	while (**str <= '9' && **str >= '0')
		(**str)++;
	while (**str == ' ' || **str == ',' || **str == '\t')
		(**str)++;
	while (**str <= '9' && **str >= '0')
	{
		res += t * (**str - '0');
		t /= 10;
		(**str)++;
	}
	res += (double)n;
	return (res);
}

t_vec3	string_to_vector(char *str)
{
	t_vec3 res;
	
	while (!(*str >= '0' && *str <= '9'))
		(*str)++;
	res.x = string_to_double(&str);
	while (*str == ' ' || *str == ',' || *str == '\t')
		(*str)++;
	res.y = string_to_double(&str);
	while (*str == ' ' || *str == ',' || *str == '\t')
		(*str)++;
	res.z = string_to_double(&str);
	return (res);
}

void	parse_sphere(char *str, t_retr *r, int i)
{
	r->figures->type = obj_sphere;
	if (ft_strstr(str, "position") != NULL)
		r->figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->figures[i].color = string_to_vector(str);
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
		r->figures[i].figures.radius = string_to_double(&str);
	}
}

void	parse_plane(char *str, t_retr *r, int i)
{
	r->figures->type = obj_plane;
	if (ft_strstr(str, "position") != NULL)
		r->figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "direction") != NULL)
		r->figures[i].figures.direction = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->figures[i].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "vertices[0]") != NULL)
		r->figures[i].figures.vertices[0] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[1]") != NULL)
		r->figures[i].figures.vertices[1] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[2]") != NULL)
		r->figures[i].figures.vertices[2] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[3]") != NULL)
		r->figures[i].figures.vertices[3] = string_to_vector(str);
}

void	parse_cone(char *str, t_retr *r, int i)
{
	r->figures->type = obj_cone;
	if (ft_strstr(str, "position") != NULL)
		r->figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->figures[i].color = string_to_vector(str);
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
		r->figures[i].figures.radius = string_to_double(&str);
	}
	else if (ft_strstr(str, "height") != 0)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.height = string_to_double(&str);
	}
	else if (ft_strstr(str, "vertex") != NULL)
		r->figures[i].figures.ver = string_to_vector(str);
	else if (ft_strstr(str, "direction") != NULL)
		r->figures[i].figures.direction = string_to_vector(str);
}

void	parse_cylinder(char *str, t_retr *r, int i)
{
	r->figures->type = obj_cylinder;
	if (ft_strstr(str, "position") != NULL)
		r->figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->figures[i].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "direction") != NULL)
		r->figures[i].figures.direction = string_to_vector(str);
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->figures[i].figures.radius = string_to_double(&str);
	}
}

void	parse_light(char *str, t_retr *r, int j, int type)
{
	if (type == 5)
		r->lights[j].type = light_ambient;
	if (type == 6)
		r->lights[j].type = light_point;
	if (type == 7)
		r->lights[j].type = light_directional;
	if (ft_strstr(str, "intensity") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			(*str)++;
		r->lights[j].intensity = string_to_double(&str);
	}
	else if (ft_strstr(str, "position") != NULL)
		r->lights[j].position = string_to_vector(str);
	else if (ft_strstr(str, "direction") != NULL)
		r->lights[j].direction = string_to_vector(str);
}

void	parse_string(int type, char *str, t_retr *r, int i, int j)
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
	
	else if (type >= 5 && type <= 7)
		parse_light(str, r, j, type);
}

int		parse_type(char *str, int *i, int *j)
{
	if (ft_strstr(str, "sphere") != NULL)
	{
		(*i)++;
		return (1);
	}
	if (ft_strstr(str, "plane") != NULL)
	{
		(*i)++;
		return (2);
	}
	if (ft_strstr(str, "cone") != NULL)
	{
		(*i)++;
		return (3);
	}
	if (ft_strstr(str, "cylinder") != NULL)
	{
		(*i)++;
		return (4);
	}
	
	if (ft_strstr(str, "ambient") != NULL)
	{
		(*j)++;
		return (5);
	}
	if (ft_strstr(str, "point") != NULL)
	{
		(*j)++;
		return (6);
	}
	if (ft_strstr(str, "directional") != NULL)
	{
		(*j)++;
		return (7);
	}
}

int		read_scene(char *fname, t_retr *r)
{
	int		fd;
	char	*str;
	int		gnl;
	int		type;
	int		i;
	int		j;

	i = -1;
	j = -1;
	type = 0;
	if ((fd = open(fname, O_RDONLY)) == -1)
		return (0);
	while ((gnl = get_next_line(fd, &str)) != 0)
	{
		if (gnl == -1)
			return (0);
		if (ft_strstr(str, "type") != NULL)
			type = parse_type(str, &i, &j);
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
			parse_string(type, str, r, i, j);
	}
	return (1);
}
