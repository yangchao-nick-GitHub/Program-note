#include <stdio.h>
#include <stdlib.h>
#include "stack.h"
#include "common.h"

void node_dump(void* outdata)
{
    int *ret = (int*)outdata;
    fprintf(stdout,"%d\n",*ret);
    return ;
}

int main()
{
    /*
    int arr[]= {1,2,3,4};
    stack *st = stack_new();
    stack_push(st,&(arr[0]));
    stack_push(st,&(arr[1]));
    stack_push(st,&(arr[2]));
    stack_push(st,&(arr[3]));
    stack_foreach(st,node_dump);
    stack_destory(st);
    */
    FILE *fp = fopen("/root/123.txt","r");
    if (fp == NULL){
          log_cmd("file log %s","buf");
    }
  


    return 0;
}
