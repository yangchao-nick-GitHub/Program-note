#ifndef _STACK_H_
#define _STACK_H_

#include <stdio.h>
#include <stdlib.h>

#define FREE(p)     \
    do{             \
        free(p);    \
        p = NULL;   \
    }while(0)     


typedef struct node{
    void *data;
    struct node *next;
}node;

typedef struct stack{
    int count;
    node *head;
}stack;

typedef void (*stack_node_op)(void *outdata);

stack* stack_new();
int stack_empty(stack *st);
void stack_push(stack *st, void *data);
void* stack_pop(stack *st);
void stack_foreach(stack *st,stack_node_op node_op);
void stack_destory(stack *st);

typedef stack T;






#endif