
# include "global.h"


static jmp_buf theEND ;
static char eflag ;

struct tag_IDX idx_info ={ 0,0,0,0 };

int main(int argc,char *argv[])
{
    struct stat st; //用于计算文件大小

    FILE* idx_fp = NULL ;
    FILE* nam_fp = NULL ;
    FILE* dat_fp = NULL ;


    char idxnam_Buf[64] ;
    char namnam_Buf[64] ;
    char datnam_Buf[64] ;

    char fname_buff[64] ;  //解压文件名
    char folder_all[128] ; //完整路径（包括文件名）
    unsigned int file_count = 0;
    unsigned int result_count = 0;



    eflag = FALSE ;

    fputs("\n"
    "刀剑Online资源解压器 "VER"\n"
    "by Hhacker\n"
    "e-mail: hhacker@hhacker.com\n"
    "web:    hhacker.com\n"
    "\n", stdout);
    memset(folder_all,0x0,256) ;


    setjmp(theEND) ;
    if (eflag == FALSE) {
        if (argc == 1 || argc > 2) error("示例:bounzip package_name\n") ;
        else if (argc == 2) {
            sprintf(idxnam_Buf,"%s.idx",argv[1]) ;
            sprintf(namnam_Buf,"%s.nam",argv[1]) ;
            sprintf(datnam_Buf,"%s.dat",argv[1]) ;

            printf("开始查找文件...");
            idx_fp = fopen(idxnam_Buf,"rb") ;
            nam_fp = fopen(namnam_Buf,"rb") ;
            dat_fp = fopen(datnam_Buf,"rb") ;
            if (idx_fp == NULL) printf("\n%s文件不存在",idxnam_Buf) ;
            if (nam_fp == NULL) printf("\n%s文件不存在",namnam_Buf) ;
            if (dat_fp == NULL) printf("\n%s文件不存在",datnam_Buf) ;

            if (idx_fp == NULL || nam_fp == NULL || dat_fp == NULL)
                error("\n\n错误：文件列表不完整。");
            else printf("查找文件完成") ;
            printf("\n开始读取索引(.idx)文件...\n") ;
            stat(idxnam_Buf,&st) ;  //计算文件大小
            file_count = (st.st_size-97)/33 ;
            //printf("\n压缩包中总文件数(包括路径名)：%u\n",file_count) ;


            #ifdef DEBUG
            Readidx(idx_fp,0x4A2,&idx_info);       //0x4A2范例文件位置
            ReadName(nam_fp, idx_info.nam_idx,fname_buff) ;
            Readdat(dat_fp,idx_info.dat_idx,idx_info.unzip_size,"output_test.txt") ;
            #else
            unsigned int count_step = 0;
            int file_flag = FALSE ;
            for (count_step = 0; count_step < file_count; ++count_step ){
                Readidx(idx_fp,97+count_step*33,&idx_info);
                ReadName(nam_fp, idx_info.nam_idx,fname_buff) ;
                file_flag = GetNamelist(folder_all,&idx_info,fname_buff) ;
                if (file_flag == TRUE) {
                    printf("文件名:%s   解压后大小:%u   索引位置:%X\n",folder_all,(unsigned int)idx_info.unzip_size,(unsigned int)idx_info.dat_idx) ;
                    result_count += Readdat(dat_fp,idx_info.dat_idx,idx_info.unzip_size,folder_all) ;
                }
            }
            #endif

        }
    }
    printf("\n压缩包中总文件数：%u\n",(unsigned int)(file_count - (Scanidx(idx_fp,file_count) * 2 - 1))) ;
    printf("解压成功文件数：%u\n",(unsigned int)result_count) ;

    fclose(idx_fp) ;
    fclose(nam_fp) ;
    fclose(dat_fp) ;

    return 0;
}

int error(const char *tips, ...)
{
	char printf_buf[256];
	va_list args;
	int printed;

	va_start(args, tips);
	printed = vsprintf(printf_buf, tips, args);
	va_end(args);


	printf("%s",printf_buf) ;

	eflag = TRUE ;
    longjmp(theEND,1) ;

	return 0 ;
}

