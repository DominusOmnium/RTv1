/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raytracing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/14 14:23:15 by marvin            #+#    #+#             */
/*   Updated: 2020/08/01 00:26:08 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"

t_vec3	canvas_to_viewport(int i, int j, t_retr *r)
{
    t_vec3	res;

    res.x = j * r->vw / WIN_WIDTH;
    res.y = i * r->vh / WIN_HEIGHT;
    res.z = r->d;
    return (res);
}

void	intersect_ray_sphere(t_vec3 ds, t_vec3 o, t_object sphere, double *t1, double *t2)
{
	t_vec3	c;
	double	r;
	
	t_vec3	oc;
	double	k1;
	double	k2;
	double	k3;
	double 	discriminant;

	c = sphere.transform.position;
	r = sphere.figures.radius;
	//r = ((t_sphere*)(sphere.obj))->radius;
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

void	intersect_ray_cone(t_vec3 ds, t_vec3 o, /*t_object obj*/ t_figure cone, double *t1, double *t2)
{
	t_vec3	c;
	t_vec3	ov;
	//t_cone	cone;
	double	cos;
	double	k1;
	double	k2;
	double	k3;
	double 	discriminant;

	//cone = *((t_cone*)obj.obj);
	cos = cone.height / sqrt(cone.height * cone.height + cone.radius * cone.radius);
	ov = vec3d_sub_vec3d(cone.ver, o);
	k1 = cos * cos * vec3d_scalar(ds, ds) - vec3d_scalar(ds, cone.direction) * vec3d_scalar(ds, cone.direction);
	k2 = 2 * (vec3d_scalar(ov, cone.direction) * vec3d_scalar(ds, cone.direction) - cos * cos * vec3d_scalar(ds, ov));
	k3 = vec3d_scalar(ov, ov) * cos * cos - vec3d_scalar(ov, cone.direction) * vec3d_scalar(ov, cone.direction);
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
	if ((vec3d_scalar(o, cone.direction) + *t1 * vec3d_scalar(ds, cone.direction) - vec3d_scalar(cone.ver, cone.direction)) > 0)
		*t1 = DBL_MAX;
	if ((vec3d_scalar(o, cone.direction) + *t2 * vec3d_scalar(ds, cone.direction) - vec3d_scalar(cone.ver, cone.direction)) > 0)
		*t2 = DBL_MAX;
}

void	intersect_ray_cylinder(t_vec3 ds, t_vec3 o, t_object cyl, double *t1, double *t2)
{
	t_vec3	c;
	double	r;
	t_vec3	oc;
	t_vec3	v;
	double	k1;
	double	k2;
	double	k3;
	double 	discriminant;

	c = cyl.transform.position;
	r = cyl.figures.radius;
	v = cyl.figures.direction;
	//r = ((t_cylinder*)(cyl.obj))->radius;
	//v = ((t_cylinder*)(cyl.obj))->direction;
	oc = vec3d_sub_vec3d(o, c);
	k1 = vec3d_scalar(vec3d_cross(ds, v), vec3d_cross(ds, v));
	k2 = 2 * vec3d_scalar(vec3d_cross(ds, v), vec3d_cross(oc, v));
	k3 = vec3d_scalar(vec3d_cross(oc, v), vec3d_cross(oc, v)) - r * r;
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
	t_figure	p;
	//t_plane		p;
	t_vec3		col;

	p = plane.figures;
	//p = *((t_plane*)plane.obj);
	a = vec3d_scalar(dir, p.direction);
	//a = vec3d_scalar(dir, p.normal);
	if (fabs(a) < 0.0003f)
		return (DBL_MAX);
	b = -vec3d_scalar(vec3d_sub_vec3d(o, plane.transform.position), p.direction) / a;
	//b = -vec3d_scalar(vec3d_sub_vec3d(o, plane.transform.position), p.normal) / a;
	col = vec3d_add_vec3d(o, vec3d_mul_d(dir, b));
	if (b < 0.0003f)
		return (DBL_MAX);
	if (vec3d_scalar(p.direction, vec3d_cross(vec3d_sub_vec3d(p.vertices[1], p.vertices[0]), vec3d_sub_vec3d(col, p.vertices[0]))) > 0 &&
		vec3d_scalar(p.direction, vec3d_cross(vec3d_sub_vec3d(p.vertices[2], p.vertices[1]), vec3d_sub_vec3d(col, p.vertices[1]))) > 0 &&
		vec3d_scalar(p.direction, vec3d_cross(vec3d_sub_vec3d(p.vertices[3], p.vertices[2]), vec3d_sub_vec3d(col, p.vertices[2]))) > 0 &&
		vec3d_scalar(p.direction, vec3d_cross(vec3d_sub_vec3d(p.vertices[0], p.vertices[3]), vec3d_sub_vec3d(col, p.vertices[3]))) > 0)
		return (b);
	/*if (vec3d_scalar(p.normal, vec3d_cross(vec3d_sub_vec3d(p.vertices[1], p.vertices[0]), vec3d_sub_vec3d(col, p.vertices[0]))) > 0 &&
		vec3d_scalar(p.normal, vec3d_cross(vec3d_sub_vec3d(p.vertices[2], p.vertices[1]), vec3d_sub_vec3d(col, p.vertices[1]))) > 0 &&
		vec3d_scalar(p.normal, vec3d_cross(vec3d_sub_vec3d(p.vertices[3], p.vertices[2]), vec3d_sub_vec3d(col, p.vertices[2]))) > 0 &&
		vec3d_scalar(p.normal, vec3d_cross(vec3d_sub_vec3d(p.vertices[0], p.vertices[3]), vec3d_sub_vec3d(col, p.vertices[3]))) > 0)
		return (b);*/
	return (DBL_MAX);
}

