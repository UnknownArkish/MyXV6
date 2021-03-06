/*
 * 测试信号量的用户程序
 * 使用信号量进行互斥锁
 */

#include "types.h"
#include "stat.h"
#include "user.h"

// 总共生成的进程数量
#define PROCESS_COUNT 5
// 每个进程写入的次数/个数
#define WRITE_COUNT_PER_PROCESS 10000

void write_sh_var();

int sem_index;

int main(int argc, char **argv)
{
    printf(0, "APP_TEST_SEM\n");

    // 创建一个信号量
    sem_index = create_sem(1);

    int i;
    int pid;
    // 产生指定数量的进程
    for (i = 0; i < PROCESS_COUNT; i++)
    {
        pid = fork();
        if (pid < 0)
        {
            printf(0, "Error Fork!\n");
        }
        else if (pid == 0)
        {
            break;
        }
    }

    // 使每个进程进程共享变量的读写
    write_sh_var();
    if (pid > 0)
    {
        // 等待所有子进程执行完write_sh_var
        while (wait() != -1)
        {
        }
        // 输出sh_var最终的数值
        printf(0, "sh_var: %d\n", sh_var_read());
        sh_var_write(0);
        free_sem(sem_index);
        exit();
    }
    else if (pid == 0)
    {
        exit();
    }
    return 0;
}

void write_sh_var()
{
    int i;
    int result = 0;
    for (i = 0; i < WRITE_COUNT_PER_PROCESS; i++)
    {
        result++;
    }
    if( !sem_p(sem_index) ){
        sh_var_write( sh_var_read() + result );
        sem_v(sem_index);
    }
}