#include <stdio.h>
#include <string.h>

/*
* fwrite function
*/
void fwrite_demo()
{
    FILE *fp = fopen("./123.txt","w");
    if (fp == NULL){
        perror("error");
    }

    const char *msg = "hello bit\n";
    int count = 5;
    while(count--){
        int ret = fwrite(msg,strlen(msg),1,fp);
        fprintf(stdout,"ret %d\n",ret);
    }
    fclose(fp);
}

/*
* fread function
*/
void fread_demo()
{
    char buffer[256] = {0};
    const char *msg = "hello bit\n";
    FILE *fp = fopen("./123.txt","r");
    if (fp == NULL){
        perror("error");
    }
    while(1){
        if (feof(fp)){
            break;
        }
        size_t s = fread(buffer,strlen(),1,fp);
        
        if (s > 0){
            fprintf(stdout,"read file : %s\n",buffer);
            memset(buffer,0,256);
        }
        
    }
    fclose(fp);
}

/*
* write system call
*/
void func3()
{
    const char *msg0="hello printf\n";
    const char *msg1="hello pwrite\n";
    const char *msg2="hello write\n";
    printf("%s",msg0);
    fwrite(msg1,strlen(msg1),1,stdout);
    write(1,msg)
}


int main()
{
    fwrite_demo();
    fread_demo();
    return 0;
}