t_object	closest_intersection(t_retr *r, t_vec3 ds, t_vec3 o)
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
		//ft_printf("%lf, %lf, %lf\n", (r->figures)[i].transform.position.x, (r->figures)[i].transform.position.y, (r->figures)[i].transform.position.z);
		t2 = DBL_MAX;
		if ((r->figures)[i].type == obj_sphere)
			intersect_ray_sphere(ds, o, (r->figures)[i], &t1, &t2);
		else if ((r->figures)[i].type == obj_plane)
			t1 = intersect_ray_plane(o, ds, (r->figures)[i]);
		else if ((r->figures)[i].type == obj_cylinder)
			intersect_ray_cylinder(ds, o, (r->figures)[i], &t1, &t2);
		else if ((r->figures)[i].type == obj_cone)
			intersect_ray_cone(ds, o, /*(r->figures)[i]*/ (r->figures)[i].figures, &t1, &t2);
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
	//getchar();
	return (obj);
}

double	compute_lighting(t_vec3 p, t_vec3 n, t_retr *r, int s)
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
			r->t_c.t_min = 0.0000001;
			r->t_c.t_max = t_max;
			shadow_obj = closest_intersection(r, l, p);
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

t_vec3	trace_ray(t_retr *r)
{
	int			i;
	t_vec3		norm;
	t_vec3		p;
	t_object	obj;
	double		cL;

	obj = closest_intersection(r, r->ds, r->camera.position);
	if (obj.type == obj_null)
		return ((t_vec3){255, 255, 255});
	p = vec3d_add_vec3d(r->camera.position, vec3d_mul_d(r->ds, r->t_c.closest_t));
	if (obj.type == obj_sphere)
	{
		norm = vec3d_sub_vec3d(p, obj.transform.position);
		norm = vec3d_mul_d(norm, 1.0/vec3d_mod(norm));	
	}
	else if (obj.type == obj_plane)
	{
		if (vec3d_scalar(r->ds, obj.figures.direction) < 0)
			norm = obj.figures.direction;
		else
			norm = vec3d_mul_d(obj.figures.direction, -1);
	}
	else if (obj.type == obj_cylinder)
	{
		norm = vec3d_mul_d(vec3d_sub_vec3d(vec3d_mul_d(obj.figures.direction,
			vec3d_scalar(vec3d_sub_vec3d(obj.transform.position, p), obj.figures.direction)),
			vec3d_sub_vec3d(obj.transform.position, p)), 1 / vec3d_mod(obj.figures.direction));
	}
	else if (obj.type == obj_cone)
	{
		t_vec3	pc = vec3d_sub_vec3d(p, obj.transform.position);
		t_vec3	pv = vec3d_sub_vec3d(p, obj.figures.ver);
		double	cos = obj.figures.height / sqrt(obj.figures.height * obj.figures.height + obj.figures.radius * obj.figures.radius);
		norm = vec3d_add_vec3d(obj.transform.position, vec3d_mul_d(obj.figures.direction, obj.figures.height - vec3d_mod(pv) / cos));
		norm = vec3d_sub_vec3d(p, norm);
		norm = vec3d_mul_d(norm, 1 / vec3d_mod(norm));
	}
	cL = compute_lighting(p, norm, r, obj.specular);
	return (vec3d_mul_d(obj.color, cL));
}

int		color_int(t_vec3 c)
{
	int res;

	res = 0 << 24 | (int)(c.x) << 16 | (int)(c.y) << 8 | (int)(c.z);
	return (res);
}

t_vec3	rotation_axis(double angle, t_vec3 axis, t_vec3 p)
{
	t_vec3 res;
	double c;
	double s;

	c = cos(angle);
	s = sin(angle);
	res.x = p.x * (c + axis.x * axis.x * (1 - c)) + p.y * (axis.y * axis.x * (1 - c) + axis.z * s) + p.z * (axis.z * axis.x * (1 - c) - axis.y * s);
	res.y = p.x * (axis.x * axis.y * (1 - c) - axis.z * s) + p.y * (c + axis.y * axis.y * (1 - c)) + p.z * (axis.z * axis.y * (1 - c) + axis.x * s);
	res.z = p.x * (axis.x * axis.z * (1 - c) + axis.y * s) + p.y * (axis.y * axis.z* (1 - c) - axis.x * s) + p.z * (c + axis.z * axis.z * (1 - c));
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
			r->ds = canvas_to_viewport(WIN_HEIGHT/2 - i, j - WIN_WIDTH/2, r);
			r->ds = rotation_axis(r->camera.rotation.z, (t_vec3){0, 1, 0}, r->ds);
			r->t_c.t_min = 1.0;
			r->t_c.t_max = DBL_MAX;
			color = trace_ray(r);
			col = color_int(color);
			((int*)(app->vulkan.buf.mem_ptr))[i * WIN_WIDTH + j] = col;
            j++;
        }
        i++;
    }
}
