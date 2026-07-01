NAME = libasm.a

SRCS = ft_strlen.s ft_strcpy.s ft_strcmp.s ft_write.s ft_read.s ft_strdup.s
OBJS = $(SRCS:.s=.o)

NASM = nasm
NASMFLAGS = -f elf64

AR = ar
ARFLAGS = rcs

all: $(NAME)

$(NAME): $(OBJS)
	$(AR) $(ARFLAGS) $(NAME) $(OBJS)

%.o: %.s
	$(NASM) $(NASMFLAGS) $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
