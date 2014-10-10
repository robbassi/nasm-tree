#define DT_DIR		4

struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent  */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */  
  /* char           d_type; */
};

void puts(char*, int);
int open(char*);
int dirents(char*, struct linux_dirent*buf, int len);
void* malloc(unsigned long);

int base_pos = 0;
int level = 0;
char buff[2000];
char base_path[2000];

int len (char *s) {
  int l = 0;
  for (;*s != '\0'; s++, l++);
  return l;
}

char* concat(char*a, char*b) {
  int lena = len(a);
  int lenb = len(b);
  int i, j;
  for (i = 0; i < lena; i++) {
    buff[i] = a[i];
  } 
  for (j = 0; j < lenb; j++, i++) {
    buff[i] = b[j];
  }
  buff[i] = '\0';
  return buff;
}

int strcmp (char*a, char*b) {
  int lena = len(a);
  int lenb = len(b);

  if (lena != lenb)
    return 0;

  int i;
  for (i = 0; i < lena; i++)
    if (a[i] != b[i])
      return 0;

  return 1;
}

int strcpy (char*s, char*d) {
  int l = len(s);
  int i;
  for (i = 0; i < l; i++) {
    d[i] = s[i];
  }
  d[i] = '\0';
  return i;
}

char* push (char*path) {
  concat(buff, path);
  return concat(buff, "/");
}

void pop () {
  int l = len(buff);
  if (buff[l-1] == '/')
    l-=2;
  for (;l > 0; l--)
    if (buff[l] == '/') {
      buff[l+1] = '\0';
      return;
    }
  
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

void indent() {
  if (!level) return;
  int i;
  puts(" ", 1);
  puts("|", 1);
  for (i = 0; i < level; i++)
    puts("--", 2);
}

void display_contents(int l, struct linux_dirent entries[]) {
  while (entries->d_ino > 0) {
    if (!strcmp(entries->d_name, ".") && !strcmp(entries->d_name, "..")) {
      
      indent();
      println(entries->d_name);

      if (is_directory(entries)) {
	level++;
	tree(entries->d_name);
	level--;
      }
    }
    entries = (struct linux_dirent *) (((char *) entries) + entries->d_reclen);  
  }
}

void tree (char*path) {
  push(path);

  int fd = open(buff);
  struct linux_dirent*buf = (struct linux_dirent*)malloc(50000);
  int l = dirents(fd, buf, 50000);
  display_contents(l, buf);

  pop();
}

void init () {
  base_path[0] = '\0';
}

