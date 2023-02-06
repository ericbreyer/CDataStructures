#include "./llist.h"
#include "../EBdefs.h"
#include "./llnode/llnode.h"

void llist_print(struct llist *this, FILE * out, void(*printThing)(void*thing, FILE * out)) {
    if(!this->head) {
        fprintf(out, "[]");
    }
    fprintf(out, "[");
    struct llnode * cur = this->head;
    for(; cur; cur = cur->next) {
        printThing(cur->value, out);
        fprintf(out, ", ");
    }
    fprintf(out, "\b\b]");
}

int llist_length(struct llist *this) {
    if(!this->head) {
        return 0;
    }
    int len = 0;
    struct llnode * cur = this->head;
    for(; cur; cur = cur->next, ++len);
    return len;
}

int llist_toArray(struct llist *this, void ***values) {
    if(!this->head) {
        return 0;
    }
    int len = this->length(this);
    struct llnode * cur = this->head;
    *values = malloc(sizeof **values * len);
    for(int i = 0; i < len; ++i) {
        (*values)[i] = cur->value;
        cur = cur->next;
    }
    return len;
}

void llist_clear(struct llist *this) {
    if(!this->last) {
        return;
    }
    struct llnode * cur = this->last->prev;
    for(; cur->prev; cur = cur->prev) {
        free(cur->next);
    }
    free(cur->next);
    free(cur);
    this->head = NULL;
    this->last = NULL;
}

bool llist_contains(struct llist *this, void * value) {
    if(!this->head) {
        return 0;
    }
    struct llnode * cur = this->head;

    for(; cur; cur = cur->next) {
        if(cur->value == value) {
            return true;
        }
    }
    return false;
}

int llist_find(struct llist *this, void * value) {
    if(!this->head) {
        return NOT_FOUND;
    }
    int len = 0;
    struct llnode * cur = this->head;
    for(; cur; cur = cur->next, ++len) {
        if(cur->value == value) {
            return len;
        }
    }
    return NOT_FOUND;
 }

void * llist_get(struct llist *this, int index) {
    if(!this->head) {
        return 0;
    }
    struct llnode * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    return cur->value;
}

void llist_set(struct llist *this, int index, void * value) {
    if(!this->head) {
        return;
    }
    struct llnode * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    cur->value = value;
}
void llist_append(struct llist *this, void * value) {
    if(!this->last) {
        struct llnode * new = malloc(sizeof *new);
        new->next = new->prev = NULL;
        new->value = value;

        this->last = new;
        if(!this->head) {
            this->head = new;
        }
        return;
    }
    struct llnode * new = malloc(sizeof *new);
    new->next = NULL;
    new->prev = this->last;
    new->value = value;
    this->last->next = new;
    this->last = new;
}
void llist_prepend(struct llist *this, void * value) {
    if(!this->head) {
        struct llnode * new = malloc(sizeof *new);
        new->next = new->prev = NULL;
        new->value = value;

        this->head = new;
        this->last = new;
        return;
    }
    struct llnode * new = malloc(sizeof *new);
    new->prev = NULL;
    new->next = this->head;
    new->value = value;

    this->head->prev = new;
    this->head = new;
}

void llist_insert(struct llist *this, int index, void * value) {
    if(!this->head) {
        return;
    }
    struct llnode * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }

    struct llnode * new = malloc(sizeof *new);
    new->next = new->prev = NULL;
    new->value = value;

    cur->prev->next = new;
    cur->prev = new;
}

struct llist *llist_copy(struct llist *this) {
    if(!this->head) {
        return construct_llist();
    }
    struct llist * newl = construct_llist();

    struct llnode * oldCur = this->head;

    for(;oldCur; oldCur = oldCur->next) {
        newl->append(newl, oldCur->value);
    }
    return newl;
}
void llist_remove(struct llist *this, int index) {
    if(!this->head) {
        return;
    }

    struct llnode * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }

    if(cur == this->head) {
        this->head = this->head->next;
    }
    else if(cur == this->last) {
        this->last = this->last->prev;
    }
    else {
        struct llnode * prev = cur->prev;
        struct llnode * next = cur->next;
        prev->next = next;
        next->prev = prev;
    }
    free(cur);
}

static void mergeSortInPlace(struct llist ** list, int length);

void llist_sort(struct llist *this) {
    mergeSortInPlace(&this, this->length(this));
}

void destroy_llist(struct llist *this) {
    this->clear(this);
}

struct llist *construct_llist() {
    struct llist * new = malloc(sizeof *new);
    new->head = NULL;
    new->last = NULL;
    
    new->append = llist_append;
    new->clear = llist_clear;
    new->contains = llist_contains;
    new->copy = llist_copy;
    new->find = llist_find;
    new->get = llist_get;
    new->insert = llist_insert;
    new->length = llist_length;
    new->prepend = llist_prepend;
    new->print = llist_print;
    new->remove = llist_remove;
    new->set = llist_set;
    new->toArray = llist_toArray;
    new->sort = llist_sort;

    return new;
    
};

static struct llist * llist_skip(struct llist *this, int index) {
    if(!this->head) {
        return 0;
    }
    struct llnode * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    struct llist * new = construct_llist();
    new->head = cur;
    new->last = this->last;
    return new;
}


static void mergeSortInPlace(struct llist ** list, int length) {

    // base case, a list of one is already sorted
    if (length <= 1) {
        return;
    }

    // split the array in half to sort recursively
    int firstLen = length / 2;
    int secondLen = length - length / 2;

    struct llist **firstHalf = list;    
    struct llist * temp = llist_skip(*list, firstLen);
    struct llist **secondHalf = &temp;

    // sort these two halves
    mergeSortInPlace(firstHalf, firstLen);
    mergeSortInPlace(secondHalf, secondLen);

    // initialize some stuff to merge the two
    // sorted halves into a big sorted array
    struct llist * sortedList = construct_llist();
    int firstIdx = 0;
    int secondIdx = 0;
    // int sortedIdx = 0;

    // while there are still elements that haven't been sorted
    while (firstIdx < firstLen || secondIdx < secondLen) {
        // if all the elements from the first half are sorted,
        // just fill the rest of the sortd buffer with the rest of the second
        if (firstIdx == firstLen) {
            sortedList->append(sortedList, llist_get(*secondHalf, secondIdx++));
            continue;
        }
        // if all the elements from the second half are sorted,
        // just fill the rest of the sortd buffer with the rest of the first
        if (secondIdx == secondLen) {
            sortedList->append(sortedList, llist_get(*firstHalf, firstIdx++));
            continue;
        }

        // fill the next spot in the sorted buffer with the smaller of the
        // two candidated from the two halves, update pointers accordingly
        if (llist_get(*firstHalf, firstIdx) < llist_get(*secondHalf, secondIdx)) {
            sortedList->append(sortedList, llist_get(*firstHalf, firstIdx++));
        } else {
            sortedList->append(sortedList, llist_get(*secondHalf, secondIdx++));       
        }
    }
    // copy from the sorted buffer back into the input files
    // (we are sorting in place) and free the memory

    for(int i = 0; i < length; ++i) {
        (*list)->set(*list, i, sortedList->get(sortedList, i));
    }

    destroy_llist(sortedList);
    free(sortedList);
}