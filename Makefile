NAME = rtv1

SRCDIR					= srcs/
INCDIR					= includes/
OBJDIR					= obj/
LIBSDIR					= libs/
WIN_VULKAN_INCLUDE_PATH = C:\VulkanSDK\1.2.131.2\Include
WIN_VULKAN_LIB_PATH		= C:\VulkanSDK\1.2.131.2\Lib


SRCS =	app_core.c \
		main.c \
		scene_reader.c \
		shader_reader.c \
		utils.c \
		vku_buffers.c \
		vku_commandbuffers.c \
		vku_descriptors.c \
		vku_devices.c \
		vku_drawframe.c \
		vku_framebuffer.c \
		vku_instance.c \
		vku_pipeline.c \
		vku_render.c \
		vku_renderpass.c \
		vku_shadermodule.c \
		vku_swapchain.c \
		vku_window.c \

OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror

OPENCLLNK = -framework OpenCL

VULKANLINK = -l vulkan
VULKANINC_WIN = -I C:\VulkanSDK\1.2.131.2\Include
VULKANLINK_WIN = -L C:\VulkanSDK\1.2.131.2\Lib -l vulkan-1

SDL2LINK = -l SDL2
SDL2INC_win = -I SDL2_win/include/SDL2
SDL2LINK_win = -L SDL2_win/bin -l SDL2

VECLIB =  $(addprefix $(LIBSDIR), vec_lib/lib)
VECINC =  $(addprefix $(LIBSDIR), vec_lib/includes)
VECLIBLINK = -L $(VECLIB) -l vec_lib.a
VECLIBINC = -I $(VECINC)

FTLIBDIR =  $(addprefix $(LIBSDIR), printf)
FTINCDIR =  $(addprefix $(LIBSDIR), printf/includes)
FTLINK = -L $(FTLIBDIR) -l libftprintf.a
FTINC = -I $(FTINCDIR)

all: $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o:$(SRCDIR)%.c
	$(CC) $(CFLAGS) $(FTINC) $(VECLIBINC) -I $(INCDIR) $(VULKANINC_WIN) $(SDL2INC_win) -o $@ -c $<

$(NAME): libs obj $(OBJS)
	$(CC) $(OBJS) libs/vec_lib/lib/vec_lib.a libs/printf/libftprintf.a $(SDL2LINK) $(VULKANLINK) -lm -o $(NAME)

win: libs obj $(OBJS)
	$(CC) $(OBJS) libs/vec_lib/lib/vec_lib.a libs/printf/libftprintf.a $(SDL2LINK_win) $(VULKANLINK_WIN) -lm -o $(NAME)

winre: fclean win

libs:
	@make -C libs/printf
	@make -C libs/vec_lib

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

test:
	gcc -I C:\VulkanSDK\1.2.131.2\Include -I SDL2_win1/include/SDL2 -l SDL2 -l vulkan -lm -c test.c
	gcc -o q test.o -l SDL2 -l vulkan -lm

shaders:
	glslc -o shaders/rtv1-frag.spv shaders/rtv1.frag
	glslc -o shaders/rtv1-vert.spv shaders/rtv1.vert

.PHONY: clean fclean all re windows libs
