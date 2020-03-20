/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 14:23:15 by marvin            #+#    #+#             */
/*   Updated: 2020/03/20 13:02:26 by dkathlee         ###   ########.fr       */
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

t_coord	minus_v_and_v(t_coord one, t_coord two)
{
	t_coord res;

	res.x = one.x - two.x;
	res.y = one.y - two.y;
	res.z = one.z - two.z;
	return (res);
}

t_coord	plus_v_and_v(t_coord one, t_coord two)
{
	t_coord res;

	res.x = one.x + two.x;
	res.y = one.y + two.y;
	res.z = one.z + two.z;
	return (res);
}

t_coord	plus_v_and_num(t_coord one, double two)
{
	t_coord res;

	res.x = one.x + two;
	res.y = one.y + two;
	res.z = one.z + two;
	return (res);
}

t_coord	mul_v_and_v(t_coord one, t_coord two)
{
	t_coord res;

	res.x = one.x * two.x;
	res.y = one.y * two.y;
	res.z = one.z * two.z;
	return (res);
}

t_coord	mul_v_and_num(t_coord one, double two)
{
	t_coord res;

	res.x = one.x * two;
	res.y = one.y * two;
	res.z = one.z * two;
	return (res);
}

double	mod_v(t_coord one)
{
	return (sqrt(pow(one.x, 2) + pow(one.y, 2) + pow(one.z, 2)));
}

double		scalarVectors(t_coord one, t_coord two)
{
	double res;
	
	res = one.x * two.x + one.y * two.y + one.z * two.z;
	return (res);
}

void	intersectRaySphere(t_retr *r, t_sphere sphere, double *t1, double *t2)
{
	t_coord	c;
	double	rad;
	t_coord oc;
	double	k1;
	double	k2;
	double	k3;
	double 	discriminant;

	c = sphere.center;
	rad = sphere.radius;
	oc = minus_v_and_v(r->o, c);
	k1 = scalarVectors(r->ds, r->ds);
	k2 = scalarVectors(oc, r->ds) * 2;
	k3 = scalarVectors(oc, oc) - rad * rad;
	discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0)
	{
		*t1 = DBL_MAX;
		*t2 = DBL_MAX;
	}
	else
	{
		*t1 = (-k2 + sqrt(discriminant)) / (2 * k1);
		*t2 = (-k2 - sqrt(discriminant)) / (2 * k1);
	}
}

double	computeLighting(t_coord p, t_coord n, t_retr *r)
{
	double	res;
	int		i;
	t_coord	l;
	double	n_scal_l;

	res = 0.0;
	i = 0;
	while (i < r->n_lig)
	{
		if ((r->lights)[i].type == 'a')
			res += (r->lights)[i].intensity;
		else 
		{
			if ((r->lights)[i].type == 'p')
				l = minus_v_and_v((r->lights)[i].position, p);
			else
				l = (r->lights)[i].direction;
			n_scal_l = scalarVectors(n, l);
			if (n_scal_l > 0)
				res += (r->lights)[i].intensity * n_scal_l / (mod_v(n) * mod_v(l));
		}
		i++;
	}
	return (res);
}

t_coord	traceRay(t_retr *r, double t_min, double t_max)
{
    double		closest_t;
	t_sphere	sphere;
	int			n;
	int			i;
	double		t1;
	double		t2;

    closest_t = DBL_MAX;
	sphere = (t_sphere){(t_coord){0, 0, 0}, 0, (t_coord){0, 0, 0}};
	i = 0;
	n = 0;
	while (i < r->n_fig)
	{
		intersectRaySphere(r, (r->figures)[i], &t1, &t2);
		if ((t_min < t1 && t_max > t1) && t1 < closest_t)
		{
			closest_t = t1;
			sphere = (r->figures)[i];
			n++;
		}
		else if ((t_min < t2 && t_max > t2) && t2 < closest_t)
		{
			closest_t = t2;
			sphere = (r->figures)[i];
			n++;
		}
		i++;
	}
	if (n == 0)
		return ((t_coord){255, 255, 255});
	
	
	t_coord p = plus_v_and_v(mul_v_and_num(r->ds, closest_t), r->o);
	t_coord norm = minus_v_and_v(p, sphere.center);
	norm = mul_v_and_num(norm, (1 / mod_v(norm)));
	
	return (sphere.color);
	return (mul_v_and_num(sphere.color, computeLighting(p, norm, r)));
}

int		color_int(t_coord c)
{
	int res;

	res = 0 << 24 | (int)(c.x) << 16 | (int)(c.y) << 8 | (int)(c.z);
	return (res);
}

void    raytracing(t_retr *r, t_app *app)
{
    int i;
    int j;
	t_coord color;

    //не забыть задавать начальные значения в другом месте
	r->n_fig = 4;
	r->n_lig = 0;
	r->figures = (t_sphere*)malloc(sizeof(t_sphere) * r->n_fig);
	r->lights = (t_light*)malloc(sizeof(t_light) * r->n_lig);
    r->vw = 1;
    r->vh = 1;
    r->d = 1;
    (r->figures)[0] = (t_sphere){(t_coord){0, -1.0, 3.0}, 1.0, (t_coord){255, 0, 0}};
    (r->figures)[1] = (t_sphere){(t_coord){2.0, 0, 4.0}, 1.0, (t_coord){0, 0, 255}};
    (r->figures)[2] = (t_sphere){(t_coord){-2.0, 0, 4.0}, 1.0, (t_coord){0, 255, 0}};
	(r->figures)[3] = (t_sphere){(t_coord){0, -5001.0, 0}, 5000, (t_coord){255, 255, 0}};
	/*(r->lights)[0] = (t_light){'a', 0.2, (t_coord){0, 0, 0}, (t_coord){0, 0, 0}};
	(r->lights)[1] = (t_light){'p', 0.6, (t_coord){2, 1, 0}, (t_coord){0, 0, 0}};
	(r->lights)[2] = (t_light){'d', 0.2, (t_coord){0, 0, 0}, (t_coord){1, 4, 4}};*/
    r->o = (t_coord){0, 0, 0};

    i = 0;
    while (i < WIN_HEIGHT)
    {
        j = 0;
        while (j < WIN_WIDTH)
        {
            r->ds = CanvasToViewport(WIN_HEIGHT/2 - i, j - WIN_WIDTH/2, r);
			color = traceRay(r, 1.0, DBL_MAX);
			int col = color_int(color);
			((int*)(app->vulkan.buf.mem_ptr))[i * WIN_WIDTH + j] = col;
            j++;
        }
        i++;
    }
}
