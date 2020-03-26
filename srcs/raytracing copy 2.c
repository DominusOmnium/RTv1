/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 14:23:15 by marvin            #+#    #+#             */
/*   Updated: 2020/03/25 12:04:51 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3	canvasToViewport(int i, int j, t_retr *r)
{
    t_vec3	res;

    res.x = j * r->vw / WIN_WIDTH;
    res.y = i * r->vh / WIN_HEIGHT;
    res.z = r->d;
    return (res);
}

void	intersectRaySphere(t_vec3 ds, t_vec3 o, t_object sphere, double *t1, double *t2)
{
	t_vec3	c;
	double	r;
	
	t_vec3	oc;
	double	k1;
	double	k2;
	double	k3;
	double 	discriminant;

	c = sphere.transform.position;
	r = ((t_sphere*)(sphere.obj))->radius;
	oc = vec3d_sub_vec3d(o, c);
	k1 = vec3d_scalar(ds, ds);
	k2 = vec3d_scalar(oc, ds) * 2;
	k3 = vec3d_scalar(oc, oc) - r * r;
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

double	intersect_ray_plane(t_vec3 o, t_vec3 dir, t_object plane)
{
	double		a;
	double		b;

	a = vec3d_scalar(dir, ((t_plane*)plane.obj)->normal);
	if (fabs(a) < 0.0003f)
		return (0);
	b = -vec3d_scalar(vec3d_sub_vec3d(o, plane.transform.position), ((t_plane*)plane.obj)->normal) / a;
	return (b < 0.0003f) ? 0 : b;
}

t_object	closestIntersection(t_retr *r, t_vec3 ds, t_vec3 o)
{
	t_object	obj;
	int			i;
	double		t1;
	double		t2;
	
	r->t_c.closest_t = DBL_MAX;
	obj.type = obj_null;
	i = 0;
	while (i < r->n_fig)
	{
		t2 = DBL_MAX;
		if ((r->figures)[i].type == obj_sphere)
			intersectRaySphere(ds, o, (r->figures)[i], &t1, &t2);
		else if ((r->figures)[i].type == obj_plane)
			t1 = intersect_ray_plane(o, ds, (r->figures)[i]);
		if ((r->t_c.t_min < t1 && r->t_c.t_max > t1) && t1 < r->t_c.closest_t)
		{
			r->t_c.closest_t = t1;
			obj = (r->figures)[i];
		}
		if ((r->t_c.t_min < t2 && r->t_c.t_max > t2) && t2 < r->t_c.closest_t)
		{
			r->t_c.closest_t = t2;
			obj = (r->figures)[i];
		}
		i++;
	}
	return (obj);
}

double	computeLighting(t_vec3 p, t_vec3 n, t_retr *r, int s)
{
	double		res;
	int			i;
	t_vec3		l;
	t_vec3		v;
	double		n_scal_l;
	double		shadow_t;
	t_object	shadow_obj;
	double		t_max;

	v = vec3d_mul_d(r->ds, -1.0);
	res = 0.0;
	i = 0;
	while (i < r->n_lig)
	{
		if ((r->lights)[i].type == light_ambient)
			res += (r->lights)[i].intensity;
		else 
		{
			if ((r->lights)[i].type == light_point)
			{
				l = vec3d_sub_vec3d((r->lights)[i].position, p);
				t_max = 1;
			}
			else
			{
				l = (r->lights)[i].direction;
				t_max = DBL_MAX;
			}
			
			//Проверка тени
			r->t_c.t_min = 0.001;
			r->t_c.t_max = t_max;
			shadow_obj = closestIntersection(r, l, p);
			if (shadow_obj.type != obj_null)
			{
				i++;
				continue;
			}
			
			//Диффузность
			n_scal_l = vec3d_scalar(n, l);
			if (n_scal_l > 0)
				res += (r->lights)[i].intensity * n_scal_l / (vec3d_mod(n) * vec3d_mod(l));

			//Зеркальность
			if (s != -1)
			{
				t_vec3 r_v = vec3d_sub_vec3d(vec3d_mul_d(n, (vec3d_scalar(n, l) * 2.0)), l);
				double rv_scal_v = vec3d_scalar(r_v, v);
				if (rv_scal_v > 0)
					res += (r->lights)[i].intensity * pow(rv_scal_v / (vec3d_mod(r_v) * vec3d_mod(v)), s);
			}
		}
		i++;
	}
	if (res >= 1)
		res = 1.0;
	return (res);
}

t_vec3	traceRay(t_retr *r, t_vec3 ds, t_vec3 o)
{
	int			i;
	t_vec3		norm;
	t_vec3		p;
	t_object	obj;
	double		cL;

	obj = closestIntersection(r, ds, o);
	if (obj.type == obj_null)
		return ((t_vec3){255, 255, 255});
	//printf("%Lf, %Lf, %Lf\n", r->o.x, r->o.y, r->o.z);
	//getchar();
	p = vec3d_add_vec3d(vec3d_mul_d(ds, r->t_c.closest_t), o);
	if (obj.type == obj_sphere)
	{
		norm = vec3d_sub_vec3d(p, obj.transform.position);
		norm = vec3d_mul_d(norm, 1.0/vec3d_mod(norm));	
	}
	else if (obj.type == obj_plane)
		norm = ((t_plane*)obj.obj)->normal;
	cL = computeLighting(p, norm, r, obj.specular);	
	return (vec3d_mul_d(obj.color, cL));
}

int		color_int(t_vec3 c)
{
	int res;

	res = 0 << 24 | (int)(c.x) << 16 | (int)(c.y) << 8 | (int)(c.z);
	return (res);
}

void    raytracing(t_retr *r, t_app *app)
{
    int		i;
    int		j;
	t_vec3	color;
	int		col;

    i = 0;
    while (i < WIN_HEIGHT)
    {
        j = 0;
        while (j < WIN_WIDTH)
        {
            //r->ds = vec3d_mul_vec3d(r->camera.rotation, canvasToViewport(WIN_HEIGHT/2 - i, j - WIN_WIDTH/2, r));
			r->ds = canvasToViewport(WIN_HEIGHT/2 - i, j - WIN_WIDTH/2, r);
			r->t_c.t_min = 1.0;
			r->t_c.t_max = DBL_MAX;
			//r->o = r->camera.position;
			color = traceRay(r, r->ds, r->o);
			col = color_int(color);
			((int*)(app->vulkan.buf.mem_ptr))[i * WIN_WIDTH + j] = col;
            j++;
        }
        i++;
    }
}
