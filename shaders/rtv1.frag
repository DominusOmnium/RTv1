#version 450
#extension GL_ARB_separate_shader_objects : enable

#define FLT_MAX 3.402823466e+38
#define FLT_MIN 1.175494351e-38
#define DBL_MAX 1.7976931348623158e+308
#define DBL_MIN 2.2250738585072014e-308
#define PI 3.1415926535897932384626433832795

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
};

struct							s_texture
{
	uint						offset_in_buffer;
	uint						width;
	uint						height;
	uint						index;
};

struct							s_input
{
	vec4						position;
	vec4						direction;
	vec4						f_ver;
	vec4						basis[3];
	vec4						color;
	s_texture					texture;
	s_texture					normal_map;
	uint						type;
	float						l_intensity;
	float 						f_radius;
	float						f_height;
	float						f_metalness;
	float						f_transparency;
	float						f_reflection;
	float						f_refraction;
	float						f_emission;
	float						fill_to_aligment[3];
};

struct							s_intersection_info
{
	s_input						object;
	float						d;
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
	float		fill_to_aligment[1];
	s_input		sbo_input[];
};

layout(std430, binding = 1) readonly buffer Textures
{
	uint		textures[];
};

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

vec2	intersect_ray_sphere(vec4 ray_start, vec4 ray_dir, s_input sphere)
{
	vec4	c;
	float	r;
	
	vec4	oc;
	float	k1;
	float	k2;
	float	k3;
	float 	discriminant;
	vec2	res;

	c = sphere.position;
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

vec2	intersect_ray_plane(vec4 ray_start, vec4 ray_dir, s_input plane)
{
	float		a;
	float		b;
	vec4		col;

	a = dot(ray_dir, plane.direction);
	if (abs(a) < 0.0003f)
		return (vec2(FLT_MAX, FLT_MAX));
	b = -dot(ray_start - plane.position, plane.direction) / a;
	col = ray_start + ray_dir * b;
	if (b < 0.0003f)
		return (vec2(FLT_MAX, FLT_MAX));
	return (vec2(b, b));
}

vec2	intersect_ray_cone(vec4 ray_start, vec4 ray_dir, s_input cone)
{
	vec4	c;
	vec4	ov;
	vec2	res;
	float	cos;
	float	k1;
	float	k2;
	float	k3;
	float 	discriminant;

	cos = cone.f_height / sqrt(cone.f_height * cone.f_height + cone.f_radius * cone.f_radius);
	ov = cone.f_ver - ray_start;
	float ovcd = dot(ov, cone.direction);
	float dscd = dot(ray_dir, cone.direction);
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
	if ((dot(ray_start, cone.direction) + res.x * dscd - dot(cone.f_ver, cone.direction)) > 0)
		res.x = FLT_MAX;
	if ((dot(ray_start, cone.direction) + res.y * dscd - dot(cone.f_ver, cone.direction)) > 0)
		res.y = FLT_MAX;	
	/*
		Bottom part clipping
	*/
	float cone_side_len = sqrt(cone.f_height * cone.f_height + cone.f_radius * cone.f_radius);
	if (length(ray_start + ray_dir * res.x - cone.f_ver) > cone_side_len)
		res.x = FLT_MAX;
	if (length(ray_start + ray_dir * res.y - cone.f_ver) > cone_side_len)
		res.y = FLT_MAX;
	/*
		Bottom plane intersection check
	*/
	float a = dot(ray_dir, -cone.direction);
	if (abs(a) < 0.0003f)
		return (res);
	float b = -dot(ray_start - cone.position, -cone.direction) / a;
	if (b < 0.0003f || length(ray_start + b * ray_dir - cone.position) > cone.f_radius)
		return (res);
	if (res.x == FLT_MAX)
		res.x = b;
	if (res.y == FLT_MAX)
		res.y = b;
	return (res);
}

vec2	intersect_ray_cylinder(vec4 ray_start, vec4 ray_dir, s_input cyl)
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

	c = cyl.position.xyz;
	r = cyl.f_radius;
	v = cyl.direction.xyz;
	oc = ray_start.xyz - c;
	k1 = dot(cross(ray_dir.xyz, v), cross(ray_dir.xyz, v));
	k2 = 2 * dot(cross(ray_dir.xyz, v), cross(oc, v));
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

s_intersection_info	closest_intersection(vec4 ray_start, vec4 ray_dir)
{
	s_intersection_info int_info;
	int					i;
	vec2				t;
	
	int_info.d = FLT_MAX;
	int_info.object.type = obj_null;
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
		if (t.x > 0.00003 && t.x < int_info.d)
		{
			int_info.d = t.x;
			int_info.object = sbo_input[i];
		}
		if (t.y > 0.00003 && t.y < int_info.d)
		{
			int_info.d = t.y;
			int_info.object = sbo_input[i];
		}
		i++;
	}
	return (int_info);
}

