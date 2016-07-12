# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <stdarg.h>
# include <setjmp.h>
# include <sys/stat.h>
# include <direct.h>
# include <ctype.h>
# include <stdint.h>




#define VER     "0.0.1"
#define FALSE   0
#define TRUE    !FALSE

/* Structors */
typedef struct tag_IDX {
	char folder_flag ;  //如：=1为根目录，=2为2级目录，...
	long nam_idx ;      //确保long为4字节
	long dat_idx ;
	unsigned long unzip_size ;
} IDX ;
extern struct tag_IDX idx_info ;

/* Functions */
int error(const char *tips, ...);
int Readidx(FILE* idx_fp,unsigned int fpos, struct tag_IDX* idx_info) ;
int ReadName(FILE* nam_fp, long namidx,char* namestr) ;
unsigned int Scanidx(FILE* idx_fp,unsigned int filecount) ;
int Readdat(FILE* dat_fp,unsigned int datidx,unsigned long unzip_size,const char* filename) ;
int GetNamelist(char* folder_all,struct tag_IDX* idx_info,char* fname_buff) ;
