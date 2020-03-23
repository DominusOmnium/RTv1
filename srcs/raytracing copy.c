/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing copy.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 14:23:15 by marvin            #+#    #+#             */
/*   Updated: 2020/03/23 16:03:58 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3    CanvasToViewport(int i, int j, t_retr *r)
{
    t_vec3		res;

    res.x = j * r->vw / WIN_WIDTH;
    res.y = i * r->vh / WIN_HEIGHT;
    res.z = r->d;
    return (res);
}

t_vec3	minus_v_and_v(t_vec3 one, t_vec3 two)
{
	t_vec3 res;

	res.x = one.x - two.x;
	res.y = one.y - two.y;
	res.z = one.z - two.z;
	return (res);
}

t_vec3	plus_v_and_v(t_vec3 one, t_vec3 two)
{
	t_vec3 res;

	res.x = one.x + two.x;
	res.y = one.y + two.y;
	res.z = one.z + two.z;
	return (res);
}

t_vec3	plus_v_and_num(t_vec3 one, double two)
{
	t_vec3 res;

	res.x = one.x + two;
	res.y = one.y + two;
	res.z = one.z + two;
	return (res);
}

t_vec3	mul_v_and_v(t_vec3 one, t_vec3 two)
{
	t_vec3 res;

	res.x = one.x * two.x;
	res.y = one.y * two.y;
	res.z = one.z * two.z;
	return (res);
}

t_vec3	mul_v_and_num(t_vec3 one, double two)
{
	t_vec3 res;

	res.x = one.x * two;
	res.y = one.y * two;
	res.z = one.z * two;
	return (res);
}

double	mod_v(t_vec3 one)
{
	return (sqrt(pow(one.x, 2) + pow(one.y, 2) + pow(one.z, 2)));
}

double		scalarVectors(t_vec3 one, t_vec3 two)
{
	double res;
	
	res = one.x * two.x + one.y * two.y + one.z * two.z;
	return (res);
}

void	intersectRaySphere(t_retr *r, t_sphere sphere, double *t1, double *t2)
{
	t_vec3	c;
	double	rad;
	t_vec3 oc;
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

t_sphere	closestIntersection(t_retr *r, double t_min, double t_max, int *n, double *closest_t)
{
	t_sphere	sphere;
	int			i;
	double		t1;
	double		t2;
	
	*closest_t = DBL_MAX;
	*n = 0;
	sphere = (t_sphere){(t_vec3){0, 0, 0}, 0, (t_vec3){0, 0, 0}};
	i = 0;
	while (i < r->n_fig)
	{
		intersectRaySphere(r, (r->figures)[i], &t1, &t2);
		if ((t_min < t1 && t_max > t1) && t1 < *closest_t)
		{
			*closest_t = t1;
			sphere = (r->figures)[i];
			(*n)++;
		}
		if ((t_min < t2 && t_max > t2) && t2 < *closest_t)
		{
			*closest_t = t2;
			sphere = (r->figures)[i];
			(*n)++;
		}
		i++;
	}
	return (sphere);
}

double	computeLighting(t_vec3 p, t_vec3 n, t_retr *r, int s)
{
	double		res;
	int			i;
	//t_coord		l;
	t_vec3		v;
	double		n_scal_l;
	double		shadow_t;
	t_sphere	shadow_sphere;
	int			n_sphere;
	double		t_max;

	v = mul_v_and_num(r->ds, -1.0);
	res = 0.0;
	i = 0;
	while (i < r->n_lig)
	{
		if ((r->lights)[i].type == 'a')
			res += (r->lights)[i].intensity;
		else 
		{
			if ((r->lights)[i].type == 'p')
			{
				//l = minus_v_and_v((r->lights)[i].position, p);
				r->ds = minus_v_and_v((r->lights)[i].position, r->o);
				t_max = 1;
			}
			else
			{
				//l = (r->lights)[i].direction;
				r->ds = (r->lights)[i].direction;
				t_max = DBL_MAX;
			}
			
			//Проверка тени
			shadow_sphere = closestIntersection(r, 0.001, t_max, &n_sphere, &shadow_t);
			if (n_sphere != 0)
			{
				i++;
				continue;
			}
			
			//Диффузность
			//n_scal_l = scalarVectors(n, l);
			n_scal_l = scalarVectors(n, r->ds);
			if (n_scal_l > 0)
				res += (r->lights)[i].intensity * n_scal_l / (mod_v(n) * mod_v(r->ds));
				//res += (r->lights)[i].intensity * n_scal_l / (mod_v(n) * mod_v(l));

			//Зеркальность
			if (s != -1)
			{
				//t_coord r_v = minus_v_and_v(mul_v_and_num(n, (scalarVectors(n, l) * 2.0)), l);
				t_vec3 r_v = minus_v_and_v(mul_v_and_num(n, (scalarVectors(n, r->ds) * 2.0)), r->ds);
				double rv_scal_v = scalarVectors(r_v, v);
				if (rv_scal_v > 0)
					res += (r->lights)[i].intensity * pow(rv_scal_v / (mod_v(r_v) * mod_v(v)), s);
			}
		}
		i++;
	}
	if (res >= 1)
		res = 1.0;
	return (res);
}

t_vec3	traceRay(t_retr *r, double t_min, double t_max)
{
    double		closest_t;
	int			n;
	int			i;
	t_sphere	sphere;

	sphere = closestIntersection(r, t_min, t_max, &n, &closest_t);
	if (n == 0)
		return ((t_vec3){255, 255, 255});
	
	
	//t_coord p = plus_v_and_v(mul_v_and_num(r->ds, closest_t), r->o);
	r->o = plus_v_and_v(mul_v_and_num(r->ds, closest_t), r->o);
	//t_coord norm = minus_v_and_v(p, sphere.center);
	t_vec3 norm = minus_v_and_v(r->o, sphere.center);
	norm = mul_v_and_num(norm, (1 / mod_v(norm)));
	
	//return (mul_v_and_num(sphere.color, computeLighting(p, norm, r, sphere.specular)));
	return (mul_v_and_num(sphere.color, computeLighting(r->o, norm, r, sphere.specular)));
}

int		color_int(t_vec3 c)
{
	int res;

	res = 0 << 24 | (int)(c.x) << 16 | (int)(c.y) << 8 | (int)(c.z);
	return (res);
}

void    raytracing(t_retr *r, t_app *app)
{
    int i;
    int j;
	t_vec3 color;

    //не забыть задавать начальные значения в другом месте
	r->n_fig = 4;
	r->n_lig = 3;
	r->figures = (t_sphere*)malloc(sizeof(t_sphere) * r->n_fig);
	r->lights = (t_light*)malloc(sizeof(t_light) * r->n_lig);
    r->vw = 1.5;
    r->vh = 1;
    r->d = 1;
    (r->figures)[0] = (t_sphere){(t_vec3){0, -1.0, 3.0}, 1.0, (t_vec3){255, 0, 0}, 500};
    (r->figures)[1] = (t_sphere){(t_vec3){2.0, 0, 4.0}, 1.0, (t_vec3){0, 0, 255}, 500};
    (r->figures)[2] = (t_sphere){(t_vec3){-2.0, 0, 4.0}, 1.0, (t_vec3){0, 255, 0}, 10};
	(r->figures)[3] = (t_sphere){(t_vec3){0, -5001.0, 0}, 5000, (t_vec3){255, 255, 0}, 1000};
	(r->lights)[0] = (t_light){'a', 0.2, (t_vec3){0, 0, 0}, (t_vec3){0, 0, 0}};
	(r->lights)[1] = (t_light){'p', 0.6, (t_vec3){2, 1, 0}, (t_vec3){0, 0, 0}};
	(r->lights)[2] = (t_light){'d', 0.2, (t_vec3){0, 0, 0}, (t_vec3){1, 4, 4}};
    r->o = (t_vec3){0, 0, 0};

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