float	compute_lighting(vec4 hitpoint, vec4 normal, int specular, vec4 ray_dir)
{
	float				res;
	uint				i;
	vec4				dir_to_light;
	float				n_scal_l;
	float				shadow_t;
	s_intersection_info	int_info;
	float				t_max;

	res = 0.0;
	i = 0;
	while (i < n_fig + n_lig + 2)
	{
		if (sbo_input[i].type < 5)
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
				dir_to_light = sbo_input[i].position - hitpoint;
				t_max = 1.0;
			}
			else
			{
				dir_to_light = -sbo_input[i].direction;
				t_max = FLT_MAX;
			}
			
			//Проверка тени
			int_info = closest_intersection(hitpoint, dir_to_light);
			if (int_info.object.type != obj_null)
			{
				i++;
				continue;
			}
			
			//Диффузность
			n_scal_l = dot(normal, dir_to_light);
			if (n_scal_l > 0)
				res += sbo_input[i].l_intensity * n_scal_l / (length(normal) * length(dir_to_light));

			//Зеркальность
			if (specular != -1)
			{
				vec4 r_v = normal * n_scal_l * 2 - dir_to_light;
				float rv_scal_v = dot(r_v, -ray_dir);
				if (rv_scal_v > 0)
					res += sbo_input[i].l_intensity * pow(rv_scal_v / (length(r_v) * length(-ray_dir)), specular);
			}
		}
		i++;
	}
	if (res >= 1.0)
		res = 1.0;
	return (res);
}

vec3	get_normal(s_input obj, vec3 point, vec3 direction)
{
	vec3	norm;
	if (obj.type == obj_sphere)
	{
		norm = point - obj.position.xyz;
		norm = norm * (1.0 / length(norm));	
	}
	else if (obj.type == obj_plane)
	{
		if (dot(direction, obj.direction.xyz) < 0)
			norm = obj.direction.xyz;
		else
			norm = -obj.direction.xyz;
		/*vec3 basis0;
		vec3 basis1;
		float           u;
		float           v;

		basis0 = normalize(vec3(obj.rotation.y, -obj.rotation.x, 0));
		basis1 = normalize(cross(obj.rotation.xyz, basis0));

		u = modf(0.5 + dot(basis0, point) / 2, u);
		v = modf(0.5 + dot(basis1, point) / 2, v);
		if (v < 0)
			v += 1;
		if (u < 0)
			u += 1;
		uint n_map_val = textures[obj.normal_map.offset_in_buffer + (int(v * obj.texture.height) * obj.texture.width) + (int(u * obj.texture.width))];
		norm = vec3(float(n_map_val & uint(0x000000ff)) / 255 * 2.f - 1.f, float(n_map_val >> 8 & uint(0x000000ff)) / 255 * 2.f - 1.f, float(n_map_val >> 16 & 0x000000ff) / 255 * 2.f - 1.f);
		norm.xz = norm.zx;
		norm = norm.x * obj.rotation.xyz + basis0 * norm.z + basis1 * norm.y;*/
	}
	else if (obj.type == obj_cylinder)
	{
		vec3 tmp = obj.position.xyz - point;
		norm = (obj.direction.xyz * dot(tmp, obj.direction.xyz) - tmp) / length(obj.direction.xyz);
	}
	else if (obj.type == obj_cone)
	{
		if (dot(-obj.direction.xyz, obj.position.xyz - point) < 0.003f)
			norm = -obj.direction.xyz;
		else
		{
			vec3	pc = point - obj.position.xyz;
			vec3	pv = point - obj.f_ver.xyz;
			float	cos = obj.f_height / sqrt(obj.f_height * obj.f_height + obj.f_radius * obj.f_radius);
			norm = vec3(obj.position.xyz + obj.direction.xyz * (obj.f_height - length(pv) / cos));
			norm = point - norm;
		}
		norm = norm / length(norm);
	}
	return (norm);
}

