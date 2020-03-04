NAME = rtv1

SRCDIR = srcs/
INCDIR = includes/
OBJDIR = obj/

SRCS = main.c

OBJS = $(addprefix $(OBJDIR), $(SRCS:.c=.o))

CC = gcc
CFLAGS = -g

OPENCLLNK = -framework OpenCL
VULKANLINK = -l vulkan
SDL2LINK = -L lib -l SDL2-2.0.0

all: obj $(NAME)

obj:
	mkdir -p $(OBJDIR)

$(OBJDIR)%.o:$(SRCDIR)%.c
	$(CC) $(CFLAGS) $(FTINC) -I $(INCDIR) -o $@ -c $<

$(NAME): $(OBJS)
	$(CC) $(OBJS) $(OPENCLLNK) $(SDL2LINK) $(VULKANLINK) -o $(NAME)

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all
