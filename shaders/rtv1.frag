#version 450
#extension GL_ARB_separate_shader_objects : enable

#define FLT_MAX 3.402823466e+38
#define FLT_MIN 1.175494351e-38
#define DBL_MAX 1.7976931348623158e+308
#define DBL_MIN 2.2250738585072014e-308

layout(location = 0) out vec4 outColor;

const uint obj_null		= 0x00000000u;
const uint obj_sphere	= 0x00000001u;
const uint obj_plane	= 0x00000002u;
const uint obj_cone		= 0x00000003u;
const uint obj_cylinder	= 0x00000004u;

const uint light_ambient		= 0x00000005u;
const uint light_point			= 0x00000006u;
const uint light_directional	= 0x00000007u;

struct							s_transform
{
	vec4						position;
	vec4						rotation;
}								t_transform;

struct							s_closest
{
	float						t_min;
	float						t_max;
	float						closest_t;
};

struct							s_retr
{
	vec3						ds;
	float						t_min;
	float						t_max;
	s_closest					t_c;
};

struct							s_input
{
	uint						type;
	int							f_specular;
	float 						f_radius;
	float						f_height;
	vec4						direction;
	vec4						f_ver;
	vec4						f_vertices[4];
	s_transform					transform;
	vec4						color;
	float						l_intensity;
	float						q[3];
};

layout(std430, binding = 0) readonly buffer Figures
{
	s_transform	camera;
	float		win_width;
	float		win_height;
	float		d;
	float		vh;
	float		vw;
	uint		n_fig;
	uint		n_lig;
	s_input		sbo_input[];
};

s_retr r;

vec3	canvas_to_viewport(int i, int j)
{
    vec3	res;
	
    res.x = j * vw / win_width;
    res.y = i * vh / win_height;
    res.z = d;
    return (res);
}

vec3	rotation_axis(float angle, vec3 axis, vec3 p)
{
	vec3 res;
	float c;
	float s;

	c = cos(angle);
	s = sin(angle);
	res.x = p.x * (c + axis.x * axis.x * (1 - c)) + p.y * (axis.y * axis.x * (1 - c) + axis.z * s) + p.z * (axis.z * axis.x * (1 - c) - axis.y * s);
	res.y = p.x * (axis.x * axis.y * (1 - c) - axis.z * s) + p.y * (c + axis.y * axis.y * (1 - c)) + p.z * (axis.z * axis.y * (1 - c) + axis.x * s);
	res.z = p.x * (axis.x * axis.z * (1 - c) + axis.y * s) + p.y * (axis.y * axis.z* (1 - c) - axis.x * s) + p.z * (c + axis.z * axis.z * (1 - c));
	return (res);
}

vec2	intersect_ray_sphere(vec3 ray_start, vec3 ray_dir, s_input sphere)
{
	vec3	c;
	float	r;
	
	vec3	oc;
	float	k1;
	float	k2;
	float	k3;
	float 	discriminant;
	vec2	res;

	c = sphere.transform.position.xyz;
	r = sphere.f_radius;
	oc = ray_start - c;
	k1 = dot(ray_dir, ray_dir);
	k2 = dot(oc, ray_dir) * 2;
	k3 = dot(oc, oc) - r * r;
	discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0)
	{
		res.x = FLT_MAX;
		res.y = FLT_MAX;
	}
	else
	{
		res.x = (-k2 + sqrt(discriminant)) / (2.0 * k1);
		res.y = (-k2 - sqrt(discriminant)) / (2.0 * k1);
	}
	return (res);
}

vec2	intersect_ray_plane(vec3 ray_start, vec3 ray_dir, s_input plane)
{
	float		a;
	float		b;
	vec3		col;

	a = dot(ray_dir, plane.transform.rotation.xyz);
	if (abs(a) < 0.0003f)
		return (vec2(FLT_MAX, FLT_MAX));
	b = -dot(ray_start - plane.transform.position.xyz, plane.transform.rotation.xyz) / a;
	col = ray_start + ray_dir * b;
	if (b < 0.0003f)
		return (vec2(FLT_MAX, FLT_MAX));
	if (dot(plane.transform.rotation.xyz, cross(plane.f_vertices[1].xyz - plane.f_vertices[0].xyz, col - plane.f_vertices[0].xyz)) > 0 &&
		dot(plane.transform.rotation.xyz, cross(plane.f_vertices[2].xyz - plane.f_vertices[1].xyz, col - plane.f_vertices[1].xyz)) > 0 &&
		dot(plane.transform.rotation.xyz, cross(plane.f_vertices[3].xyz - plane.f_vertices[2].xyz, col - plane.f_vertices[2].xyz)) > 0 &&
		dot(plane.transform.rotation.xyz, cross(plane.f_vertices[0].xyz - plane.f_vertices[3].xyz, col - plane.f_vertices[3].xyz)) > 0)
		return (vec2(b, b));
	return (vec2(FLT_MAX, FLT_MAX));
}

