/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/15 10:50:02 by marvin            #+#    #+#             */
/*   Updated: 2020/09/11 17:54:10 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void		parse_camera(char *str, t_camera *camera)
{
	if (ft_strstr(str, "rotation") != NULL)
	{
		camera->transform.rotation = string_to_vector(str);
		camera->transform.rotation.x = RAD(camera->transform.rotation.x);
		camera->transform.rotation.y = RAD(camera->transform.rotation.y);
		camera->transform.rotation.z = RAD(camera->transform.rotation.z);
		camera->direction = (t_vec4){0, 0, 1, 0};
		rotate_x(&camera->direction, camera->transform.rotation.x);
		rotate_y(&camera->direction, camera->transform.rotation.y);
		rotate_z(&camera->direction, camera->transform.rotation.z);
		camera->up = (t_vec4){0, 1, 0, 0};
		rotate_x(&camera->up, camera->transform.rotation.x);
		rotate_y(&camera->up, camera->transform.rotation.y);
		rotate_z(&camera->up, camera->transform.rotation.z);
	}
	else if (ft_strstr(str, "position") != NULL)
		camera->transform.position = string_to_vector(str);
}

uint32_t	parse_type(char *str, t_object **cur_figure,
									t_object **cur_light)
{
	uint32_t	type;

	type = obj_null;
	if (ft_strstr(str, "sphere") != NULL)
		type = obj_sphere;
	if (ft_strstr(str, "plane") != NULL)
		type = obj_plane;
	if (ft_strstr(str, "cone") != NULL)
		type = obj_cone;
	if (ft_strstr(str, "cylinder") != NULL)
		type = obj_cylinder;
	if (ft_strstr(str, "ambient") != NULL)
		type = light_ambient;
	if (ft_strstr(str, "point") != NULL)
		type = light_point;
	if (ft_strstr(str, "directional") != NULL)
		type = light_directional;
	if (type == light_directional || type == light_point
				|| type == light_ambient)
		(*cur_light)++;
	else if (type != obj_null)
		(*cur_figure)++;
	return (type);
}

void		process_num(t_rt *r, char *str, t_object **cur_figure,
											t_object **cur_light)
{
	if (ft_strstr(str, "figures") != NULL)
	{
		while (*str < '0' || *str > '9')
			str++;
		r->n_fig = ft_atoi(str);
		if (r->n_fig <= 0)
			handle_error("Error in scene file. Figures number must be > 0");
		r->sbo_figures = ft_memalloc(sizeof(t_object) * r->n_fig);
		*cur_figure = r->sbo_figures - 1;
	}
	else if (ft_strstr(str, "lights") != NULL)
	{
		while (*str < '0' || *str > '9')
			str++;
		r->n_lig = ft_atoi(str);
		if (r->n_lig <= 0)
			handle_error("Error in scene file. Lights number must be > 0");
		r->sbo_lights = ft_memalloc(sizeof(t_object) * r->n_lig);
		*cur_light = r->sbo_lights - 1;
	}
}

void		process_line(t_rt *r, char *str, t_object **cur_figure,
												t_object **cur_light)
{
	static uint32_t		type;

	process_num(r, str, cur_figure, cur_light);
	if (ft_strstr(str, "camera") != NULL)
		type = camera;
	else if (ft_strstr(str, "type") != NULL)
	{
		type = parse_type(str, cur_figure, cur_light);
		if (*cur_light - r->sbo_lights >= r->n_lig
				|| *cur_figure - r->sbo_figures >= r->n_fig)
			handle_error("ERROR: Scene file wrong!");
	}
	else if (type == obj_sphere)
		parse_sphere(str, *cur_figure);
	else if (type == obj_plane)
		parse_plane(str, *cur_figure);
	else if (type == obj_cone)
		parse_cone(str, *cur_figure);
	else if (type == obj_cylinder)
		parse_cylinder(str, *cur_figure);
	else if (type == camera)
		parse_camera(str, &r->camera);
	else if (type != obj_null)
		parse_light(str, *cur_light, type);
}

void		read_scene(char *fname, t_rt *r)
{
	int32_t		fd;
	char		*str;
	int32_t		gnl;
	t_object	*cur_figure;
	t_object	*cur_light;

	if ((fd = open(fname, O_RDONLY)) == -1)
		handle_error("Open file error!");
	while ((gnl = get_next_line(fd, &str)) != 0)
	{
		if (gnl == -1)
			handle_error("Read file error!");
		process_line(r, str, &cur_figure, &cur_light);
		ft_memdel((void**)&str);
	}
}
