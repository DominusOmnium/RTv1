/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retracing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 14:23:15 by marvin            #+#    #+#             */
/*   Updated: 2020/03/14 14:23:15 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_d     CanvasToViewport(int i, int j, t_retr *r)
{
    t_d res;

    res.x = j * r->vw / WIN_WIDTH;
    res.y = i * r->vh / WIN_HEIGHT;
    res.z = r->d;
    return (res);
}

int     traceRay(t_retr *r)
{
    int closest_t;

    closest_t = 10000000;
}

void    retracing(t_retr *r)
{
    int i;
    int j;
    t_sphere one;
    t_sphere two;
    t_sphere three;

    //не забыть задавать начальные значения в другом месте
    r->vw = 1;
    r->vh = 1;
    r->d = 1;
    one = {(t_coord){0, -1, 3}, 1, (t_coord){255, 0, 0}};
    two = {(t_coord){2, 0, 4}, 1, (t_coord){0, 0, 255}};
    three = {(t_coord){-2, 0, 4}, 1, (t_coord){0, 255, 0}};


    r->o = [0, 0, 0];
    i = -(WIN_WIDTH / 2);
    while (i <= (WIN_WIDTH / 2))
    {
        j = -(WIN_HEIGHT / 2);
        while (j <= (WIN_HEIGHT / 2))
        {
            r->ds = CanvasToViewport(i, j, r);
            j++;
        }
        i++;
    }
}
