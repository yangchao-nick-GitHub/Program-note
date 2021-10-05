/*
 * @Author: yangchao
 * @Date: 2021-10-04 15:40:32
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>


/*************************************************
Description: 
1.execl进程替换函数,调用成功无返回值，调用失败返回-1
2.新进程和原进程进程ID保持一致
*************************************************/

/*************************************************
Function: function1
Description: execl进程替换函数,该进程的用户空间代码和
数据完全被新程替换，从新进程开始执行，所以不会打印execl after
*************************************************/
void function1()
{
    fprintf(stdout,"pid=%d\n",getpid());
    execl("/bin/ps", "ps", "-f", (char*)0);
    fprintf(stdout,"execl after\n");
}

/*************************************************
Function: function2
Description: execl进程替换函数
*************************************************/
void function2()
{
    char *argv[] = {"ps","-f",NULL};   
    int ret = execvp("ps",argv);
    if (ret < 0){
        perror("error");
    }
    fprintf(stdout,"ret %d execl after\n",ret);
}



int main(int argc,char *argv[],char *envp[])
{
    //function2();
    for (int i = 0;i<argc;i++){
        fprintf(stdout,"argv[%d] %s\n",argc,argv[i]);
    }
    
    return 0;
}

