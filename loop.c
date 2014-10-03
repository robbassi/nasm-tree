// linked from tree.asm
void puts(char*, int);

struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */  
};

void print_strs(int l, char**strs) {
  int i;
  for (i = 0; i < l; i++) {
    puts(strs[i] , len(strs[i]));
    puts("\n", 1);
  }
}

int len (char *s) {
  int l = 0;
  for (;*s != '\0'; s++, l++);
  return l;
}
