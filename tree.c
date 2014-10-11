#define DT_DIR		4
#define BUFF_SIZE       50000

struct linux_dirent {
  unsigned long  d_ino;     /* Inode number */
  unsigned long  d_off;     /* Offset to next linux_dirent  */
  unsigned short d_reclen;  /* Length of this linux_dirent */
  char           d_name[];  /* Filename (null-terminated) */  
  /* char        d_type; */
};

// declared in tree.asm
void puts(char*, int);
int open(char*);
int close(int);
int dirents(char*, struct linux_dirent*, int);
void* malloc(unsigned long);

// 
void tree (char*);

// current indentation level
int level = 0;

// the current directory
char current_path[2000];

int len (char *s) {
  int l = 0;
  for (;*s != '\0'; s++, l++);
  return l;
}

char* concat(char*a, char*b, char*buff) {
  int lena = len(a);
  int lenb = len(b);
  int i, j;
  for (i = 0; i < lena; i++)
    buff[i] = a[i];
  for (j = 0; j < lenb; j++, i++)
    buff[i] = b[j];
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

/* push a directory on to the current path */
char* push (char*path) {
  concat(current_path, path, current_path);
  return concat(current_path, "/", current_path);
}

/* move up one directory */
void pop () {
  int l = len(current_path) - 1;
  
  // skip trailing slashes
  if (current_path[l] == '/')
    l--;
  
  // rewind to the last slash
  for (;l > 0; l--)
    if (current_path[l] == '/') {
      current_path[l+1] = '\0';
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
  puts(" |", 2);
  for (i = 0; i < level; i++)
    puts("--", 2);
}

void display_contents(struct linux_dirent entries[]) {
  while (entries->d_ino > 0) {
    if (!strcmp(entries->d_name, ".") && 
	!strcmp(entries->d_name, "..")) {
      indent();
      println(entries->d_name);

      if (is_directory(entries)) {
	level++;
	tree(entries->d_name);
	level--;
      }
    }

    // linux_dirent's are variable length
    entries = (struct linux_dirent *) (((char *) entries) + entries->d_reclen);  
  }
}

void tree (char* path) {
  push(path);

  int fd = open(current_path);
  struct linux_dirent* buf = (struct linux_dirent*) malloc(BUFF_SIZE);
  dirents(fd, buf, BUFF_SIZE);
  close(fd);
  display_contents(buf);

  pop();
}

