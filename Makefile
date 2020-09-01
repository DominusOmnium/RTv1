NAME = rtv1

SRCDIR					= srcs/
INCDIR					= includes/
OBJDIR					= obj/
LIBSDIR					= libs/
VULKAN_INCLUDE_PATH 	= libs/vulkan/includes/
VULKAN_LIB_PATH			= libs/vulkan/lib/

SHADER_COMPILER = libs/vulkan/glslangValidator
GLSLS = $(wildcard shaders/*.vert shaders/*.frag)
SPIRVS = $(addsuffix .spv,$(GLSLS))

SRCS =	app_core.c \
		input.c \
		main.c \
		p3d_rotate.c \
		scene_reader.c \
		shader_reader.c \
		utils.c \
		vku_buffers.c \
		vku_commandbuffers.c \
		vku_descriptors.c \
		vku_drawframe.c \
		vku_framebuffer.c \
		vku_instance.c \
		vku_logical_device.c \
		vku_physical_device.c \
		vku_pipeline.c \
		vku_pipeline_states_creation.c \
		vku_render.c \
		vku_renderpass.c \
		vku_shadermodule.c \
		vku_swapchain.c \
		vku_window.c \

OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror

OPENCLLNK = -framework OpenCL

VULKANINC = -I $(VULKAN_INCLUDE_PATH)
VULKANLINK = -L $(VULKAN_LIB_PATH) -l vulkan

SDL2LINK = -l SDL2
SDL2INC = -I $(addprefix $(LIBSDIR), SDL2/includes/)
SDL2LINK_win = -L $(addprefix $(LIBSDIR), SDL2/lib/) -l SDL2

VECLIB =  $(addprefix $(LIBSDIR), vec_lib/lib/vec_lib.a)
VECINC =  $(addprefix $(LIBSDIR), vec_lib/includes)
VECLIBINC = -I $(VECINC)

FTLIB =  $(addprefix $(LIBSDIR), printf/libftprintf.a)
FTINCDIR =  $(addprefix $(LIBSDIR), printf/includes)
FTINC = -I $(FTINCDIR)

TEST = -DVULKAN_SDK=libs/vulkan/macOS -DVK_ICD_FILENAMES=$$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json -DVK_LAYER_PATH=$$VULKAN_SDK/share/vulkan/explicit_layers.d -DDYLD_LIBRARY_PATH="$$VULKAN_SDK/lib" 

all: $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o:$(SRCDIR)%.c
	$(CC) $(CFLAGS) $(FTINC) $(VECLIBINC) -I $(INCDIR) $(VULKANINC) $(SDL2INC) -o $@ -c $<

$(NAME): libs obj $(OBJS) shaders
	$(CC) $(TEST) $(OBJS) $(VECLIB) $(FTLIB) -F libs/SDL2/lib -framework SDL2 -L libs/vulkan/macOS/lib -l vulkan.1.2.148 -l vulkan.1 -l vulkan -lm -o $(NAME)

run:
	VULKAN_SDK=libs/vulkan/macOS VK_ICD_FILENAMES=$$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json VK_LAYER_PATH=$$VULKAN_SDK/share/vulkan/explicit_layers.d DYLD_LIBRARY_PATH="$$VULKAN_SDK/lib" ./rtv1 scenes/scene

env:
	bash q

linux: libs obj $(OBJS) shaders
	$(CC) $(OBJS) $(VECLIB) $(FTLIB) $(SDL2LINK) $(VULKANLINK) -lm -o $(NAME)

win: libs obj $(OBJS) shaders
	$(CC) $(OBJS) $(VECLIB) $(FTLIB) $(SDL2LINK_win) $(VULKANLINK) -lm -o $(NAME)
	cp libs/SDL2/lib/SDL2.dll .

winre: fclean win

linuxre: fclean linux

libs:
	@make -C libs/printf
	@make -C libs/vec_lib

libsre:
	@make -C libs/printf re
	@make -C libs/vec_lib re

clean:
	rm -rf $(OBJDIR)
	rm -f $(SPIRVS)

fclean: clean
	rm -rf $(NAME)
	rm -rf SDL2.dll

re: fclean all

shaders: $(SPIRVS)

$(SPIRVS): %.spv: %
	$(SHADER_COMPILER) -V $< -o $@

.PHONY: clean fclean all re windows libs
