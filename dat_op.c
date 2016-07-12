# include "global.h"
# include <openssl/des.h>
# include "zlib.h"


int Readdat(FILE* dat_fp,unsigned int datidx,unsigned long unzip_size,const char* filename)
{
    unsigned long raw_size = 0 ;
    unsigned int raw_pos = datidx ;
    unsigned char* raw_data = NULL;
    unsigned char* unc_data = NULL;
    char xorkeyword[16]="shuangjianhebing" ;
    unsigned int block = 0x1 ;
    unsigned long block_offset =0x0 ;
    unsigned long block_tindx = 0x0 ;
    unsigned long block_tsize = 0x0 ;
    unsigned char zip_flag = 0x0 ;


    FILE* goal_fp ;
    goal_fp = fopen(filename,"wb") ;

    /* �жϷֿ� */
    fseek(dat_fp, datidx+1, SEEK_SET) ;
    fread(&block,sizeof(char), 1,dat_fp) ;
    //printf("\n�ֿ�����%d\n",block) ;
    /* ��ȡ�ֿ��ܳ��� */
    fseek(dat_fp, datidx+20+(block-2)*4, SEEK_SET) ;
    fread(&block_tindx,sizeof(char), 4,dat_fp) ;
    fseek(dat_fp, datidx+20+block*4 +1+block_tindx - 4, SEEK_SET) ;
    fread(&block_tsize,sizeof(char), 4,dat_fp) ;

    //printf("��ѹ�󳤶�:%4.4X\t�ֿ��ܳ���=%4.4X+%4.4X=%4.4X",unzip_size,block_tindx,block_tsize,block_tsize+block_tindx+1) ;

    unsigned int block_count ;
for (block_count = 1 ; block_count <= block ; ++block_count)
{

    if (block_count == 1) {
        fseek(dat_fp, datidx+20+(block-1)*4, SEEK_SET) ;  /*��ȡѹ����ʶ 0x1ѹ�� 0x0δѹ��*/
        fread(&zip_flag, sizeof(char), 1, dat_fp) ;

        fseek(dat_fp, datidx+20+(block-1)*4+1, SEEK_SET) ;
        fread(&raw_size, sizeof(char), 4, dat_fp) ;

        raw_data = malloc(raw_size) ;
        block_offset = 0x0 ;

    } else {
        fseek(dat_fp,datidx+20+(block_count-2)*4, SEEK_SET) ;
        fread(&block_offset,sizeof(char),4,dat_fp) ;
        //printf("\n�ֿ�ƫ��%4.4X",block_offset) ;
        //printf("\n�ֿ��С����%4.4X",datidx+20+block*4 +1 + block_offset - 5 + 1) ;

        fseek(dat_fp, datidx+20+block*4 +1 + block_offset - 5, SEEK_SET) ;  /*��ȡѹ����ʶ 0x1ѹ�� 0x0δѹ��*/
        fread(&zip_flag, sizeof(char), 1, dat_fp) ;

        fseek(dat_fp,datidx+20+block*4 +1 + block_offset - 5 + 1,SEEK_SET) ;
        fread(&raw_size, sizeof(char), 4, dat_fp) ;

        raw_data = malloc(raw_size) ;
    }
    raw_pos = datidx+20+block*4 +1+block_offset ;

    //printf("\n\nѹ�����ݳ��ȣ�\t%.8X(ʮ����:%u)\n",(unsigned int)raw_size,(unsigned int)raw_size) ;
    if (raw_data != NULL){
        fseek(dat_fp, raw_pos, SEEK_SET) ;
        fread(raw_data, sizeof(char), raw_size, dat_fp) ;

        /* �򵥵������� */
        unsigned long b_step = 0 ;
        unsigned int key_step = 0 ;
        for (b_step = 0; b_step < raw_size; ++b_step){
            raw_data[b_step] ^= xorkeyword[key_step++] ;
            key_step = (key_step >= 16) ? 0 : key_step ;
        }
        /* ���¿�ʼDES���� */
        unsigned long des_size = 0 ;
        unsigned char* des_data = NULL;
        unsigned char* cpy_data = NULL;

        des_size = ((long)(raw_size/8))*8 ;
        des_data = malloc(des_size) ;
        cpy_data = malloc(des_size) ;
        if (des_data !=NULL && cpy_data !=NULL){
            memcpy(des_data,raw_data,des_size) ;

            DES_cblock key= {'\x12','\x06','\x1d','\x04','\x0c','\x0e','\x04','\x0e'} ;
            DES_key_schedule keysched;
            DES_set_key((C_Block *)key, &keysched);
            unsigned int temp_step = 0;
            for (temp_step = 0 ; temp_step < (des_size / 8) ; ++temp_step)
                DES_ecb_encrypt((C_Block *)(des_data+temp_step*8),(C_Block *)(cpy_data+temp_step*8), &keysched, DES_DECRYPT);

            memcpy(raw_data,cpy_data,des_size) ;
        }
        else{
            free(raw_data) ;
            free(des_data) ;
            free(cpy_data) ;
            error("\n���󣺷����ڴ�ʧ�ܣ�") ;
        }

        free(des_data) ;
        free(cpy_data) ;
        /*DES�������*/

        #ifdef DEBUG
        FILE* out_fp ;
        out_fp = fopen("newput1","wb") ;
        fwrite(raw_data,sizeof(char),raw_size,out_fp) ;
        fclose(out_fp);
        /*  ���ֽ�ʮ��������ʾ����
        unsigned int bstep ;
        for (bstep = 0; bstep < raw_size; ++bstep){
            printf("%.2X ",(unsigned char)raw_data[bstep]) ;
            if (((bstep + 1 ) % 16)== 0) printf("\n") ;
        }
        */
        #endif

        /*Zlib��ѹ��ʼ*/

        if (unzip_size != raw_size && zip_flag ==0x1){//&& unzip_size != (block_tsize+block_tindx+1)){

            unc_data = malloc(unzip_size) ;
            if (unc_data != NULL){

                unsigned int have;
                z_stream strm;
                /* allocate inflate state */
                strm.zalloc = Z_NULL;
                strm.zfree = Z_NULL;
                strm.opaque = Z_NULL;
                strm.avail_in = 0;
                strm.next_in = Z_NULL;
                inflateInit(&strm);

                strm.avail_in = raw_size ;
                strm.next_in = raw_data;

                do {
                    strm.avail_out = 16384;
                    strm.next_out = unc_data;
                    inflate(&strm, Z_NO_FLUSH);

                    have = 16384 - strm.avail_out;
                    if (fwrite(unc_data,sizeof(char),have,goal_fp) != have) {
                        (void)inflateEnd(&strm);
                        printf("\nд�����") ;
                    }
                } while (strm.avail_out == 0);
                (void)inflateEnd(&strm);
            }
            else {
                free(raw_data) ;
                error("\n���󣺷����ڴ�ʧ�ܣ�") ;
            }
            free(unc_data) ;
        } /* ԭʼ���ݴ�С�����ѹ�������ݴ�С��ͬ��û��ʹ��zlibѹ�� */
    }else error("\n���󣺷����ڴ�ʧ�ܣ�") ;

    /* ����ļ� */
    if (unzip_size == raw_size)
        fwrite(raw_data,sizeof(char),unzip_size,goal_fp) ;
    else if (zip_flag == 0x0)
        fwrite(raw_data,sizeof(char),raw_size,goal_fp) ;


    free(raw_data) ;
}



    fclose(goal_fp) ;

    return 1 ;
}
