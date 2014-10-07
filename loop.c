#define DT_DIR		4
void puts(char*, int);

struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent  */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */  
  /* char           d_type; */
};

void display_contents(struct linux_dirent entries[]) {
  while (entries->d_ino > 0) {
    if (is_directory(entries))
      puts("(directory) ", 11);
    println(entries->d_name);
    entries = (struct linux_dirent *) (((char *) entries) + entries->d_reclen);
  }
}

int is_directory(struct linux_dirent *entry) {
  return *(((char*) entry) + (entry->d_reclen - 1)) == DT_DIR;
}

void print_strs(int l, char**strs) {
  int i;
  for (i = 0; i < l; i++)
    println(strs[i]);
}

void println(char *str) {
  puts(str, len(str));
  puts("\n", 1);
}

int len (char *s) {
  int l = 0;
  for (;*s != '\0'; s++, l++);
  return l;
}
