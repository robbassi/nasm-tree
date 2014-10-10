#define DT_DIR		4
void puts(char*, int);
void* malloc(unsigned long len);

struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent  */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */  
  /* char           d_type; */
};

int pos, level = 0;
char buff[1000];

/* void dir2tree(char *path) { */
/*   int fd = fopen(path); */
/*   struct linux_dirent *entries = load_contents(fd); */
/*   display_contents(entries); */
/* } */

int len (char *s) {
  int l = 0;
  for (;*s != '\0'; s++, l++);
  return l;
}

char* concat(char*a, char*b, char*buf) {
  int lena = len(a);
  int lenb = len(b);
  int i, j;
  for (i = 0; i < lena; i++) {
    buf[i] = a[i];
  } 
  for (j = 0; j < lenb; j++, i++) {
    buf[i] = b[j];
  }
  buf[i] = '\0';
  return buf;
}

int is_directory(struct linux_dirent *entry) {
  return *(((char*) entry) + (entry->d_reclen - 1)) == DT_DIR;
}

void println(char *str) {
  puts(str, len(str));
  puts("\n", 1);
}

void print_strs(int l, char**strs) {
  int i;
  for (i = 0; i < l; i++)
    println(strs[i]);
}

void display_contents(struct linux_dirent entries[]) {
  while (entries->d_ino > 0) {
    char *buf = (char*) malloc(100);
    if (is_directory(entries)) {
      level++;
      puts("(directory) ", 11);
      level--;
    }
    println(concat(entries->d_name, " ahhhhh!", buf));
    entries = (struct linux_dirent *) (((char *) entries) + entries->d_reclen);
  }
}
