#include "./rbtree.h"
#include "../EBdefs.h"
#include "./rbnode/rbnode.h"

void rbtreePrintBookends(struct rbtree *this, FILE * out) {
    fprintf(out, "---------bh:%d rp: %s---------\n", (this->root ? this->root->vtable->bh(this->root) : 0), (this->root ? (this->root->vtable->rp(this->root) ? "✅" : "❌ ") : "✅"));
}

void rbtree_print(struct rbtree *this, FILE * out) {
    rbtreePrintBookends(this, out);
    if (!this->root) {
        return;
    }
    this->root->vtable->printTree(this->root, 0, out);
    rbtreePrintBookends(this, out);
}

int rbtree_getKeys(struct rbtree *this, int **keys) {
    if (!this->root) {
        return 0;
    }
    return this->root->vtable->getKeys(this->root, keys);
}

void rbtree_clear(struct rbtree *this) {
    if (!this->root) {
        return;
    }
    destroy_rbnode(this->root);
    free(this->root);
    this->root = nullptr;
}

struct rbtree *rbtree_copy(struct rbtree *this) {
    if (!this->root) {
        struct rbtree *new = construct_rbtree();
        return new;
    }
    struct rbtree *ret = construct_rbtree();
    ret->root = this->root->vtable->copy(this->root, nullptr);
    return ret;
}

int rbtree_contains(struct rbtree *this, int key) {
    if (!this->root) {
        return false;
    }
    return this->root->vtable->contains(this->root, key);
};

int rbtree_getValue(struct rbtree *this, int key) {
    if (!this->root) {
        return 0;
    }
    return this->root->vtable->getValue(this->root, key);
};

int rbtree_tryGetValue(struct rbtree *this, int key, int *out) {
    if (!this->root) {
        return false;
    }
    return this->root->vtable->tryGetValue(this->root, key, out);
};

int rbtree_setValue(struct rbtree *this, int key, int value) {
    if (!this->root) {
        return false;
    }
    return this->root->vtable->setValue(this->root, key, value);
}

int rbtree_insert(struct rbtree *this, int key, int value) {
    if (!this->root) {
        this->root = construct_rbnode(key, value, this->root, &this->root);
        return true;
    }
    int ret = this->root->vtable->insert(this->root, key, value);
    assert(this->checkValid(this));
    return ret;
};

int rbtree_remove(struct rbtree *this, int key) {
    if (!this->root) {
        return false;
    }
    int ret = this->root->vtable->remove(this->root, key, &this->root);
    assert(this->checkValid(this));
    return ret;
}

int rbtree_checkValid(struct rbtree *this) {
    if (!this->root) {
        return true;
    }
    return this->root->vtable->rp(this->root) && (this->root->vtable->bh(this->root) != -1);
}

void destroy_rbtree(struct rbtree *this) {
    this->clear(this);
}

struct rbtree *construct_rbtree() {
    struct rbtree *new = calloc(1, sizeof *new);
    new->root = nullptr;

    new->print = rbtree_print;
    new->getKeys = rbtree_getKeys;
    new->clear = rbtree_clear;
    new->copy = rbtree_copy;
    new->contains = rbtree_contains;
    new->getValue = rbtree_getValue;
    new->tryGetValue = rbtree_tryGetValue;
    new->setValue = rbtree_setValue;
    new->insert = rbtree_insert;
    new->remove = rbtree_remove;
    new->checkValid = rbtree_checkValid;

    return new;
}