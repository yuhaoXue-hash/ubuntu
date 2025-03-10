/*************************************************************************
	> File Name: main.c
	> Author: yas
	> Mail: rage_yas@hotmail.com
	> Created Time: Thu 30 May 2024 10:31:30 AM CST
 ************************************************************************/

#include <unistd.h>
#include "log.h"

int main()
{
    Log_Dir_Test();
    log_Init();

    while(1)
    {
        log_write(LOG_WARING, "OPENDIR FAIL!");
        sleep(2);
    }

    log_Deinit();

    return 0;
}
