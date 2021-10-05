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
    char *argv[] = {"ps","-p",NULL};   
    int ret = execvp("ps",argv);
    if (ret < 0){
        perror("error");
    }
    fprintf(stdout,"ret %d execl after\n",ret);
}

char cmd[256] = {0};

int get_cmd()
{
    memset(cmd,0,256);
    fprintf(stdout,"minisheell$> ");
    fflush(stdout);
    if (scanf("%s",cmd) == 0){ 
        getchar();
        return -1;
    }
    if (cmd[0] == '\n'){
            return -1;
    }
    return 1;
}


char **parse_buff()
{
    int argc = 0;
    char *p = cmd;
    static char *argv[32];
    while(*p != '\0'){
        if (*p != ' '){
            argv[argc++] = p;
            while(*p != ' ' && *p != '\0'){
                p++;
            }
        }else{
            while(*p == ' '){
                *p = '\0';
                p++;
            }
        }
    }
    argv[argc] = NULL;
    return argv; 
}

void excel_cmd()
{
    char **argv = {NULL};
    pid_t pid = fork();
    if (pid < 0){
        perror("error");
    }else if (pid == 0){
        argv = parse_buff();
        if (argv[0] == NULL){
            exit(-1);
        }
        execvp(argv[0],argv);
    }else if (pid > 0){
        waitpid(pid,NULL,0);
    }
    
    
}

/*************************************************
Function: function3
Description: 进程替换函数制作的简易shell
*************************************************/
void minishell()
{
    while(1){
        if (get_cmd() < 0){
            continue;
        }
        //fprintf(stdout,"cmd:%s\n",cmd);
        excel_cmd();
    }
}

int main()
{
    //function2();
    minishell();
    
    return 0;
}

