/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_reader_json.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 17:57:40 by marvin            #+#    #+#             */
/*   Updated: 2020/11/23 17:57:40 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

int			couting_elements(cJSON *objects)
{
	int n;
	cJSON *new;

	new = objects;
	n = 0;
	//Уточнить как проверять до последнего элемента
	while (new->next != NULL)
		n++;
	
	return (n);
}

void		read_scene(char *fname, t_rt *r)
{
	int32_t		fd;
	char		*str;
	char		buffer[64 * Kb];
	cJSON		*json;
	cJSON		*objects;
	int32_t		gnl;
	t_object	*cur_figure;
	t_object	*cur_light;

	if (ft_strcmp(fname + ft_strlen(fname) - 5, ".json") != 0
						|| (fd = open(fname, O_RDONLY)) == -1)
		handle_error("Wrong file!");
	read(fd, buffer, 64 * Kb);
	json = cJSON_Parse(buffer);
	if (json == NULL || buffer[0] != '{')
		handle_error("Wrong .json file!");
	objects = cJSON_GetObjectItemCaseSensitive(json, "objects");
}

