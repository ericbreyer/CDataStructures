#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma once


#define NOT_FOUND -1

struct llnode;

struct llist {
    struct llnode *head; // = nullptr;
    struct llnode *last; // = nullptr;
    void (*print)(struct llist *this, FILE * out, void (*printThing)(void * thing,FILE * out)); //
    int (*toArray)(struct llist *this, void ***values); //
    int (*length)(struct llist *this);  //
    void (*clear)(struct llist *this); //
    struct llist *(*copy)(struct llist *this); //
    bool (*contains)(struct llist *this, void * value); //
    int (*find)(struct llist *this, void * value); //
    void * (*get)(struct llist *this, int index); //
    void (*set)(struct llist *this, int index, void * value); //
    void (*append)(struct llist *this, void * value); //
    void (*prepend)(struct llist *this, void * value); //
    void (*insert)(struct llist *this, int index, void * value); //
    void (*remove)(struct llist *this, int index); //
    void (*sort)(struct llist *this);
};

void destroy_llist(struct llist *this);
struct llist *construct_llist();