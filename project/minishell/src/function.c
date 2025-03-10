/*************************************************************************
	> File Name: function.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月15日 星期一 19时20分22秒
 ************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>       
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

#include "function.h"

int touch_fun(void)
{
    int i = 1;

    if(pcut[1] == NULL)
    {
        printf("Touch: missing file operand\n");
        printf("Try 'touch --help' for more information.\n");
        return -1;
    }

    while(pcut[i] != NULL && i < 5)
    {
        fopen(pcut[i],"a+");
        i++;
    }

    return 0;
}

int cp_fun(void)
{
    if(pcut[1] == NULL || pcut[2] == NULL)
    {
        printf("cp: missing file operand\n");
        printf("Try 'cp --help' for more information.\n");
        return -1;
    }

    char ch = 0;
    FILE *pf1 = fopen(pcut[1],"r+");
    FILE *pf2 = fopen(pcut[2],"w+");
    
    if(pf1 == NULL || pf2 == NULL)
    {
        printf("fileopen fail!\n");
        fclose(pf1);
        fclose(pf2);
        return -1;
    }

    while(1)
    {
        ch = fgetc(pf1);
        if(ch == EOF)
        {
            break;
        }
        fputc(ch, pf2);
    }

    fclose(pf1);
    fclose(pf2);

    return 0;
}

int mv_fun(void)
{
    if(pcut[1] == NULL || pcut[2] == NULL)
    {
        printf("mv: missing file operand\n");
        printf("Try 'mv --help' for more information.\n");
        return -1;
    }

    int i = 0;
    int ret = 0;
    int pathflag = 0;
    char ch = 0;
    char cpstr[32] = {0};
    char pathname[64] = {0};

    strcpy(cpstr,pcut[2]);

    while(cpstr[i] != '\0')
    {
        if(cpstr[i] == '/')
        {
            pathflag = 1;
            break;
        }
        i++;
    }

    //printf("pathflag = %d\n",pathflag);
    
    if(pathflag == 0)
    {
        rename(pcut[1],pcut[2]);
    }
    else if(pathflag == 1)
    {
        FILE *fp2 = fopen(pcut[2], "w+");
        FILE *fp1 = fopen(pcut[1], "r+");
        if(fp1 == NULL)
        {
            printf("mv: missing file operand\n");
            printf("Try 'mv --help' for more information.\n");
            return -1; 
        }

        if(fp2 == NULL)
        {
            sprintf(pathname,"%s%s",pcut[2],pcut[1]);
            fp2 = fopen(pathname, "w+");
            if(fp2 == NULL)
            {
                printf("fopen fail!\n");
                return -1;
            }
        }

        while(1)
        {
            ch = fgetc(fp1);
            if(ch == EOF)
            {
                break;
            }
            fputc(ch, fp2);
        }
        
        ret = remove(pcut[1]);
        if(ret == -1)
        {
            remove(pcut[2]);
            printf("mv: missing file operand\n");
            printf("Try 'mv --help' for more information.\n");
            return -1;
        }
        fclose(fp1);
        fclose(fp2);

        return 0;
    }
}

int ls_fun(void)
{
    DIR *pdir = opendir("./");
    if(pdir == NULL)
    {
        printf("opendir fail\n");
    }
    struct dirent *pdirent = NULL;

    while(1)
    {
        pdirent = readdir(pdir);
        if(pdirent == NULL)
        {
            break;
        }
        if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name, "..") == 0)
        {
            continue;
        }
        printf("%s  ",pdirent->d_name);
    }
    printf("\n");

    closedir(pdir);

    return 0;
}

int ls_a_fun(void)
{
    DIR *pdir = opendir("./");
    if(pdir == NULL)
    {
        printf("opendir fail\n");
    }
    struct dirent *pdirent = NULL;

    while(1)
    {
        pdirent = readdir(pdir);
        if(pdirent == NULL)
        {
            break;
        }
        printf("%s  ",pdirent->d_name);
    }
    printf("\n");

    closedir(pdir);

    return 0;
}

int mkdir_fun(void)
{
    int ret = 0;

    ret = mkdir(pcut[1], 0777);    
    if(ret == -1)
    {
        printf("mkdir fail!\n");
        return -1;
    }

    return 0;
}

int rmdir_fun(void)
{
    int ret = 0;

    ret = rmdir(pcut[1]);
    if(ret == -1)
    {
        printf("rmdir fail!\n");
        return -1;
    }
    return 0;
}

int cd_fun(void)
{
    int ret = 0;

    ret = chdir(pcut[1]);

}

int ln_fun(void)
{
    int ret = 0;

    ret = link(pcut[1],pcut[2]);
    if(ret == -1)
    {
        printf("create link fail!\n");
        return -1;
    }

     return 0;
}

int ln_s_fun(void)
{
    int ret = 0;

    ret = symlink(pcut[2],pcut[3]);
    if(ret == -1)
    {
        printf("create softlink fail\n");
        return -1;
    }

    return 0;
}


int ls_l_fun(void)
{
    int linkflag = 0;
    char buff[32] = {0};
char *a[12]={"January","February","March","April","May",
    "June","July","August","September","October","November","December"};
    
    DIR *fd = opendir(".");
    if(fd == NULL)
    {
        printf("opendiren dir fail\n");
        return -1;
    }

    struct dirent *pdread = readdir(fd);
    if(pdread == NULL)
    {
        printf("readdir fail!\n");
        return -1;
    }

    while(pdread != NULL)
    {

    struct stat statbuf;

    int ret = lstat(pdread->d_name, &statbuf);
    if(ret < 0)
    {
        printf("fail stat!\n");
        return -1;
    }

    switch (statbuf.st_mode & S_IFMT)
    {
        case S_IFBLK:  putchar('b'); break;
        case S_IFCHR:  putchar('c'); break;
        case S_IFDIR:  putchar('d'); break;
        case S_IFIFO:  putchar('p'); break;
        case S_IFLNK:  putchar('l'); linkflag = 1; break;
        case S_IFREG:  putchar('-'); break;
        case S_IFSOCK: putchar('s'); break;
    }
    
    statbuf.st_mode & S_IRUSR ? putchar('r') : putchar('-');
    statbuf.st_mode & S_IWUSR ? putchar('w') : putchar('-');
    statbuf.st_mode & S_IXUSR ? putchar('x') : putchar('-');

    statbuf.st_mode & S_IRGRP ? putchar('r') : putchar('-');
    statbuf.st_mode & S_IWGRP ? putchar('w') : putchar('-');
    statbuf.st_mode & S_IXGRP ? putchar('x') : putchar('-');
    
    statbuf.st_mode & S_IROTH ? putchar('r') : putchar('-');
    statbuf.st_mode & S_IWOTH ? putchar('w') : putchar('-');
    statbuf.st_mode & S_IXOTH ? putchar('x') : putchar('-');
    
    struct tm *ptm = localtime(&statbuf.st_mtime);
    
    printf("%3ld",statbuf.st_nlink);

    struct passwd *pw = getpwuid(statbuf.st_uid);
    printf(" %s",pw->pw_name);

    struct group *pg = getgrgid(statbuf.st_gid);
    printf(" %s",pg->gr_name);

    printf("%6ld %s %d %02d:%02d",statbuf.st_size,a[ptm->tm_mon],ptm->tm_mday, ptm->tm_hour, ptm->tm_min);
    
    if(linkflag == 1)
    {
        readlink(pdread->d_name, buff, sizeof(buff));
        printf(" %s -> %s\n",pdread->d_name ,buff);
        linkflag = 0;
    }
    else
    {
        printf(" %s\n",pdread->d_name);
    }

    pdread = readdir(fd);
    }

    closedir(fd);
    return 0;
}


