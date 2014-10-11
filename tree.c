#define DT_DIR		4
#define BUFF_SIZE       50000

struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent  */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */  
  /* char           d_type; */
};

void puts(char*, int);
int open(char*);
int close(int);
int dirents(char*, struct linux_dirent*, int);
void* malloc(unsigned long);

void tree (char*);

int level = 0;
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
    base_path[i] = a[i];
  } 
  for (j = 0; j < lenb; j++, i++) {
    base_path[i] = b[j];
  }
  base_path[i] = '\0';
  return base_path;
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
  concat(base_path, path);
  return concat(base_path, "/");
}

void pop () {
  int l = len(base_path);
  if (base_path[l-1] == '/')
    l-=2;
  for (;l > 0; l--)
    if (base_path[l] == '/') {
      base_path[l+1] = '\0';
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

void display_contents(struct linux_dirent entries[]) {
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

  int fd = open(base_path);
  struct linux_dirent*buf = (struct linux_dirent*) malloc(BUFF_SIZE);
  dirents(fd, buf, BUFF_SIZE);
  close(fd);
  display_contents(buf);

  pop();
}