vec4	get_plane_color(s_input plane, vec3 hitpoint)
{
	vec3 basis0;
	vec3 basis1;
    float           u;
    float           v;

	basis0 = normalize(vec3(plane.direction.y, -plane.direction.x, 0));
	basis1 = normalize(cross(plane.direction.xyz, basis0));

	u = modf(0.5 + dot(basis0, hitpoint) / 2, u);
	v = modf(0.5 + dot(basis1, hitpoint) / 2, v);
	if (v < 0)
		v += 1;
	if (u < 0)
		u += 1;
	uint col = textures[(int(v * plane.texture.height) * plane.texture.width) + (int(u * plane.texture.width))];
	return vec4(float(col & uint(0x000000ff)) / 256, float(col >> 8 & uint(0x000000ff)) / 256, float(col >> 16 & 0x000000ff) / 256, 1);
}

vec4	get_sphere_color(s_input sphere, vec3 hitpoint)
{
	vec3 basis0;
	vec3 basis1;
	vec3 basis2;
    float           u;
    float           v;

	basis0 = vec3(1, 0, 0);
	basis1 = vec3(0, 1, 0);
	basis2 = vec3(0, 0, -1);

	vec3				vect;

	vect = normalize(hitpoint - sphere.position.xyz);
	vect = vec3(dot(basis0, vect), dot(basis1, vect), dot(basis2, vect));
	u = 0.5 + atan(vect.z, vect.x) / PI;
	v = 0.5 - asin(vect.y) / PI;

	//return vec4(u, v, 0, 1);
	uint col = textures[(int(v * sphere.texture.height) * sphere.texture.width) + (int(u * sphere.texture.width))];
	return vec4(float(col & uint(0x000000ff)) / 256, float(col >> 8 & uint(0x000000ff)) / 256, float(col >> 16 & 0x000000ff) / 256, 1);
}

vec4	get_cone_color(s_input cone, vec3 hitpoint)
{
	vec3				vect;
	vec3				a;
    float               v;
	float				u;
	vec3 basis0;
	vec3 basis1;
	vec3 basis2;
	
	basis0 = normalize(vec3(cone.direction.y, -cone.direction.x, 0));
	basis1 = cross(cone.direction.xyz, basis0);
	basis2 = cross(basis0, basis1);

	vect = hitpoint - (cone.position.xyz + cone.direction.xyz * cone.f_height);
	a.y = length(vect) * length(vect) / (dot(cone.direction.xyz, vect) * 2);
	a.x = -dot(vect, basis0);
	a.z = dot(vect, basis1);
	u = 0.5 + atan(a.z, a.x) / PI;
	v = modf(0.5 + a.y * 1 / 2, v);
	if (v < 0)
		v += 1;
	//return vec4(u, v, 0, 1);
	uint col = textures[(int(v * cone.texture.height) * cone.texture.width) + (int(u * cone.texture.width))];
	return vec4(float(col & uint(0x000000ff)) / 256, float(col >> 8 & uint(0x000000ff)) / 256, float(col >> 16 & 0x000000ff) / 256, 1);
}

vec4	get_cylinder_color(s_input cylinder, vec3 hitpoint)
{
	vec3				vect;
	vec3				a;
    float               v;
	float				u;
	vec3 basis0;
	vec3 basis1;
	vec3 basis2;
	
	if (cylinder.direction.x != 0.0f || cylinder.direction.y != 0.0f)
		basis0 = normalize(vec3(cylinder.direction.y, -cylinder.direction.x, 0));
	else
		basis0 = vec3(0.0f, 1.0f, 0.0f);
	basis1 = cross(basis0, cylinder.direction.xyz);
	basis2 = vec3(0.0, 0.0, 1.0);

	vect = hitpoint - cylinder.position.xyz;
	a.y = dot(cylinder.direction.xyz, vect);
	a.x = -dot(vect, basis0);
	a.z = dot(vect, basis1) + 0;
	u = 0.5 + atan(a.z, a.x) / PI;
	v = modf(0.5 + a.y * 1 / 2, v);
	if (v < 0)
		v += 1;

	//return vec4(u, v, 0, 1);
	uint col = textures[(int(v * cylinder.texture.height) * cylinder.texture.width) + (int(u * cylinder.texture.width))];
	return vec4(float(col & uint(0x000000ff)) / 256, float(col >> 8 & uint(0x000000ff)) / 256, float(col >> 16 & 0x000000ff) / 256, 1);
}

