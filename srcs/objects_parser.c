/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:46:02 by marvin            #+#    #+#             */
/*   Updated: 2020/09/11 17:53:18 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	parse_sphere(char *str, t_object *sphere)
{
	sphere->type = obj_sphere;
	if (ft_strstr(str, "position") != NULL)
		sphere->transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		sphere->color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		sphere->f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		sphere->f_radius = string_to_float(&str);
	}
}

void	parse_plane(char *str, t_object *plane)
{
	plane->type = obj_plane;
	if (ft_strstr(str, "position") != NULL)
		plane->transform.position = string_to_vector(str);
	else if (ft_strstr(str, "rotation") != NULL)
		plane->transform.rotation = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		plane->color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		plane->f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "vertices[0]") != NULL)
		plane->f_vertices[0] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[1]") != NULL)
		plane->f_vertices[1] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[2]") != NULL)
		plane->f_vertices[2] = string_to_vector(str);
	else if (ft_strstr(str, "vertices[3]") != NULL)
		plane->f_vertices[3] = string_to_vector(str);
}

void	parse_cone(char *str, t_object *cone)
{
	cone->type = obj_cone;
	if (ft_strstr(str, "position") != NULL)
		cone->transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		cone->color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		cone->f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		cone->f_radius = string_to_float(&str);
	}
	else if (ft_strstr(str, "height") != 0)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		cone->f_height = string_to_float(&str);
	}
	else if (ft_strstr(str, "direction") != NULL)
		cone->direction = string_to_vector(str);
}

void	parse_cylinder(char *str, t_object *cylinder)
{
	cylinder->type = obj_cylinder;
	if (ft_strstr(str, "position") != NULL)
		cylinder->transform.position = string_to_vector(str);
	else if (ft_strstr(str, "color") != NULL)
		cylinder->color = string_to_vector(str);
	else if (ft_strstr(str, "specular") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		cylinder->f_specular = ft_atoi(str);
	}
	else if (ft_strstr(str, "rotation") != NULL)
		cylinder->transform.rotation = string_to_vector(str);
	else if (ft_strstr(str, "radius") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		cylinder->f_radius = string_to_float(&str);
	}
}

void	parse_light(char *str, t_object *light, uint32_t type)
{
	light->type = type;
	if (ft_strstr(str, "intensity") != NULL)
	{
		while (!(*str >= '0' && *str <= '9'))
			str++;
		light->l_intensity = string_to_float(&str);
	}
	else if (ft_strstr(str, "position") != NULL)
		light->transform.position = string_to_vector(str);
	else if (ft_strstr(str, "direction") != NULL)
		light->direction = string_to_vector(str);
}
