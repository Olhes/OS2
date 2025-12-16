#include "types.h"
#include "user.h"
#include "stat.h"
#include "fcntl.h"
#include "fs.h"

#define MAX_PATH 128
#define MAX_FILES 64

struct file_info {
  char name[DIRSIZ];
  int size;
  int type;
  int inum;
  int nlink;
};

int
read_dir(int fd, struct file_info *files, int max_files)
{
  struct dirent de;
  struct stat st;
  int count = 0;
  
  while(read(fd, &de, sizeof(de)) == sizeof(de)) {
    if(de.inum == 0)
      continue;
    
    if(count >= max_files)
      break;
    
    // Get file stats
    if(fstat(de.inum, &st) < 0) {
      printf(2, "lsx: cannot stat %s\n", de.name);
      continue;
    }
    
    // Copy file info
    strcpy(files[count].name, de.name);
    files[count].size = st.size;
    files[count].type = st.type;
    files[count].inum = st.ino;
    files[count].nlink = st.nlink;
    count++;
  }
  
  return count;
}

void
print_file_info(struct file_info *file)
{
  char type_char;
  
  switch(file->type) {
    case T_FILE:   type_char = '-'; break;
    case T_DIR:    type_char = 'd'; break;
    case T_DEV:    type_char = 'c'; break;
    default:       type_char = '?'; break;
  }
  
  printf(1, "%c %-10s %8d %4d %5d\n", 
         type_char, file->name, file->size, file->nlink, file->inum);
}

int
main(int argc, char *argv[])
{
  int fd;
  struct file_info files[MAX_FILES];
  int file_count;
  char path[MAX_PATH];
  
  if(argc > 2) {
    printf(2, "Usage: lsx [directory]\n");
    exit();
  }
  
  if(argc == 1) {
    strcpy(path, ".");
  } else {
    strcpy(path, argv[1]);
  }
  
  if((fd = open(path, O_RDONLY)) < 0) {
    printf(2, "lsx: cannot open %s\n", path);
    exit();
  }
  
  file_count = read_dir(fd, files, MAX_FILES);
  close(fd);
  
  if(file_count < 0) {
    printf(2, "lsx: error reading directory\n");
    exit();
  }
  
  // Print header
  printf(1, "Type Name      Size    Links Inode\n");
  printf(1, "---- -------- -------- ----- -----\n");
  
  // Print files
  for(int i = 0; i < file_count; i++) {
    print_file_info(&files[i]);
  }
  
  exit();
}
