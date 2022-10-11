CC=gcc
OBJS=shell.o 

shell:$(OBJS)
	$(CC) $^ -o shell

%.o:%.c 
	$(CC) -c $^

# 等同于 gcc -c shell.c  生成shell.o
# 等同于 gcc shell.o -o shell 
# 偷懒而已