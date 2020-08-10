/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rtv1.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/06 10:24:56 by celva             #+#    #+#             */
/*   Updated: 2020/08/10 19:05:39 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RTV1_H
# define RTV1_H
# ifdef __APPLE__
# define VK_USE_PLATFORM_MACOS_MVK
# elif _WIN32
# define VK_USE_PLATFORM_WIN32_KHR
# define SDL_MAIN_HANDLED
# else
# define VK_USE_PLATFORM_XCB_KHR
# endif
# include <SDL.h>
# include <SDL_vulkan.h>
# include "ft_printf.h"
# include "vku.h"

# include <stdio.h>
# include <unistd.h>
# include <math.h>
# include <float.h>
# define WIN_WIDTH	1500
# define WIN_HEIGHT	1000
# ifdef _WIN32
//# include <float.h>
typedef int32_t u_int32_t;
# endif

/*
	Objects utils
*/
typedef struct					s_transform
{
	t_vec3						rotation;
	t_vec3						position;
}								t_transform;
typedef enum
{
	obj_null, obj_sphere, obj_plane, obj_cone, obj_cylinder,
	light_ambient, light_point, light_directional
}	t_obj_type;

/*
	Objects
*/

typedef	struct					s_figure
{
	float 				radius;
	t_vec3				direction;
	float				height;
	t_vec3				ver;
	t_vec3				vertices[4];

}								t_figure;
typedef struct					s_object
{
	t_obj_type			type;
	t_transform			transform;
	t_vec3				color;
	int					specular;
	t_figure			figures;
}								t_object;
/*
	Light
*/

typedef struct					s_light
{
	t_obj_type					type;
	double						intensity;
	t_vec3						position;
	t_vec3						direction;
}								t_light;
/*
	Utils
*/
typedef struct					s_interRS
{
	double						t1;
	double						t2;
	t_vec3						center;
	double						radius;
}								t_interRS;
typedef struct					s_closest
{
	double						t_min;
	double						t_max;
	double						closest_t;
}								t_closest;
typedef struct					s_retr
{
	t_vec3						ds;
	double						vw;
	double						vh;
	double						d;
	double						t_min;
	double						t_max;
	t_object					*figures;
	t_light						*lights;
	int							n_fig;
	int							n_lig;
	t_closest					t_c;
	t_transform					camera;
}								t_retr;

struct							s_input
{
	u_int32_t					type;
	s_transform					transform;
	vec3						color;
	float						l_intensity;
	int							f_specular;
	float 						f_radius;
	vec3						f_direction;
	float						f_height;
	vec3						f_ver;
	vec3						f_vertices[4];
}								t_input;

typedef struct					s_app
{
	t_vulkan					vulkan;
	SDL_Window					*window;
	char						*appname;
	t_retr						r;
}								t_app;

VkBuffer uploadBuffer;
VkDeviceMemory uploadBufferMemory;
void* uploadBufferPtr;
VkBuffer staticBuffer;
VkDeviceMemory staticBufferMemory;


int		rtv_app_create(t_app *app);
void	rtv_app_run(t_app *app, char *fname);
int		vku_instance_create(t_app *app);
int		vku_get_physical_device(t_vulkan *v);
int		vku_create_logical_device(t_vulkan *v);
int		vku_window_create(t_app *app);
int		vku_init_render(t_app *app);
int		vku_record_cmb(t_vulkan *v);
int		vku_draw_frame(t_vulkan *v);
int		vku_swapchain_create(t_vulkan *v);
//int		vku_create_buffer(t_vulkan *v);
void	handle_error(char *msg);
void    raytracing(t_retr *r, t_app *app);
int		read_scene(char *fname, t_retr *r);
size_t	load_shader_file(char *fname, char **shader);
VkShaderModule	vku_createShaderModule(t_vulkan *v, char *code, uint32_t codeL);
int init_render(t_vulkan *v, t_retr *r);
void draw_frame(t_vulkan *v, t_retr *r);
void draw_frame1(t_vulkan *v, t_retr *r);
#endif
