/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: 2024年07月15日 星期一 17时25分33秒
 ************************************************************************/

#include<stdio.h>
#include"string_process.h"
#include <unistd.h>
#include "function.h"

int main()
{
    int flag = 0;
    size_t size = 0;   
    char pathname[64] = {0};

    while(1)
    {
        getcwd(pathname, sizeof(pathname));
        printf("linux@ubuntu:%s$ ",pathname);
        flag = string_process();
        
        if(flag == 12)
        {
            break;
        }
        else if(flag == 1)
        {
            touch_fun();
        }
        else if(flag == 2)
        {
            cp_fun();
        }
        else if(flag == 3)
        {
            mv_fun();
        }
        else if(flag == 13)
        {
            ls_fun();
        }
        else if(flag == 4)
        {
            ls_a_fun();
        }
        else if(flag == 6)
        {
            mkdir_fun();
        }
        else if(flag == 7)
        {
            rmdir_fun();
        }
        else if(flag == 8)
        {
            printf("%s\n",pathname); //pwd
        }
        else if(flag == 9)
        {
            chdir(pcut[1]);          //cd
            getcwd(pathname, sizeof(pathname));
        }
        else if(flag == 10)
        {
            ln_fun();
        }
        else if(flag == 11)
        {
            ln_s_fun();
        }
        else if(flag == 5)
        {
            ls_l_fun();
        }

        flag = 0;
        clean_strdata();
    }

    return 0;
}
