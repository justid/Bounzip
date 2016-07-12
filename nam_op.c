# include "global.h"

int ReadName(FILE* nam_fp, long namidx,char* namestr)
{
    char name_size ;
    char* name ;

    fseek(nam_fp, namidx, SEEK_SET) ;
    fread(&name_size, sizeof(char), 1, nam_fp) ;

    name = calloc((size_t)name_size, sizeof(char)) ;
    if (name != NULL)
    {
        fread(name, sizeof(char), (size_t)name_size, nam_fp) ;
#ifdef DEBUG
        printf("\n从.nam文件中提取文件名...") ;
        printf("\n文件名:%s\t%d字节",name, (int)name_size) ;
#endif

        strncpy(namestr,name,name_size) ;
    }
    else error("\n错误：分配内存失败！") ;
    free(name) ;
    return 0;
}
static char folder1[64] ;
static char folder2[64] ;
static char folder3[64] ;
static char folder4[64] ;
static char folder5[64] ;
static char folder6[64] ;
static char folder7[64] ;
static char folder8[64] ;
static unsigned int folder_div = 0;    //目录层级

int GetNamelist(char* folder_all,struct tag_IDX* idx_info,char* fname_buff)
{
    if (idx_info->folder_flag == 0x1)
    {
        switch (folder_div)
        {
        case 0:     //根目录
            sprintf(folder_all,"%s",fname_buff) ;
            break ;
        case 1:
            sprintf(folder_all,"%s\\%s",folder1,fname_buff) ;
            break ;
        case 2:
            sprintf(folder_all,"%s\\%s\\%s",folder1,folder2,fname_buff) ;
            break ;
        case 3:
            sprintf(folder_all,"%s\\%s\\%s\\%s",folder1,folder2,folder3,fname_buff) ;
            break ;
        case 4:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,fname_buff) ;
            break ;
        case 5:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,fname_buff) ;
            break ;
        case 6:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6,fname_buff) ;
            break ;
        case 7:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6,folder7,fname_buff) ;
            break ;
        case 8:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6,folder7,folder8,fname_buff) ;
            break ;
        default:
            break ;
        }
    }
    if (idx_info->folder_flag == 0x2)
    {
        ++folder_div;
        switch (folder_div)
        {
        case 0:     //根目录
            break ;
        case 1:
            strcpy(folder1,fname_buff) ;
            sprintf(folder_all,"%s",folder1) ;
            break ;
        case 2:
            strcpy(folder2,fname_buff) ;
            sprintf(folder_all,"%s\\%s",folder1,folder2) ;
            break ;
        case 3:
            strcpy(folder3,fname_buff) ;
            sprintf(folder_all,"%s\\%s\\%s",folder1,folder2,folder3) ;
            break ;
        case 4:
            strcpy(folder4,fname_buff) ;
            sprintf(folder_all,"%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4) ;
            break ;
        case 5:
            strcpy(folder5,fname_buff) ;
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5) ;
            break ;
        case 6:
            strcpy(folder6,fname_buff) ;
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6) ;
            break ;
        case 7:
            strcpy(folder7,fname_buff) ;
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6,folder7) ;
            break ;
        case 8:
            strcpy(folder8,fname_buff) ;
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6,folder7,folder8) ;
            break ;
        default:
            break ;
        }
        mkdir(folder_all) ;
    }
    if (idx_info->folder_flag == 0x3)
    {
        folder_div = folder_div ? folder_div - 1 : 0;
        switch (folder_div)
        {
        case 0:     //根目录
            memset(folder_all,0x0,256) ;
            break ;
        case 1:
            sprintf(folder_all,"%s",folder1) ;
            break ;
        case 2:
            sprintf(folder_all,"%s\\%s",folder1,folder2) ;
            break ;
        case 3:
            sprintf(folder_all,"%s\\%s\\%s",folder1,folder2,folder3) ;
            break ;
        case 4:
            sprintf(folder_all,"%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4) ;
            break ;
        case 5:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5) ;
            break ;
        case 6:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6) ;
            break ;
        case 7:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6,folder7) ;
            break ;
        case 8:
            sprintf(folder_all,"%s\\%s\\%s\\%s\\%s\\%s\\%s\\%s",folder1,folder2,folder3,folder4,folder5,folder6,folder7,folder8) ;
            break ;
        default:
            break ;
        }
    }
    //printf("当前层:%2.2X  索引位置%8.8X\t文件名：%s\n",folder_div,(unsigned int)idx_info->dat_idx,folder_all) ;
    return (idx_info->folder_flag == 0x1)? TRUE : FALSE ;
}