/*vec3[2]	get_refract_ray(s_input obj, vec3 hitpoint, vec3 direction)
{
	vec3	res[2];

	if (obj.type == obj_plane)
	{
		res[0] = hitpoint;
		res[1] = direction;
	}
	else if (obj.type == obj_sphere)
	{
		vec2		t;
		t = intersect_ray_sphere()
	}
}*/

vec4	trace_ray(int depth, vec4 ray_start, vec4 ray_dir)
{
	int					i;
	vec4				norm;
	vec4				hitpoint;
	vec4				final_color;
	vec4				local_color;
	s_intersection_info	int_info;
	float				frac;

	i = 0;
	final_color = vec4(0.0);
	frac = 1.0;
	while (i <= depth)
	{
		if (i == 0)
			int_info = closest_intersection(ray_start, ray_dir);
		else
			int_info = closest_intersection(ray_start, ray_dir);
		if (int_info.object.type == obj_null)
		{
				final_color += vec4(0.0471, 0.4353, 0.8, 1.0) * (1.0 - 0.5) * frac;
				break;
		}
		hitpoint = ray_start + ray_dir * int_info.d;
		norm = vec4(get_normal(int_info.object, hitpoint.xyz, ray_dir.xyz), 0);
		/*if (int_info.object.type == obj_plane)
		{
			local_color = get_plane_color(int_info.object, hitpoint.xyz);
		}
		else if (int_info.object.type == obj_sphere)
		{
			local_color = get_sphere_color(int_info.object, hitpoint.xyz);
		}
		else if (int_info.object.type == obj_cone)
		{
			local_color = get_cone_color(int_info.object, hitpoint.xyz);
		}
		else if (int_info.object.type == obj_cylinder)
		{
			local_color = get_cylinder_color(int_info.object, hitpoint.xyz);
		}
		else*/
			local_color = int_info.object.color * compute_lighting(hitpoint, norm, int(int_info.object.f_metalness), ray_dir);
		final_color += local_color * (1.0 - int_info.object.f_reflection) * frac;
		frac *= int_info.object.f_reflection;
		if (int_info.object.f_reflection == 0.0 || frac < 0.05)
			break;
		ray_start = hitpoint;
		ray_dir = 2 * norm * dot(norm, -ray_dir) - -ray_dir;
		i++;
	}
	return (final_color);
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
	vec3 d = canvas_to_viewport(int(win_height/2 - gl_FragCoord.y), int(gl_FragCoord.x - win_width/2));
	d = rotate_x(d, camera.rotation.x);
	d = rotate_y(d, camera.rotation.y);
	d = rotate_z(d, camera.rotation.z);
	outColor = trace_ray(8, camera.position, vec4(d, 0));
	/*if (edge == false)
		return;*/
	/*r.ds = canvas_to_viewport(int(win_height/2 - gl_FragCoord.y), int(gl_FragCoord.x - 1 - win_width/2));
	r.ds = rotate_x(r.ds, camera.rotation.x);
	r.ds = rotate_y(r.ds, camera.rotation.y);
	r.ds = rotate_z(r.ds, camera.rotation.z);
	vec4 tmp = trace_ray(8);
	outColor = mix(outColor, tmp, 0.5);
	r.ds = canvas_to_viewport(int(win_height/2 - gl_FragCoord.y), int(gl_FragCoord.x + 1 - win_width/2));
	r.ds = rotate_x(r.ds, camera.rotation.x);
	r.ds = rotate_y(r.ds, camera.rotation.y);
	r.ds = rotate_z(r.ds, camera.rotation.z);
	tmp = trace_ray(8);
	outColor = mix(outColor, tmp, 0.5);
	r.ds = canvas_to_viewport(int(win_height/2 - gl_FragCoord.y - 1), int(gl_FragCoord.x - win_width/2));
	r.ds = rotate_x(r.ds, camera.rotation.x);
	r.ds = rotate_y(r.ds, camera.rotation.y);
	r.ds = rotate_z(r.ds, camera.rotation.z);
	tmp = trace_ray(8);
	outColor = mix(outColor, tmp, 0.5);
	r.ds = canvas_to_viewport(int(win_height/2 - gl_FragCoord.y + 1), int(gl_FragCoord.x - win_width/2));
	r.ds = rotate_x(r.ds, camera.rotation.x);
	r.ds = rotate_y(r.ds, camera.rotation.y);
	r.ds = rotate_z(r.ds, camera.rotation.z);
	tmp = trace_ray(8);
	outColor = mix(outColor, tmp, 0.5);*/
}
