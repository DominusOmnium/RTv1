/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader_json.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 20:29:13 by marvin            #+#    #+#             */
/*   Updated: 2020/11/23 20:29:13 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec4		json_parse_vec3(cJSON *value)
{
	cJSON	*vec_x;
	cJSON	*vec_y;
	cJSON	*vec_z;
	t_vec4	vec;

	vec_x = cJSON_GetArrayItem(value, 0);
	vec_y = cJSON_GetArrayItem(value, 1);
	vec_z = cJSON_GetArrayItem(value, 2);
	if (vec_x != NULL && vec_y != NULL && vec_z != NULL)
	{
		vec = (t_vec4){.x = vec_x->valuedouble,
					.y = vec_y->valuedouble, .z = vec_z->valuedouble, 0};
	}
	else
		handle_error("Wrong vector in .json!");
	return (vec);
}

t_vec2		json_parse_vec2(cJSON *value)
{
	cJSON	*vec_x;
	cJSON	*vec_y;
	t_vec2	vec;

	vec_x = cJSON_GetArrayItem(value, 0);
	vec_y = cJSON_GetArrayItem(value, 1);
	if (vec_x != NULL && vec_y != NULL)
	{
		vec = (t_vec2){.x = vec_x->valuedouble,
					.y = vec_y->valuedouble};
	}
	else
		handle_error("Wrong vector in .json!");
	return (vec);
}

int			parse_type(char *type)
{
	if (ft_strcmp(type, "sphere") == 0)
		return (obj_sphere);
	else if (ft_strcmp(type, "plane") == 0)
		return (obj_plane);
	else if (ft_strcmp(type, "cone") == 0)
		return (obj_cone);
	else if (ft_strcmp(type, "cylinder") == 0)
		return (obj_cylinder);
	else if (ft_strcmp(type, "point") == 0)
		return (light_point);
	else if (ft_strcmp(type, "directional") == 0)
		return (light_directional);
	return (obj_null);
}

void		parse_sphere_values(cJSON *json_object, t_object *object)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "position")) != NULL)
		object->position = json_parse_vec3(value);
	else
		handle_error("Error! Sphere has no position!");
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "radius")) != NULL)
		object->f_radius = value->valuedouble;
	else
		handle_error("Error! Sphere has no radius!");
}

void		parse_cone_values(cJSON *json_cone, t_object *cone)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_cone, "position")) != NULL)
		cone->position = json_parse_vec3(value);
	else
		handle_error("Error! Cone has no position!");
	if ((value = cJSON_GetObjectItemCaseSensitive(json_cone, "direction")) != NULL)
		cone->direction = json_parse_vec3(value);
	else
		handle_error("Error! Cone has no direction!");
	if ((value = cJSON_GetObjectItemCaseSensitive(json_cone, "radius")) != NULL)
		cone->f_radius = value->valuedouble;
	else
		handle_error("Error! Cone has no radius!");
	if ((value = cJSON_GetObjectItemCaseSensitive(json_cone, "height")) != NULL)
		cone->f_height = value->valuedouble;
	else
		handle_error("Error! Cone has no height!");
}

void		parse_plane_values(cJSON *json_object, t_object *object)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "position")) != NULL)
		object->position = json_parse_vec3(value);
	else
		handle_error("Error! Plane has no position!");

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "direction")) != NULL)
		object->direction = json_parse_vec3(value);
	else
		handle_error("Error! Plane has no direction!");
}

void		parse_cylinder_values(cJSON *json_object, t_object *object)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "position")) != NULL)
		object->position = json_parse_vec3(value);
	else
		handle_error("Error! Cylinder has no position!");
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "direction")) != NULL)
		object->direction = json_parse_vec3(value);
	else
		handle_error("Error! Cylinder has no direction!");
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "radius")) != NULL)
		object->f_radius = value->valuedouble;
	else
		handle_error("Error! Cylinder has no radius!");
}

void		parse_camera_values(cJSON *camera, t_rt *r)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(camera, "position")) != NULL)
		r->camera.position = json_parse_vec3(value);
	else
		handle_error("Error! Camera has no position!");
	if ((value = cJSON_GetObjectItemCaseSensitive(camera, "forward")) != NULL)
		r->camera.forward = json_parse_vec3(value);
	else
		handle_error("Error! Camera has no forward!");
	if ((value = cJSON_GetObjectItemCaseSensitive(camera, "up")) != NULL)
		r->camera.up = json_parse_vec3(value);
	else
		handle_error("Error! Camera has no up!");
}

int32_t		texture_index(char *texture_fname, t_rt *r)
{
	uint32_t	index;

	if (ft_strcmp(texture_fname, "chess") == 0)
		return (-1);
	if (ft_strcmp(texture_fname, "perlin") == 0)
		return (-2);
	if (ft_strcmp(texture_fname, "wave") == 0)
		return (-3);
	index = 0;
	while (index < r->n_textures)
	{
		if (ft_strcmp(r->texture_files[index], texture_fname) == 0)
			return (index);
		index++;
	}
	return (index);
}

void		parse_texture(cJSON *json_object, t_object *object, t_rt *r)
{
	cJSON	*value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "texture")) == NULL)
	{
		object->texture.index = -4;
		return;
	}
	object->texture.index = texture_index(value->valuestring, r);
	if (object->texture.index == (int32_t)r->n_textures)
	{
		r->texture_files[r->n_textures] = ft_strjoin("images/", value->valuestring);
		r->n_textures++;
	}
}

void		parse_optional_parameters(cJSON *json_object, t_object *object, t_rt *r)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "color")) != NULL)
		object->color = json_parse_vec3(value);
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "specular")) != NULL)
		object->f_metalness = value->valueint;
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "reflection")) != NULL)
		object->f_reflection = value->valuedouble;
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "refraction")) != NULL)
		object->f_refraction = value->valuedouble;
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "transparency")) != NULL)
		object->f_transparency = value->valuedouble;
	parse_texture(json_object, object, r);
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "tiling")) != NULL)
		object->f_tiling = json_parse_vec2(value);
	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "offset")) != NULL)
		object->f_offset = json_parse_vec2(value);
}

void		parse_point_values(cJSON *json_object, t_object *light)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "intensity")) != NULL)
		light->l_intensity = value->valuedouble;
	else
		handle_error("Error! Light point has no intensity!");

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "position")) != NULL)
		light->position = json_parse_vec3(value);
	else
		handle_error("Error! Light point has no position!");
}

void		parse_directional_values(cJSON *json_object, t_object *light)
{
	cJSON *value;

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "intensity")) != NULL)
		light->l_intensity = value->valuedouble;
	else
		handle_error("Error! Light point has no intensity!");

	if ((value = cJSON_GetObjectItemCaseSensitive(json_object, "direction")) != NULL)
		light->direction = json_parse_vec3(value);
	else
		handle_error("Error! Light point has no direction!");
}

void		set_defaults(t_object *object)
{
	object->f_reflection = 0.0f;
	object->f_refraction = 1.517f;
	object->f_transparency = 0.0f;
	object->f_tiling = (t_vec2){ .x = 1, .y = 1 };
	object->f_offset = (t_vec2){ .x = 0, .y = 0 };
}

void		read_scene(char *fname, t_rt *r)
{
	int32_t		fd;
	char		buffer[64 * Kb];
	cJSON		*json;
	cJSON		*tmp;
	cJSON		*object;
	t_object	*cur_object;

	if (ft_strcmp(fname + ft_strlen(fname) - 5, ".json") != 0
						|| (fd = open(fname, O_RDONLY)) == -1)
		handle_error("Wrong file!");
	read(fd, buffer, 64 * Kb);
	json = cJSON_Parse(buffer);
	if (json == NULL || buffer[0] != '{')
		handle_error("Wrong .json file!");

	tmp = cJSON_GetObjectItemCaseSensitive(json, "figures");
	r->n_fig = cJSON_GetArraySize(tmp);
	r->sbo_figures = ft_memalloc(sizeof(t_object) * r->n_fig);
	r->texture_files = ft_memalloc(sizeof(char*) * r->n_fig);
	object = tmp->child;
	cur_object = r->sbo_figures;
	while(object)
	{
		cJSON *type;
		if((type = cJSON_GetObjectItemCaseSensitive(object, "type")) == NULL ||
				(cur_object->type = (parse_type(type->valuestring))) == obj_null)
			handle_error("No type for object!");
		set_defaults(cur_object);
		if (cur_object->type == obj_sphere)
			parse_sphere_values(object, cur_object);
		else if (cur_object->type == obj_cone)
			parse_cone_values(object, cur_object);
		else if (cur_object->type == obj_plane)
			parse_plane_values(object, cur_object);
		else if (cur_object->type == obj_cylinder)
			parse_cylinder_values(object, cur_object);
		parse_optional_parameters(object, cur_object, r);
		object = object->next;
		cur_object++;
	}
	if ((tmp = cJSON_GetObjectItemCaseSensitive(json, "camera")) == NULL)
		handle_error("Error! File has no camera!");
	parse_camera_values(tmp, r);
	r->camera.right = vec4_cross(r->camera.up, r->camera.forward);
	tmp = cJSON_GetObjectItemCaseSensitive(json, "lights");
	r->n_lig = cJSON_GetArraySize(tmp);
	r->sbo_lights = ft_memalloc(sizeof(t_object) * r->n_lig);
	object = tmp->child;
	cur_object = r->sbo_lights;
	while(object)
	{
		cJSON *type;
		if((type = cJSON_GetObjectItemCaseSensitive(object, "type")) == NULL ||
				(cur_object->type = (parse_type(type->valuestring))) == obj_null)
			handle_error("No type for object!");
		if (cur_object->type == light_point)
			parse_point_values(object, cur_object);
		else if (cur_object->type == light_directional)
			parse_directional_values(object, cur_object);
		object = object->next;
		cur_object++;
	}
	cJSON_Delete(json);
}
