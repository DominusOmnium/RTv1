/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 10:50:02 by marvin            #+#    #+#             */
/*   Updated: 2020/08/25 14:31:27 by marvin           ###   ########.fr       */
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

void	parse_sphere(char *str, t_rt *r, int figure_number)
{
	r->sbo_figures[figure_number].type = obj_sphere;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[figure_number].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[figure_number].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_radius = string_to_float(&str);
	}
}

void	parse_plane(char *str, t_rt *r, int figure_number)
{
	r->sbo_figures[figure_number].type = obj_plane;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[figure_number].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "rotation") != NULL)
		r->sbo_figures[figure_number].transform.rotation = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[figure_number].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "vertices[0]") != NULL)
		r->sbo_figures[figure_number].f_vertices[0] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[1]") != NULL)
		r->sbo_figures[figure_number].f_vertices[1] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[2]") != NULL)
		r->sbo_figures[figure_number].f_vertices[2] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[3]") != NULL)
		r->sbo_figures[figure_number].f_vertices[3] = string_to_vector(str);
}

void	parse_cone(char *str, t_rt *r, int figure_number)
{
	r->sbo_figures[figure_number].type = obj_cone;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[figure_number].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[figure_number].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_radius = string_to_float(&str);
	}
	else if (ft_strstr(str, "height") != 0)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_height = string_to_float(&str);
	}
	else if (ft_strstr(str, "vertex") != NULL)
		//r->sbo_figures[figure_number].f_ver = string_to_vector(str);
		r->sbo_figures[figure_number].f_ver = vec4_add_vec4(r->sbo_figures[figure_number].transform.position, vec4_mul_f(r->sbo_figures[figure_number].direction, r->sbo_figures[figure_number].f_height));
	else if (ft_strstr(str, "direction") != NULL)
		r->sbo_figures[figure_number].direction = string_to_vector(str);
}

void	parse_cylinder(char *str, t_rt *r, int figure_number)
{
	r->sbo_figures[figure_number].type = obj_cylinder;
	if (ft_strstr(str, "position") != NULL)
		r->sbo_figures[figure_number].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		r->sbo_figures[figure_number].color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "rotation") != NULL)
		r->sbo_figures[figure_number].transform.rotation = string_to_vector(str);
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_figures[figure_number].f_radius = string_to_float(&str);
	}
}

void	parse_light(char *str, t_rt *r, int light_number, int type)
{
	if (type == light_ambient)
		r->sbo_lights[light_number].type = light_ambient;
	if (type == light_point)
		r->sbo_lights[light_number].type = light_point;
	if (type == light_directional)
		r->sbo_lights[light_number].type = light_directional;
	if (ft_strstr(str, "intensity") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		r->sbo_lights[light_number].l_intensity = string_to_float(&str);
	}
	else if (ft_strstr(str, "position") != NULL)
		r->sbo_lights[light_number].transform.position = string_to_vector(str);
	else if (ft_strstr(str, "direction") != NULL)
		r->sbo_lights[light_number].direction = string_to_vector(str);
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

void	sc_parse_string(int type, char *str, t_rt *r, int figure_number, int light_number)
{
	if (type == obj_null)
		return ;
	else if (type == obj_sphere)
		parse_sphere(str, r, figure_number);
	else if (type == obj_plane)
		parse_plane(str, r, figure_number);
	else if (type == obj_cone)
		parse_cone(str, r, figure_number);
	else if (type == obj_cylinder)
		parse_cylinder(str, r, figure_number);
	else if (type == light_ambient || type == light_point || type == light_directional)
		parse_light(str, r, light_number, type);
	else if (type == camera)
		parse_camera(r, str);
}

// ПЕРЕДЕЛАТЬ ЭТУ ФУНКЦИЮ. РЕШИТЬ ВОПРОС С ТИПАМИ НОРМАЛЬНО
int		parse_type(char *str, int *figure_number, int *light_number)
{
	if (ft_strstr(str, "sphere") != NULL)
	{
		(*figure_number)++;
		return (obj_sphere);
	}
	if (ft_strstr(str, "plane") != NULL)
	{
		(*figure_number)++;
		return (obj_plane);
	}
	if (ft_strstr(str, "cone") != NULL)
	{
		(*figure_number)++;
		return (obj_cone);
	}
	if (ft_strstr(str, "cylinder") != NULL)
	{
		(*figure_number)++;
		return (obj_cylinder);
	}
	if (ft_strstr(str, "ambient") != NULL)
	{
		(*light_number)++;
		return (light_ambient);
	}
	if (ft_strstr(str, "point") != NULL)
	{
		(*light_number)++;
		return (light_point);
	}
	if (ft_strstr(str, "directional") != NULL)
	{
		(*light_number)++;
		return (light_directional);
	}
	return (obj_null);
}

void		read_scene(char *fname, t_rt *r)
{
	int		fd;
	char	*str;
	int		gnl;
	int		type;
	int		figure_number;
	int		light_number;

	figure_number = -1;
	light_number = -1;
	type = 0;
	if ((fd = open(fname, O_RDONLY)) == -1)
		handle_error("Open file error!");
	while ((gnl = get_next_line(fd, &str)) != 0)
	{
		if (gnl == -1)
			handle_error("Read file error!");
		if (ft_strstr(str, "type") != NULL)
			type = parse_type(str, &figure_number, &light_number);
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
			type = camera;
		else
			sc_parse_string(type, str, r, figure_number, light_number);
	}
}
