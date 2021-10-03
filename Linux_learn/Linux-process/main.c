#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int g_val = 10;

/*************************************************
Function: function1
Description: 通过返回值判断父子进程
*************************************************/
void function1()
{
    pid_t pid = fork();
    if (pid < 0){
        perror("fork error");
    }else if (pid == 0){
        fprintf(stdout,"I am child prcess\n");
    }else if (pid > 0){
        fprintf(stdout,"I am father prcess\n");
    }
}

/*************************************************
Function: function2
Description: 父子继承部分相同的数据区。指向同一物理地址
*************************************************/
void function2()
{
    pid_t pid = fork();
    if (pid < 0){
        perror("fork error");
    }else if (pid == 0){
        fprintf(stdout,"I am child prcess val %d %p\n",g_val,&g_val);
    }else if (pid > 0){
        fprintf(stdout,"I am father prcess val %d %p\n",g_val,&g_val);
    }
}

/*************************************************
Function: function3
Description: 父子继承部分相同的数据区。指向同一物理地址
*************************************************/
void function3()
{
    pid_t pid = fork();
    if (pid < 0){
        perror("fork error");
    }else if (pid == 0){
        g_val = 20;
        fprintf(stdout,"I am child prcess val %d %p\n",g_val,&g_val);
    }else if (pid > 0){
        sleep(2);
        fprintf(stdout,"I am father prcess val %d %p\n",g_val,&g_val);
    }
}

/*************************************************
Function: function4
Description: 一共有三个进程，A打印三次
*************************************************/
void function4()
{
    fork() || fork();
    fprintf(stdout,"A\n");
    exit(0);
}

/*************************************************
Function: function5
Description: 一共有三个进程，A打印三次
*************************************************/
void function5()
{
    int i;
    for (i = 0; i < 2; i++){
        fork();
        fprintf(stdout,"A\n");
        sleep(2);
    }
}

int main()
{
    //function1();
    //function2();
    //function3();
    //function4();
    //function5();
    return 0;
}
