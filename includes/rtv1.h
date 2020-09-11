/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:24:56 by celva             #+#    #+#             */
/*   Updated: 2020/09/10 16:29:44 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# ifdef __APPLE__
#  define VK_USE_PLATFORM_MACOS_MVK
#  define O_BINARY 0x0000
# elif _WIN32
#  define VK_USE_PLATFORM_WIN32_KHR
#  define SDL_MAIN_HANDLED
# else
#  define VK_USE_PLATFORM_XCB_KHR
#  define O_BINARY 0x0000
# endif
# include "SDL.h"
# include "SDL_vulkan.h"
# include "ft_printf.h"
# include "vku.h"

# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <float.h>
# define WIN_WIDTH	1500
# define WIN_HEIGHT	1000
# define RAD(Value)		((Value) * 0.0174533)
# define ERROR_MEM_ALLOC "Memory Allocation Error"

typedef struct	s_transform
{
	t_vec4		position;
	t_vec4		rotation;
}				t_transform;
typedef enum
{
	obj_null, obj_sphere, obj_plane, obj_cone, obj_cylinder,
	light_ambient, light_point, light_directional, camera
}	t_obj_type;

typedef struct	s_camera
{
	t_transform	transform;
	t_vec4		direction;
	t_vec4		up;
	float		d;
	float		vh;
	float		vw;
}				t_camera;

typedef struct	s_rt_input
{
	t_transform	camera;
	float		win_width;
	float		win_height;
	float		d;
	float		vh;
	float		vw;
	uint32_t	n_fig;
	uint32_t	n_lig;
	float		fill_to_aligment[1];
}				t_rt_input;

typedef struct	s_object
{
	uint32_t	type;
	int32_t		f_specular;
	float		f_radius;
	float		f_height;
	t_vec4		direction;
	t_vec4		f_ver;
	t_vec4		f_vertices[4];
	t_transform	transform;
	t_vec4		color;
	float		l_intensity;
	float		fill_to_aligment[3];
}				t_object;

typedef struct	s_rt
{
	t_camera	camera;
	float		win_width;
	float		win_height;
	uint32_t	n_fig;
	t_object	*sbo_figures;
	uint32_t	n_lig;
	t_object	*sbo_lights;
}				t_rt;

typedef struct	s_app
{
	t_vulkan	vulkan;
	SDL_Window	*window;
	char		*appname;
	t_rt		r;
}				t_app;

void			rtv_app_create(t_app *app);
void			rtv_app_run(t_app *app);
void			vku_instance_create(t_app *app);
void			vku_get_physical_device(t_vulkan *v);
void			vku_create_logical_device(t_vulkan *v);
void			vku_window_create(t_app *app);
void			vku_init_render(t_vulkan *v);
void			vku_record_cmb(t_vulkan *v);
void			vku_draw_frame(t_vulkan *v);
void			vku_swapchain_create(t_vulkan *v, t_rt *r);
VkShaderModule	vku_create_shader_module(t_vulkan *v, char *code,
												uint32_t code_l);
void			vku_create_pipeline(t_vulkan *v);
void			vku_create_render_pass(t_vulkan *v);
void			vku_create_framebuffers(t_vulkan *v);
void			vku_create_command_buffers(t_vulkan *v);
void			vku_destroy_buffers(t_vulkan *v);
void			read_scene(char *fname, t_rt *r);
size_t			load_shader_file(char *fname, char **shader);
void			draw_frame(t_vulkan *v, t_rt *r);
int				handling_keyboard_input(SDL_Event evt, t_app *app);
void			rotate_x(t_vec4 *t, float angle);
void			rotate_y(t_vec4 *t, float angle);
void			rotate_z(t_vec4 *t, float angle);
void			handle_error(char *msg);
uint32_t		clamp_u32(uint32_t d, uint32_t min, uint32_t max);
t_vec4			string_to_vector(char *str);
float			string_to_float(char **str);
void	parse_sphere(char *str, t_object *sphere);
void	parse_plane(char *str, t_object *plane);
void	parse_cone(char *str, t_object *cone);
void	parse_cylinder(char *str, t_object *cylinder);
void	parse_light(char *str, t_object *light);
#endif
