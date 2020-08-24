/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 10:50:02 by marvin            #+#    #+#             */
/*   Updated: 2020/08/24 16:35:48 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

float	string_to_float(char **str)
{
	int		n;
	float	res;
	float	t;

	t = 0.1;
	res = 0;
	n = ft_atoi(*str);
	while ((**str <= '9' && **str >= '0') || **str == '-')
		(*str)++;
	while (**str == ' ' || **str == '.' || **str == '\t')
		(*str)++;
	while (**str <= '9' && **str >= '0')
	{
		res += t * (**str - '0');
		t /= 10;
		(*str)++;
	}
	res += (float)n;
	return (res);
}

t_vec4	string_to_vector(char *str)
{
	t_vec4 res;
	
	while ((!((*str >= '0' && *str <= '9') && (*(str + 1) != ']'))) && (*str != '-'))
		str++;
	res.x = string_to_float(&str);
	while (*str == ' ' || *str == ',' || *str == '\t')
		str++;
	res.y = string_to_float(&str);
	while (*str == ' ' || *str == ',' || *str == '\t')
		str++;
	res.z = string_to_float(&str);
	return (res);
}

void	parse_sphere(char *str, t_rt *r, int i)
{
	r->sbo_figures[i].type = obj_sphere;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[i].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_radius = string_to_float(&str);
	}
}

void	parse_plane(char *str, t_rt *r, int i)
{
	r->sbo_figures[i].type = obj_plane;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "rotation") != NULL)
		r->sbo_figures[i].transform.rotation = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[i].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "vertices[0]") != NULL)
		r->sbo_figures[i].f_vertices[0] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[1]") != NULL)
		r->sbo_figures[i].f_vertices[1] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[2]") != NULL)
		r->sbo_figures[i].f_vertices[2] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[3]") != NULL)
		r->sbo_figures[i].f_vertices[3] = string_to_vector(str);
}

void	parse_cone(char *str, t_rt *r, int i)
{
	r->sbo_figures[i].type = obj_cone;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[i].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_radius = string_to_float(&str);
	}
	else if (ft_strstr(str, "height") != 0)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_height = string_to_float(&str);
	}
	else if (ft_strstr(str, "vertex") != NULL)
		r->sbo_figures[i].f_ver = string_to_vector(str);
	else if (ft_strstr(str, "direction") != NULL)
		r->sbo_figures[i].f_direction = string_to_vector(str);
}

void	parse_cylinder(char *str, t_rt *r, int i)
{
	r->sbo_figures[i].type = obj_cylinder;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[i].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[i].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "rotation") != NULL)
		r->sbo_figures[i].transform.rotation = string_to_vector(str);
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[i].f_radius = string_to_float(&str);
	}
}

void	parse_light(char *str, t_rt *r, int j, int type)
{
	if (type == 5)
		r->sbo_lights[j].type = light_ambient;
	if (type == 6)
		r->sbo_lights[j].type = light_point;
	if (type == 7)
		r->sbo_lights[j].type = light_directional;
	if (ft_strstr(str, "intensity") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_lights[j].l_intensity = string_to_float(&str);
	}
	else if (ft_strstr(str, "position") != NULL)
		r->sbo_lights[j].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "rotation") != NULL)
		r->sbo_lights[j].transform.rotation = string_to_vector(str);
}

void	parse_camera(t_rt *r, char *str)
{
	if (ft_strstr(str, "direction") != NULL)
	{
		r->camera.direction = string_to_vector(str);
		r->camera.direction = vec4_mul_f(r->camera.direction, 1.0 / vec4_mod(r->camera.direction));
		if (r->camera.direction.z != 0.0f)
			r->camera.transform.rotation.x = -atanf(r->camera.direction.y / r->camera.direction.z);
		else
			r->camera.transform.rotation.x = 0.0f;
		if (r->camera.direction.z != 0.0f)
			r->camera.transform.rotation.y = -atanf(r->camera.direction.x / r->camera.direction.z);
		else
			r->camera.transform.rotation.y = 0.0f;
		if (r->camera.direction.x != 0.0f)
			r->camera.transform.rotation.z = -atanf(r->camera.direction.y / r->camera.direction.x);
		else
			r->camera.transform.rotation.z = 0.0f;
		r->camera.transform.rotation.w = 0.0f;
		printf("r->camera.direction: (%f, %f, %f)\n", r->camera.direction.x, r->camera.direction.y, r->camera.direction.z);
		printf("r->camera.transform.rotation.x: %f\n", r->camera.transform.rotation.x);
		printf("r->camera.transform.rotation.y: %f\n", r->camera.transform.rotation.y);
		printf("r->camera.transform.rotation.z: %f\n", r->camera.transform.rotation.z);
	}
	else if (ft_strstr(str, "position") != NULL)
		r->camera.transform.position = string_to_vector(str);
}

void	sc_parse_string(int type, char *str, t_rt *r, int i, int j)
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
	else if (type == 8)
		parse_camera(r, str);
}

// ПЕРЕДЕЛАТЬ ЭТУ ФУНКЦИЮ. РЕШИТЬ ВОПРОС С ТИПАМИ НОРМАЛЬНО
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
	if (ft_strstr(str, "rotational") != NULL)
	{
		(*j)++;
		return (7);
	}
	return (0);
}

void		read_scene(char *fname, t_rt *r)
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
		handle_error("Open file error!");
	while ((gnl = get_next_line(fd, &str)) != 0)
	{
		if (gnl == -1)
			handle_error("Read file error!");
		if (ft_strstr(str, "type") != NULL)
			type = parse_type(str, &i, &j);
		else if (ft_strstr(str, "figures") != NULL)
		{
			while (!(*str >= '0' && *str <= '9'))
				str++;
			r->n_fig = ft_atoi(str);
			r->sbo_figures = (t_object*)ft_memalloc(sizeof(t_object) * r->n_fig);
		}
		else if (ft_strstr(str, "lights") != NULL)
		{
			while (!(*str >= '0' && *str <= '9'))
				str++;
			r->n_lig = ft_atoi(str);
			r->sbo_lights = (t_object*)ft_memalloc(sizeof(t_object) * r->n_lig);
		}
		else if (ft_strstr(str, "camera") != NULL)
			type = 8;
		else
			sc_parse_string(type, str, r, i, j);
	}
}
