#version 450
#extension GL_ARB_separate_shader_objects : enable

#define FLT_MAX 3.402823466e+38
#define FLT_MIN 1.175494351e-38
#define DBL_MAX 1.7976931348623158e+308
#define DBL_MIN 2.2250738585072014e-308

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform FSConst {
    vec2 resolution;
    vec2 mouse;
    float time;
} u_input;
vec2 resolution = vec2(1500.0, 1000.0);

const uint obj_null		= 0x00000000u;
const uint obj_sphere	= 0x00000001u;
const uint obj_plane	= 0x00000002u;
const uint obj_cone		= 0x00000003u;
const uint obj_cylinder	= 0x00000004u;

const uint light_ambient	= 0x00000005u;
const uint light_point	= 0x00000006u;
const uint light_directional	= 0x00000007u;

struct					s_transform
{
	vec3						rotation;
	vec3						position;
}								t_transform;

struct					s_figure
{
	float 				radius;
	vec3				direction;
	float				height;
	vec3				ver;
	vec3				vertices[4];

};
struct					s_object
{
	uint				type;
	s_transform			transform;
	vec3				color;
	int					specular;
	s_figure			figures;
};

struct					s_light
{
	uint				type;
	float				intensity;
	vec3				position;
	vec3				direction;
};

struct					s_closest
{
	float				t_min;
	float				t_max;
	float				closest_t;
};

struct					s_retr
{
	vec3						ds;
	float						vw;
	float						vh;
	float						d;
	float						t_min;
	float						t_max;
	s_object					figures[10];
	s_light						lights[10];
	int							n_fig;
	int							n_lig;
	s_closest					t_c;
	s_transform					camera;
};


s_retr r;


