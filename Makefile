NAME = rtv1

SRCDIR = srcs/
INCDIR = includes/
OBJDIR = obj/

SRCS = main.c app_core.c vku_devices.c vku_drawframe.c vku_instance.c vku_render.c vku_swapchain.c vku_window.c vku_buffers.c utils.c

OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

CC = gcc
CFLAGS = -g

OPENCLLNK = -framework OpenCL
VULKANLINK = -l vulkan
SDL2LINK = -l SDL2

all: obj $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o:$(SRCDIR)%.c
	$(CC) $(CFLAGS) $(FTINC) -I $(INCDIR) -o $@ -c $<

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(SDL2LINK) $(VULKANLINK) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all