vec2	intersect_ray_cone(vec3 ray_start, vec3 ray_dir, s_input cone)
{
	vec3	c;
	vec3	ov;
	vec2	res;
	float	cos;
	float	k1;
	float	k2;
	float	k3;
	float 	discriminant;

	cos = cone.f_height / sqrt(cone.f_height * cone.f_height + cone.f_radius * cone.f_radius);
	ov = cone.f_ver.xyz - ray_start;
	float ovcd = dot(ov, cone.direction.xyz);
	float dscd = dot(ray_dir, cone.direction.xyz);
	k1 = cos * cos * dot(ray_dir, ray_dir) - dscd * dscd;
	k2 = 2 * (ovcd * dscd - cos * cos * dot(ray_dir, ov));
	k3 = dot(ov, ov) * cos * cos - ovcd * ovcd;
	discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0)
	{
		res.x = FLT_MAX;
		res.y = FLT_MAX;
	}
	else
	{
		res.x = (-k2 + sqrt(discriminant)) / (2 * k1);
		res.y = (-k2 - sqrt(discriminant)) / (2 * k1);
	}
	/*
		Top part clipping
	*/
	if ((dot(ray_start, cone.direction.xyz) + res.x * dscd - dot(cone.f_ver.xyz, cone.direction.xyz)) > 0)
		res.x = FLT_MAX;
	if ((dot(ray_start, cone.direction.xyz) + res.y * dscd - dot(cone.f_ver.xyz, cone.direction.xyz)) > 0)
		res.y = FLT_MAX;	
	/*
		Bottom part clipping
	*/
	float cone_side_len = sqrt(cone.f_height * cone.f_height + cone.f_radius * cone.f_radius);
	if (length(ray_start + ray_dir * res.x - cone.f_ver.xyz) > cone_side_len)
		res.x = FLT_MAX;
	if (length(ray_start + ray_dir * res.y - cone.f_ver.xyz) > cone_side_len)
		res.y = FLT_MAX;
	/*
		Bottom plane intersection check
	*/
	float a = dot(ray_dir, -cone.direction.xyz);
	if (abs(a) < 0.0003f)
		return (res);
	float b = -dot(ray_start - cone.transform.position.xyz, -cone.direction.xyz) / a;
	if (b < 0.0003f || length(ray_start + b * ray_dir - cone.transform.position.xyz) > cone.f_radius)
		return (res);
	if (res.x == FLT_MAX)
		res.x = b;
	if (res.y == FLT_MAX)
		res.y = b;
	return (res);
}

vec2	intersect_ray_cylinder(vec3 ray_start, vec3 ray_dir, s_input cyl)
{
	vec3	c;
	vec3	oc;
	vec3	v;
	float	r;
	float	k1;
	float	k2;
	float	k3;
	float 	discriminant;
	vec2	res;

	c = cyl.transform.position.xyz;
	r = cyl.f_radius;
	v = cyl.transform.rotation.xyz;
	oc = ray_start - c;
	k1 = dot(cross(ray_dir, v), cross(ray_dir, v));
	k2 = 2 * dot(cross(ray_dir, v), cross(oc, v));
	k3 = dot(cross(oc, v), cross(oc, v)) - r * r;
	discriminant = k2 * k2 - 4 * k1 * k3;
	if (discriminant < 0)
	{
		res.x = FLT_MAX;
		res.y = FLT_MAX;
	}
	else
	{
		res.x = (-k2 + sqrt(discriminant)) / (2 * k1);
		res.y = (-k2 - sqrt(discriminant)) / (2 * k1);
	}
	return (res);
}

s_input	closest_intersection(vec3 ray_dir, vec3 ray_start)
{
	s_input		obj;
	int			i;
	vec2		t;
	
	r.t_c.closest_t = FLT_MAX;
	obj.type = obj_null;
	i = 0;
	while (i < n_fig)
	{			
		if (sbo_input[i].type == obj_sphere)
			t = intersect_ray_sphere(ray_start, ray_dir, sbo_input[i]);
		else if (sbo_input[i].type == obj_plane)
			t = intersect_ray_plane(ray_start, ray_dir, sbo_input[i]);
		else if (sbo_input[i].type == obj_cylinder)
			t = intersect_ray_cylinder(ray_start, ray_dir, sbo_input[i]);
		else if (sbo_input[i].type == obj_cone)
			t = intersect_ray_cone(ray_start, ray_dir, sbo_input[i]);
		if ((r.t_c.t_min < t.x && r.t_c.t_max > t.x) && t.x < r.t_c.closest_t)
		{
			r.t_c.closest_t = t.x;
			obj = sbo_input[i];
		}
		if ((r.t_c.t_min < t.y && t.y < r.t_c.t_max) && t.y < r.t_c.closest_t)
		{
			r.t_c.closest_t = t.y;
			obj = sbo_input[i];
		}
		i++;
	}
	return (obj);
}

