/* Public Domain.  See the LICENSE file. */

/* A simple program to compare two files on a byte by byte basis. */
/* It will report the byte offsets and differing bytes.           */
 
/* To only check if two files are different, it's easier to       */
/* compare hashes on the files.  eg.  md5sum file1 file2          */

/* If the files are plain text files, using the 'diff' command    */
/* might suffice.                                                 */ 

/* To compile on linux try:  gcc fcomp.c -o fcomp                 */

#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc , char * argv[])
{
  int               fp1 = 0;
  int               fp2 = 0;
  int               ret1 = 0;
  int               ret2 = 0;
  int               i = 0;
  unsigned char     buf1[50];
  unsigned char     buf2[50];
  long long         address = 0LL;

  /* check usage  */
  if (argc != 3) {
    printf("\nUsage: fcomp file_name1 file_name2\n\n");
    return 1;
  }

  fp1 = open(argv[1], O_RDONLY);
  if (fp1 <= 0) {
    printf("\nUnable to open file1 \"%s\"\n", argv[1]);
    return 2;
  }

  fp2 = open(argv[2], O_RDONLY);
  if (fp2 <= 0) {
    printf("\nUnable to open file2 \"%s\"\n", argv[2]);
    return 3;
  }

  printf("\nfcomp\n\n");
  printf("Address           F1 F2\n");

  ret1 = read(fp1, buf1, 1);
  ret2 = read(fp2, buf2, 1);
  while (ret1 == 1 && ret2 == 1) {
    if (buf1[0] != buf2[0]) {
      printf("\n");
      printf("%016llx  ", address);
      printf("%02x ", buf1[0]);
      printf("%02x", buf2[0]);
    }
    
    ret1 = read(fp1, buf1, 1);
    ret2 = read(fp2, buf2, 1);   
    address++;
  }
  if ((ret1 != 1 && ret2 == 1) ||
      (ret1 == 1 && ret2 != 1) ) {
    printf("\n\nFiles are different sizes!!!!\n\n" );
  }

  printf("\n\n");

  close(fp1);
  close(fp2);
}

