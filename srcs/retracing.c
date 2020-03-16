/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   retracing.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celva <celva@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 14:23:15 by marvin            #+#    #+#             */
/*   Updated: 2020/03/16 17:49:37 by celva            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_coord    CanvasToViewport(int i, int j, t_retr *r)
{
    t_coord		res;

    res.x = j * r->vw / WIN_WIDTH;
    res.y = i * r->vh / WIN_HEIGHT;
    res.z = r->d;
    return (res);
}

t_coord	oper_on_vectors(t_coord one, t_coord two)
{
	t_coord res;

	res.x = one.x - two.x;
	res.y = one.y - two.y;
	res.z = one.z - two.z;
	return (res);
}

int		scalarVectors(t_coord one, t_coord two)
{
	int res;
	
	res = one.x * two.x + one.y * two.y + one.z * two.z;
	return (res);
}

void	intersectRaySphere(t_retr *r, t_sphere sphere, double *t1, double *t2)
{
	t_coord	c;
	int		rad;
	t_coord oc;
	int		k1;
	int		k2;
	int		k3;
	int 	discriminant;

	c = sphere.center;
	rad = sphere.radius;
	oc = oper_on_vectors(r->o, c);
	k1 = scalarVectors(r->ds, r->ds);
	k2 = scalarVectors(oc, r->ds) * 2;
	k3 = scalarVectors(oc, oc) - rad * rad;
	discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0)
	{
		*t1 = 2147483647;
		*t2 = 2147483647;
	}
	else
	{
		*t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
		*t2 = (-k2 - sqrt(discriminant)) / (2*k1);
	}
}

t_coord	traceRay(t_retr *r, int t_min, int t_max)
{
    int			closest_t;
	t_sphere	sphere;
	int			n;
	int			i;
	double		t1;
	double		t2;

    closest_t = 2147483647;
	sphere = (t_sphere){(t_coord){0, 0, 0}, 0, (t_coord){0, 0, 0}};
	i = 0;
	n = 0;
	while (i < r->n_fig)
	{
		intersectRaySphere(r, r->figures[i], &t1, &t2);
		if ((t_min < t1 && t_max > t1) && t1 < closest_t)
		{
			closest_t = t1;
			sphere = r->figures[i];
			n++;
		}
		if ((t_min < t2 && t_max > t2) && t2 < closest_t)
		{
			closest_t = t2;
			sphere = r->figures[i];
			n++;
		}
	}
	if (n == 0)
		return ((t_coord){255, 255, 255});
	return (sphere.color);
}

void    retracing(t_retr *r)
{
    int i;
    int j;
	t_coord color;

    //не забыть задавать начальные значения в другом месте
	r->n_fig = 3;
	r->figures = (t_sphere*)malloc(sizeof(t_sphere) * r->n_fig);
    r->vw = 1;
    r->vh = 1;
    r->d = 1;
    r->figures[0] = (t_sphere){(t_coord){0, -1, 3}, 1, (t_coord){255, 0, 0}};
    r->figures[1] = (t_sphere){(t_coord){2, 0, 4}, 1, (t_coord){0, 0, 255}};
    r->figures[2] = (t_sphere){(t_coord){-2, 0, 4}, 1, (t_coord){0, 255, 0}};
    r->o = (t_coord){0, 0, 0};

	
    i = -(WIN_WIDTH / 2);
    while (i <= (WIN_WIDTH / 2))
    {
        j = -(WIN_HEIGHT / 2);
        while (j <= (WIN_HEIGHT / 2))
        {
            r->ds = CanvasToViewport(i, j, r);
			color = traceRay(r, 1, 2147483647);
			
            j++;
        }
        i++;
    }
}
