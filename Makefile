NAME = rtv1

SRCDIR	= srcs/
INCDIR	= includes/
OBJDIR	= obj/
LIBSDIR	= libs/

SRCS =	main.c \
		app_core.c \
		vku_devices.c \
		vku_drawframe.c \
		vku_instance.c \
		vku_render.c \
		vku_swapchain.c \
		vku_window.c \
		vku_buffers.c \
		utils.c \
		raytracing.c

OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

CC = gcc
CFLAGS = -g

OPENCLLNK = -framework OpenCL
VULKANLINK = -l vulkan
SDL2LINK = -l SDL2

VECLIB =  $(addprefix $(LIBSDIR), vec_lib/lib)
VECINC =  $(addprefix $(LIBSDIR), vec_lib/includes)
VECLIBLINK = -L $(VECLIB) -l vec_lib.a
VECLIBINC = -I $(VECINC)

all: obj $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o:$(SRCDIR)%.c
	$(CC) $(CFLAGS) $(FTINC) $(VECLIBINC) -I $(INCDIR) -o $@ -c $<

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(SDL2LINK) $(VULKANLINK) $(VECLIBLINK) -lm -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all
