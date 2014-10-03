rebuild : clean all

all : tree.o loop.o
	ld tree.o loop.o -o tree
tree.o :
	nasm -felf64 tree.asm -o tree.o
loop.o :
	gcc -nostdlib -c loop.c
clean :
	rm -rf *.o
