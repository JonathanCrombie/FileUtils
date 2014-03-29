/* Public Domain.  See the LICENSE file. */

/* A simple file utility program to hexadecimal dump out the contents of a file. */
/* To compile on linux try:  gcc xdump.c -o xdump                                */

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void pline(unsigned char buf[50], int size, long long address);

int main(int argc , char * argv[])
{
  int               fp = 0;
  int               ret = 0;
  int               i = 0;
  long long         llint = 0;
  unsigned char     buf[50];
  off64_t           address = 0LL;
  off64_t           offset  = 0LL;


  /* check usage  */
  if (argc != 2 && argc != 3) {
    printf("\nUsage xdump [decimal offset] file_name\n");
    return 1;
  }

  if (argc == 3) {
    sscanf( argv[1], "%lld", &llint );
    if (llint < 0LL)
      llint = 0LL;
    offset = llint;

    fp = open(argv[2], O_RDONLY);
  }
  else
    fp = open(argv[1], O_RDONLY);

  if (fp <= 0) {
    printf("\nUnable to open file.\n");
    return 2;
  }

  printf("\nxdump\n\n");
  printf("File: %s\n\n", argv[argc == 3 ? 2 : 1]);
  printf("Address           Contents                               Printable\n");


  address = lseek64(fp, offset, 0);
  ret = read(fp, buf, 16);
  while (ret == 16) {
    pline(buf, 16, llint);  
    ret = read(fp, buf, 16);
    llint += 16LL;
  }

  if (ret != -1)
    pline(buf, ret, llint);

  printf("\n\n");

  close(fp);
}

void pline(unsigned char buf[50], int size, long long address)
{
  int  i = 0;
  printf("\n");
  printf("%016llx  ", address);
  for (i = 0; i < 4; i++) {
    if (i < size)
      printf("%02x", buf[i]);
    else
      printf("  ");         
  }
  printf(" ");

  for (i = 4; i < 8; i++) {
    if (i < size)
      printf("%02x", buf[i]);
    else
      printf("  ");         
  }
  printf("  ");

  for (i = 8; i < 12; i++) {
    if (i < size)
      printf("%02x", buf[i]);
    else
      printf("  ");         
  }
  printf(" ");

  for (i = 12; i < 16; i++) {
    if (i < size)
      printf("%02x", buf[i]);
    else
      printf("  ");         
  }
  printf("   ");

  for (i = 0; i < 8; i++) {
    if ( i >= size )
      printf(" ");
    else if (isalnum(buf[i]) || ispunct(buf[i]) || buf[i] == 0x20)
      printf("%c", buf[i]);
    else
      printf(".");
  }
  printf(" "); 
  for (i = 8; i < 16; i++) {
    if ( i >= size )
      printf(" ");
    else if (isalnum(buf[i]) || ispunct(buf[i]) || buf[i] == 0x20)
      printf("%c", buf[i]);
    else
      printf(".");
  }     
}
