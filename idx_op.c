# include "global.h"

int Readidx(FILE* idx_fp,unsigned int fpos, struct tag_IDX* idx_info)
{
    unsigned char bytebuff[33] ;


    fseek(idx_fp, fpos, SEEK_SET) ;
    fread(bytebuff, sizeof(char), 33, idx_fp) ;

    idx_info->folder_flag = bytebuff[0] ;
    memcpy(&idx_info->nam_idx,&bytebuff[1],4) ;
    memcpy(&idx_info->dat_idx,&bytebuff[25],4) ;
    memcpy(&idx_info->unzip_size,&bytebuff[29],4) ;
    #ifdef DEBUG
    unsigned int bstep ;
    printf("\n33�ֽ���������:\n") ;
    for (bstep = 0; bstep < 33; ++bstep){
        printf("%.2X ",bytebuff[bstep]) ;
        if (bstep == 0 || bstep == 4 || bstep == 24 || bstep == 28) printf("\n") ;
    }
    printf("\n\n�ļ�·����־��\t%2.2X",(unsigned int)idx_info->folder_flag) ;
    printf("\n�ļ�������λ�ã�%8.8X",(unsigned int)idx_info->nam_idx) ;
    printf("\n��������λ�ã�\t%8.8X",(unsigned int)idx_info->dat_idx) ;
    #endif

    return 0 ;
}

unsigned int Scanidx(FILE* idx_fp,unsigned int filecount)
{
    unsigned int bstep ;
    unsigned char check_b ;
    unsigned int in_count = 0;
    unsigned int out_count = 0;

    fseek(idx_fp, 97, SEEK_SET) ;
    for (bstep = 0; bstep < filecount; ++bstep){
        fseek(idx_fp, 97+bstep*33, SEEK_SET) ;
        fread(&check_b, sizeof(char), 1, idx_fp) ;
        # ifdef DEBUG
        //if (check_b!=0x1) printf("·����־��%2.2X\n",check_b) ;
        # endif
        if (check_b==0x2) ++in_count ;
        if (check_b==0x3) ++out_count ;
    }
    //if (in_count - (out_count -1) != 0) error("����idx�ļ���·���޷����") ;
    return out_count ;
}
