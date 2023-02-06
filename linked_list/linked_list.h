#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once


#define NOT_FOUND -1

struct linked_list_node;

struct linked_list {
    struct linked_list_node *head; // = nullptr;
    struct linked_list_node *last; // = nullptr;
    void (*print)(struct linked_list *this, FILE * out, void (*printThing)(void * thing,FILE * out)); //
    int (*toArray)(struct linked_list *this, void ***values); //
    int (*length)(struct linked_list *this);  //
    void (*clear)(struct linked_list *this); //
    struct linked_list *(*copy)(struct linked_list *this); //
    bool (*contains)(struct linked_list *this, void * value); //
    int (*find)(struct linked_list *this, void * value); //
    void * (*get)(struct linked_list *this, int index); //
    void (*set)(struct linked_list *this, int index, void * value); //
    void (*append)(struct linked_list *this, void * value); //
    void (*prepend)(struct linked_list *this, void * value); //
    void (*insert)(struct linked_list *this, int index, void * value); //
    void (*remove)(struct linked_list *this, int index); //
    void (*sort)(struct linked_list *this);
};

void destroy_llist(struct linked_list *this);
struct linked_list *construct_llist();