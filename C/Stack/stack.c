#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

stack* stack_new()
{
    stack *st = (stack*)malloc(sizeof(stack));
    st->count = 0;
    st->head = NULL;
    return st;
}

int stack_empty(stack *st)
{   
    if (st == NULL){
        return -1;
    }
    return st->count == 0;
}

void stack_push(stack *st, void *data)
{
    if (st == NULL){
        return ;
    }
    node *p = (node*)malloc(sizeof(node));
    p->data = data;
    p->next = st->head;
    st->head = p;
    st->count++;
    return ;
}

void* stack_pop(stack *st)
{
    if (st == NULL || st->count == 0){
        return NULL;
    }
    void *data;
    node *p = st->head;
    st->head = p->next;
    data = p->data;
    st->count--;
    free(p);
    p = NULL;
    return data;
}

void stack_foreach(stack *st,stack_node_op node_op)
{
    if (st == NULL){
        return ;
    }
    node *p = st->head;
    while(p != NULL){
        node_op(p->data);
        p = p->next;
    }
    return ;
}

void stack_destory(stack *st)
{
    if (st == NULL){
        return ;
    }
    node *p1 = st->head;
    node *p2;
    while(p1 != NULL){
        p2 = p1->next;
        FREE(p1);
        p1 = p2;
    }
    FREE(st);
}



