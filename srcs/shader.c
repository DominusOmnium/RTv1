/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dkathlee <dkathlee@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/16 12:45:59 by dkathlee          #+#    #+#             */
/*   Updated: 2020/08/05 16:40:27 by dkathlee         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "rtv1.h"
//#include <shaderc/shaderc.h>

VkShaderModule	vku_createShaderModule(t_vulkan *v, char *code, uint32_t codeL)
{
	VkShaderModuleCreateInfo	smCreateInfo;
	VkShaderModule				shaderModule;
	
	smCreateInfo = (VkShaderModuleCreateInfo) {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = codeL,
		.pCode = (uint32_t*)code
	};
	ft_printf("Shader file length: %d\n", codeL);
	VkResult res;
	if ((res = vkCreateShaderModule(v->device, &smCreateInfo, NULL, &shaderModule)) != VK_SUCCESS)
	{
		ft_printf("%d\n", res);
		handle_error("Shader module creation error!");
	}
	ft_printf("shader module created\n");
	return (shaderModule);
}

/*
void	shader_compile(char *code)
{
	shaderc_compiler_t compiler = shaderc_compiler_initialize();
	shaderc_compilation_result_t result = shaderc_compile_into_spv(compiler, code, ft_strlen(code), shaderc_glsl_vertex_shader,
          "shaders/live.vert", "main", NULL);
	shaderc_compile_into_spv_assembly(compiler, code, ft_strlen(code), shaderc_glsl_vertex_shader, "shaders/live.vert", "main", NULL);
	shaderc_compilation_status status = shaderc_result_get_compilation_status(result);
	if (status != shaderc_compilation_status_success)
		handle_error("Shader compilation error!");
	shaderc_result_release(result);
	shaderc_compiler_release(compiler);
}
*/