float	compute_lighting(vec3 p, vec3 n, int s)
{
	float		res;
	uint		i;
	vec3		l;
	float		n_scal_l;
	float		shadow_t;
	s_input		shadow_obj;
	float		t_max;

	res = 0.0;
	i = n_fig;
	while (i < n_fig + n_lig)
	{
		if (sbo_input[i].type != light_ambient && sbo_input[i].type != light_point && sbo_input[i].type != light_directional)
		{
			i++;
			continue;
		}
		if (sbo_input[i].type == light_ambient)
			res += sbo_input[i].l_intensity;
		else 
		{
			if (sbo_input[i].type == light_point)
			{
				l = sbo_input[i].transform.position.xyz - p;
				t_max = 1.0;
			}
			else
			{
				l = sbo_input[i].direction.xyz;
				t_max = FLT_MAX;
			}
			
			//Проверка тени
			r.t_c.t_min = 0.001;
			r.t_c.t_max = t_max;
			shadow_obj = closest_intersection(l, p);
			if (shadow_obj.type != obj_null)
			{
				i++;
				continue;
			}
			
			//Диффузность
			n_scal_l = dot(n, l);
			if (n_scal_l > 0)
				res += sbo_input[i].l_intensity * n_scal_l / (length(n) * length(l));

			//Зеркальность
			if (s != -1)
			{
				vec3 r_v = n * dot(n, l) * 2 - l;
				float rv_scal_v = dot(r_v, -r.ds);
				if (rv_scal_v > 0)
					res += sbo_input[i].l_intensity * pow(rv_scal_v / (length(r_v) * length(-r.ds)), s);
			}
		}
		i++;
	}
	if (res >= 1.0)
		res = 1.0;
	return (res);
}

vec4	trace_ray()
{
	int			i;
	vec3		norm;
	vec3		p;
	s_input		obj;
	float		cL;

	obj = closest_intersection(r.ds, camera.position.xyz);
	if (obj.type == obj_null)
		return (vec4(1.0, 1.0, 1.0, 1.0));
	p = camera.position.xyz + r.ds * r.t_c.closest_t;
	if (obj.type == obj_sphere)
	{
		norm = p - obj.transform.position.xyz;
		norm = norm * (1.0 / length(norm));	
	}
	else if (obj.type == obj_plane)
	{
		if (dot(r.ds, obj.transform.rotation.xyz) < 0)
			norm = obj.transform.rotation.xyz;
		else
			norm = -obj.transform.rotation.xyz;
	}
	else if (obj.type == obj_cylinder)
	{
		vec3 tmp = obj.transform.position.xyz - p;
		norm = (obj.transform.rotation.xyz * dot(tmp, obj.transform.rotation.xyz) - tmp) / length(obj.transform.rotation.xyz);
	}
	else if (obj.type == obj_cone)
	{
		if (dot(-obj.direction.xyz, obj.transform.position.xyz - p) < 0.003f)
			norm = -obj.direction.xyz;
		else
		{
			vec3	pc = p - obj.transform.position.xyz;
			vec3	pv = p - obj.f_ver.xyz;
			float	cos = obj.f_height / sqrt(obj.f_height * obj.f_height + obj.f_radius * obj.f_radius);
			norm = vec3(obj.transform.position.xyz + obj.direction.xyz * (obj.f_height - length(pv) / cos));
			norm = p - norm;
		}
		norm = norm / length(norm);
	}
	cL = compute_lighting(p, norm, obj.f_specular);
	return (obj.color * cL);
}

vec3	rotate_x(vec3 t, float angle)
{
	float		prev_y;
	float		prev_z;

	prev_y = t.y;
	prev_z = t.z;
	t.y = (prev_y * cos(angle) - prev_z * sin(angle));
	t.z = (prev_y * sin(angle) + prev_z * cos(angle));
	return (t);
}

vec3	rotate_y(vec3 t, float angle)
{
	float		prev_x;
	float		prev_z;

	prev_x = t.x;
	prev_z = t.z;
	t.x = (prev_x * cos(angle) + prev_z * sin(angle));
	t.z = (prev_z * cos(angle) - prev_x * sin(angle));
	return (t);
}

vec3	rotate_z(vec3 t, float angle)
{
	float		prev_x;
	float		prev_y;

	prev_x = t.x;
	prev_y = t.y;
	t.x = (prev_x * cos(angle) - prev_y * sin(angle));
	t.y = (prev_x * sin(angle) + prev_y * cos(angle));
	return (t);
}

void main()
{
	r.ds = canvas_to_viewport(int(win_height/2 - gl_FragCoord.y), int(gl_FragCoord.x - win_width/2));
	r.ds = rotate_x(r.ds, camera.rotation.x);
	r.ds = rotate_y(r.ds, camera.rotation.y);
	r.ds = rotate_z(r.ds, camera.rotation.z);
	r.t_c.t_min = 1.0;
	r.t_c.t_max = FLT_MAX;
	outColor = trace_ray();
}
