
# include "global.h"


static jmp_buf theEND ;
static char eflag ;

struct tag_IDX idx_info ={ 0,0,0,0 };

int main(int argc,char *argv[])
{
    struct stat st; //���ڼ����ļ���С

    FILE* idx_fp = NULL ;
    FILE* nam_fp = NULL ;
    FILE* dat_fp = NULL ;


    char idxnam_Buf[64] ;
    char namnam_Buf[64] ;
    char datnam_Buf[64] ;

    char fname_buff[64] ;  //��ѹ�ļ���
    char folder_all[128] ; //����·���������ļ�����
    unsigned int file_count = 0;
    unsigned int result_count = 0;



    eflag = FALSE ;

    fputs("\n"
    "����Online��Դ��ѹ�� "VER"\n"
    "by Hhacker\n"
    "e-mail: hhacker@hhacker.com\n"
    "web:    hhacker.com\n"
    "\n", stdout);
    memset(folder_all,0x0,256) ;


    setjmp(theEND) ;
    if (eflag == FALSE) {
        if (argc == 1 || argc > 2) error("ʾ��:bounzip package_name\n") ;
        else if (argc == 2) {
            sprintf(idxnam_Buf,"%s.idx",argv[1]) ;
            sprintf(namnam_Buf,"%s.nam",argv[1]) ;
            sprintf(datnam_Buf,"%s.dat",argv[1]) ;

            printf("��ʼ�����ļ�...");
            idx_fp = fopen(idxnam_Buf,"rb") ;
            nam_fp = fopen(namnam_Buf,"rb") ;
            dat_fp = fopen(datnam_Buf,"rb") ;
            if (idx_fp == NULL) printf("\n%s�ļ�������",idxnam_Buf) ;
            if (nam_fp == NULL) printf("\n%s�ļ�������",namnam_Buf) ;
            if (dat_fp == NULL) printf("\n%s�ļ�������",datnam_Buf) ;

            if (idx_fp == NULL || nam_fp == NULL || dat_fp == NULL)
                error("\n\n�����ļ��б�������");
            else printf("�����ļ����") ;
            printf("\n��ʼ��ȡ����(.idx)�ļ�...\n") ;
            stat(idxnam_Buf,&st) ;  //�����ļ���С
            file_count = (st.st_size-97)/33 ;
            //printf("\nѹ���������ļ���(����·����)��%u\n",file_count) ;


            #ifdef DEBUG
            Readidx(idx_fp,0x4A2,&idx_info);       //0x4A2�����ļ�λ��
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
                    printf("�ļ���:%s   ��ѹ���С:%u   ����λ��:%X\n",folder_all,(unsigned int)idx_info.unzip_size,(unsigned int)idx_info.dat_idx) ;
                    result_count += Readdat(dat_fp,idx_info.dat_idx,idx_info.unzip_size,folder_all) ;
                }
            }
            #endif

        }
    }
    printf("\nѹ���������ļ�����%u\n",(unsigned int)(file_count - (Scanidx(idx_fp,file_count) * 2 - 1))) ;
    printf("��ѹ�ɹ��ļ�����%u\n",(unsigned int)result_count) ;

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