vec3	canvas_to_viewport(int i, int j)
{
    vec3	res;
	
    res.x = j * r.vw / 1500;
    res.y = i * r.vh / 1000;
    res.z = r.d;
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

vec2	intersect_ray_sphere(vec3 ds, vec3 o, s_object sphere)
{
	vec3	c;
	float	r;
	
	vec3	oc;
	float	k1;
	float	k2;
	float	k3;
	float 	discriminant;
	vec2	res;

	c = sphere.transform.position;
	r = sphere.figures.radius;
	//r = ((t_sphere*)(sphere.obj)).radius;
	oc = o - c; //vec3d_sub_vec3d(o, c);
	k1 = dot(ds, ds); //vec3d_scalar(ds, ds);
	k2 = dot(oc, ds) * 2; //vec3d_scalar(oc, ds) * 2;
	k3 = dot(oc, oc) - r * r; //vec3d_scalar(oc, oc) - r * r;
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

vec2	intersect_ray_plane(vec3 o, vec3 dir, s_object plane)
{
	float		a;
	float		b;
	s_figure	p;
	vec3		col;

	p = plane.figures;
	a = dot(dir, p.direction); //vec3d_scalar(dir, p.direction);
	if (abs(a) < 0.0003f)
		return (vec2(FLT_MAX, FLT_MAX));
	b = -dot(o - plane.transform.position, p.direction) / a; //-vec3d_scalar(vec3d_sub_vec3d(o, plane.transform.position), p.direction) / a;
	//b = -vec3d_scalar(vec3d_sub_vec3d(o, plane.transform.position), p.normal) / a;
	col = o + dir * b; //vec3d_add_vec3d(o, vec3d_mul_d(dir, b));
	if (b < 0.0003f)
		return (vec2(FLT_MAX, FLT_MAX));
	if (dot(p.direction, cross(p.vertices[1] - p.vertices[0], col - p.vertices[0])) > 0 &&
		dot(p.direction, cross(p.vertices[2] - p.vertices[1], col - p.vertices[1])) > 0 &&
		dot(p.direction, cross(p.vertices[3] - p.vertices[2], col - p.vertices[2])) > 0 &&
		dot(p.direction, cross(p.vertices[0] - p.vertices[3], col - p.vertices[3])) > 0)
		return (vec2(b, b));
	return (vec2(FLT_MAX, FLT_MAX));
}

s_object	closest_intersection(vec3 ds, vec3 o)
{
	s_object	obj;
	int		i;
	vec2		t;
	
	r.t_c.closest_t = FLT_MAX;
	obj.type = obj_null;
	i = 0;
	while (i < r.n_fig)
	{
		//t.y = FLT_MAX;
		if ((r.figures)[i].type == obj_sphere)
			t = intersect_ray_sphere(ds, o, (r.figures)[i]);
		else if ((r.figures)[i].type == obj_plane)
			t = intersect_ray_plane(o, ds, (r.figures)[i]);
		/*else if ((r.figures)[i].type == obj_cylinder)
			t = intersect_ray_cylinder(ds, o, (r.figures)[i]);
		else if ((r.figures)[i].type == obj_cone)
			t = intersect_ray_cone(ds, o, (r.figures)[i].figures);*/
		if ((r.t_c.t_min < t.x && r.t_c.t_max > t.x) && t.x < r.t_c.closest_t)
		{
			r.t_c.closest_t = t.x;
			obj = (r.figures)[i];
		}
		if ((r.t_c.t_min < t.y && r.t_c.t_max > t.y) && t.y < r.t_c.closest_t)
		{
			r.t_c.closest_t = t.y;
			obj = (r.figures)[i];
		}
		i++;
	}
	return (obj);
}

float	compute_lighting(vec3 p, vec3 n, int s)
{
	float		res;
	int			i;
	vec3		l;
	float		n_scal_l;
	float		shadow_t;
	s_object	shadow_obj;
	float		t_max;

	res = 0.0;
	i = 0;
	while (i < r.n_lig)
	{
		if ((r.lights)[i].type == light_ambient)
			res += (r.lights)[i].intensity;
		else 
		{
			if ((r.lights)[i].type == light_point)
			{
				l = (r.lights)[i].position - p; //vec3d_sub_vec3d((r.lights)[i].position, p);
				t_max = 1.0;
			}
			else
			{
				l = (r.lights)[i].direction;
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
			n_scal_l = dot(n, l); //vec3d_scalar(n, l);
			if (n_scal_l > 0)
				res += (r.lights)[i].intensity * n_scal_l / (length(n) * length(l)); //res += (r.lights)[i].intensity * n_scal_l / (vec3d_mod(n) * vec3d_mod(l));

			//Зеркальность
			if (s != -1)
			{
				vec3 r_v = n * dot(n, l) * 2 - l; //vec3d_sub_vec3d(vec3d_mul_d(n, (vec3d_scalar(n, l) * 2.0)), l);
				float rv_scal_v = dot(r_v, -r.ds); //vec3d_scalar(r_v, v);
				if (rv_scal_v > 0)
					res += (r.lights)[i].intensity * pow(rv_scal_v / (length(r_v) * length(-r.ds)), s);//res += (r.lights)[i].intensity * pow(rv_scal_v / (vec3d_mod(r_v) * vec3d_mod(-r.ds)), s);
			}
		}
		i++;
	}
	if (res >= 1.0)
		res = 1.0;
	return (res);
}

vec3	trace_ray()
{
	int		i;
	vec3		norm;
	vec3		p;
	s_object	obj;
	float		cL;

	obj = closest_intersection(r.ds, r.camera.position);
	if (obj.type == obj_null)
		return (vec3(1.0, 1.0, 1.0));
	p = r.camera.position + r.ds * r.t_c.closest_t; // vec3d_add_vec3d(r.camera.position, vec3d_mul_d(r.ds, r.t_c.closest_t));
	if (obj.type == obj_sphere)
	{
		norm = p - obj.transform.position; //vec3d_sub_vec3d(p, obj.transform.position);
		norm = norm * (1.0 / length(norm)); //vec3d_mul_d(norm, 1.0/vec3d_mod(norm));	
	}
	else if (obj.type == obj_plane)
	{
		if (dot(r.ds, obj.figures.direction) < 0)
			norm = obj.figures.direction;
		else
			norm = -obj.figures.direction; //vec3d_mul_d(obj.figures.direction, -1);
	}
	else if (obj.type == obj_cylinder)
	{
		vec3 tmp = obj.transform.position - p;
		norm = (obj.figures.direction * dot(tmp, obj.figures.direction) - tmp) / length(obj.figures.direction);
		/*vec3d_mul_d(vec3d_sub_vec3d(vec3d_mul_d(obj.figures.direction,
			vec3d_scalar(vec3d_sub_vec3d(obj.transform.position, p), obj.figures.direction)),
			vec3d_sub_vec3d(obj.transform.position, p)), 1 / vec3d_mod(obj.figures.direction));*/
	}
	else if (obj.type == obj_cone)
	{
		vec3	pc = p - obj.transform.position; //vec3d_sub_vec3d(p, obj.transform.position);
		vec3	pv = p - obj.figures.ver; //vec3d_sub_vec3d(p, obj.figures.ver);
		float	cos = obj.figures.height / sqrt(obj.figures.height * obj.figures.height + obj.figures.radius * obj.figures.radius);
		norm = obj.transform.position + obj.figures.direction * (obj.figures.height - length(pv) / cos); //vec3d_add_vec3d(obj.transform.position, vec3d_mul_d(obj.figures.direction, obj.figures.height - vec3d_mod(pv) / cos));
		norm = p - norm; //vec3d_sub_vec3d(p, norm);
		norm = norm / length(norm); //vec3d_mul_d(norm, 1 / vec3d_mod(norm));
	}
	cL = compute_lighting(p, norm, obj.specular);
	return (obj.color * cL); //(vec3d_mul_d(obj.color, cL));
}

void main()
{

    r.vw = 1.5;
    r.vh = 1.0;
    r.d = 1.0;
	r.camera.position = vec3(0.0, 0.0, 0.0);
	r.camera.rotation = vec3(1.0, 0.0, 0.0);
	r.n_fig = 4;
	r.n_lig = 3;

	r.lights[0].type = light_ambient;
	r.lights[0].intensity = 0.2;
	r.lights[1].type = light_directional;
	r.lights[1].intensity = 0.2;
	r.lights[1].direction = vec3(1, 4, 4);
	r.lights[2].type = light_point;
	r.lights[2].intensity = 0.6;
	r.lights[2].position = vec3(0, 2, 1);

	r.figures[0].figures.radius = 1.0;
	r.figures[0].transform.position = vec3(0.0, 0.0, 3.0);
	r.figures[0].color = vec3(1.0, 0.0, 0.0);
	r.figures[0].type = obj_sphere;
	r.figures[0].specular = 500;

	r.figures[1].figures.radius = 1;
	r.figures[1].transform.position = vec3(2.0, 0.0, 4.0);
	r.figures[1].color = vec3(0.0, 0.0, 1.0);
	r.figures[1].type = obj_sphere;
	r.figures[1].specular = 500;

	r.figures[2].figures.radius = 1.0;
	r.figures[2].transform.position = vec3(-2, 0.0, 4.0);
	r.figures[2].color = vec3(0.0, 1.0, 0.0824);
	r.figures[2].type = obj_sphere;
	r.figures[2].specular = 10;

	r.figures[3].type = obj_plane;
	r.figures[3].color = vec3(0, 0.5, 0);
	r.figures[3].specular = 100;
	r.figures[3].transform.position = vec3(0, -1, 0);
	r.figures[3].figures.direction = vec3(0, 1, 0);
	r.figures[3].figures.vertices[0] = vec3(-2, -1, 4);
	r.figures[3].figures.vertices[1] = vec3(2, -1, 4);
	r.figures[3].figures.vertices[2] = vec3(2, -1, 2);
	r.figures[3].figures.vertices[3] = vec3(-2, -1, 2);
	r.ds = canvas_to_viewport(int(1000/2 - gl_FragCoord.y), int(gl_FragCoord.x - 1500/2));
	r.ds = rotation_axis(r.camera.rotation.z, vec3(0, 1, 0), r.ds);
	r.t_c.t_min = 1.0;
	r.t_c.t_max = FLT_MAX;
	outColor = vec4(trace_ray(), 1.0);
	//outColor = col;
}
