#include "./linked_list.h"
#include "./linked_list_node/linked_list_node.h"

void llist_print(struct linked_list *this, FILE * out, void(*printThing)(void*thing, FILE * out)) {
    if(!this->head) {
        fprintf(out, "[]");
    }
    fprintf(out, "[");
    struct linked_list_node * cur = this->head;
    for(; cur; cur = cur->next) {
        printThing(cur->value, out);
        fprintf(out, ", ");
    }
    fprintf(out, "\b\b]");
}

int llist_length(struct linked_list *this) {
    if(!this->head) {
        return 0;
    }
    int len = 0;
    struct linked_list_node * cur = this->head;
    for(; cur; cur = cur->next, ++len);
    return len;
}

int llist_toArray(struct linked_list *this, generic_t**values) {
    if(!this->head) {
        return 0;
    }
    int len = this->length(this);
    struct linked_list_node * cur = this->head;
    *values = malloc(sizeof **values * len);
    for(int i = 0; i < len; ++i) {
        (*values)[i] = cur->value;
        cur = cur->next;
    }
    return len;
}

void llist_clear(struct linked_list *this) {
    if(!this->last) {
        return;
    }
    struct linked_list_node * cur = this->last->prev;
    for(; cur->prev; cur = cur->prev) {
        free(cur->next);
    }
    free(cur->next);
    free(cur);
    this->head = NULL;
    this->last = NULL;
}

bool llist_contains(struct linked_list *this, generic_t value) {
    if(!this->head) {
        return 0;
    }
    struct linked_list_node * cur = this->head;

    for(; cur; cur = cur->next) {
        if(cur->value == value) {
            return true;
        }
    }
    return false;
}

int llist_find(struct linked_list *this, generic_t value) {
    if(!this->head) {
        return NOT_FOUND;
    }
    int len = 0;
    struct linked_list_node * cur = this->head;
    for(; cur; cur = cur->next, ++len) {
        if(cur->value == value) {
            return len;
        }
    }
    return NOT_FOUND;
 }

generic_t llist_get(struct linked_list *this, int index) {
    if(!this->head) {
        return 0;
    }
    struct linked_list_node * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    return cur->value;
}

void llist_set(struct linked_list *this, int index, generic_t value) {
    if(!this->head) {
        return;
    }
    struct linked_list_node * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    cur->value = value;
}
void llist_append(struct linked_list *this, generic_t value) {
    if(!this->last) {
        struct linked_list_node * new = malloc(sizeof *new);
        new->next = new->prev = NULL;
        new->value = value;

        this->last = new;
        if(!this->head) {
            this->head = new;
        }
        return;
    }
    struct linked_list_node * new = malloc(sizeof *new);
    new->next = NULL;
    new->prev = this->last;
    new->value = value;
    this->last->next = new;
    this->last = new;
}
void llist_prepend(struct linked_list *this, generic_t value) {
    if(!this->head) {
        struct linked_list_node * new = malloc(sizeof *new);
        new->next = new->prev = NULL;
        new->value = value;

        this->head = new;
        this->last = new;
        return;
    }
    struct linked_list_node * new = malloc(sizeof *new);
    new->prev = NULL;
    new->next = this->head;
    new->value = value;

    this->head->prev = new;
    this->head = new;
}

void llist_insert(struct linked_list *this, int index, generic_t value) {
    if(!this->head) {
        return;
    }
    struct linked_list_node * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }

    struct linked_list_node * new = malloc(sizeof *new);
    new->next = new->prev = NULL;
    new->value = value;

    cur->prev->next = new;
    cur->prev = new;
}

struct linked_list *llist_copy(struct linked_list *this) {
    if(!this->head) {
        return construct_llist();
    }
    struct linked_list * newl = construct_llist();

    struct linked_list_node * oldCur = this->head;

    for(;oldCur; oldCur = oldCur->next) {
        newl->append(newl, oldCur->value);
    }
    return newl;
}
void llist_remove(struct linked_list *this, int index) {
    if(!this->head) {
        return;
    }

    struct linked_list_node * cur = this->head;
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
        struct linked_list_node * prev = cur->prev;
        struct linked_list_node * next = cur->next;
        prev->next = next;
        next->prev = prev;
    }
    free(cur);
}

static void mergeSortInPlace(struct linked_list ** list, int length);

void llist_sort(struct linked_list *this) {
    mergeSortInPlace(&this, this->length(this));
}

void destroy_llist(struct linked_list *this) {
    this->clear(this);
}

struct linked_list *construct_llist() {
    struct linked_list * new = malloc(sizeof *new);
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

static struct linked_list * llist_skip(struct linked_list *this, int index) {
    if(!this->head) {
        return 0;
    }
    struct linked_list_node * cur = this->head;
    for(int i = 0; i < index; ++i) {
        cur = cur->next;
    }
    struct linked_list * new = construct_llist();
    new->head = cur;
    new->last = this->last;
    return new;
}


static void mergeSortInPlace(struct linked_list ** list, int length) {

    // base case, a list of one is already sorted
    if (length <= 1) {
        return;
    }

    // split the array in half to sort recursively
    int firstLen = length / 2;
    int secondLen = length - length / 2;

    struct linked_list **firstHalf = list;    
    struct linked_list * temp = llist_skip(*list, firstLen);
    struct linked_list **secondHalf = &temp;

    // sort these two halves
    mergeSortInPlace(firstHalf, firstLen);
    mergeSortInPlace(secondHalf, secondLen);

    // initialize some stuff to merge the two
    // sorted halves into a big sorted array
    struct linked_list * sortedList = construct_llist();
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