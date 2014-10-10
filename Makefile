rebuild : clean all

all : base.o tree.o
	ld base.o tree.o -o tree
base.o :
	nasm -felf64 tree.asm -o base.o
tree.o :
	gcc -nostdlib -c tree.c
clean :
	rm -rf *.o
