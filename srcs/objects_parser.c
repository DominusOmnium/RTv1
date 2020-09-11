/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_parser.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:46:02 by marvin            #+#    #+#             */
/*   Updated: 2020/09/10 15:46:02 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

void	parse_sphere(char *str, t_object *sphere)
{
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
		printf("plane->f_specular: %d\n", plane->f_specular);
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
	//ПЕРЕДЕЛАТЬ
	else if (ft_strstr(str, "vertex") != NULL)
		cone->f_ver = vec4_add_vec4(cone->transform.position, vec4_mul_f(cone->direction, cone->f_height));
	else if (ft_strstr(str, "direction") != NULL)
		cone->direction = string_to_vector(str);
}

void	parse_cylinder(char *str, t_object *cylinder)
{
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

void	parse_light(char *str, t_object *light)
{
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
