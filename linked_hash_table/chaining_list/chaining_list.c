#include "chaining_list.h"
#include <stdio.h>
struct Node * construct_Node(generic_t key, generic_t value) {
    struct Node * new = malloc(sizeof *new);
    new->key = key;
    new->value = value;
    new->next = NULL;
    return new;
}

bool ChainingList_insert(struct ChainingList * this, generic_t key, generic_t value) {
    struct Node *prev = NULL;
    struct Node *curr = this->root;
    while (curr != NULL) {
        if (key == curr->key) {
            return false;
        }
        prev = curr;
        curr = curr->next;
    }
    struct Node *newNode = construct_Node(key, value);
    if (!this->root) { // root is null
        this->root = newNode;
    } else {
        prev->next = newNode;
    }
    return true;
}

bool ChainingList_setValue(struct ChainingList * this, generic_t key, generic_t value) {
    struct Node *prev = NULL;
    struct Node *curr = this->root;
    while (curr != NULL) {
        if (key == curr->key) {
            curr->value = value;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

bool ChainingList_remove(struct ChainingList * this, generic_t key) {
    struct Node *prev = NULL;
    struct Node *curr = this->root;
    while (curr != NULL) {
        if (key == curr->key) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    if (prev && curr) { // key was found and is not root
        prev->next = curr->next;
        free(curr);
        curr = NULL;
    } else if (!prev && curr) { // key was root
        this->root = NULL;
        free(curr);
    } else { // key was not found
        return false;
    }
    return true;
}

bool ChainingList_tryGet(struct ChainingList * this, generic_t key, generic_t *out) {
    struct Node *curr = this->root;
    while (curr != NULL) {
        if (key == curr->key) {
            *out = curr->value;
            return true;
        }
        curr = curr->next;
    }
    return false;
}

int ChainingList_keys(struct ChainingList * this, generic_t *keys) {
    struct Node *curr = this->root;
    int numKeys = 0;
    while (curr != NULL) {
        *(keys + numKeys) = curr->key;
        ++numKeys;
        curr = curr->next;
    }
    return numKeys;
}

struct ChainingList * construct_ChainingList() {
    struct ChainingList * new = malloc(sizeof *new);
    new->root = NULL;
    return new;
    
}

void destroy_ChainingList(struct ChainingList * this) {
    struct Node *prev = NULL;
    struct Node *curr = this->root;
    while (curr != NULL) {
        prev = curr;
        curr = curr->next;
        free(prev);
    }
    this->root = NULL;
}