#include "./rbtree.h"
#include "../EBdefs.h"
#include "./rbnode/rbnode.h"

struct rbnode;

struct rbtree {
    struct IMap super;
    struct rbnode *root; // = nullptr;
};


void rbtreePrintBookends(struct rbtree *this, FILE * out) {
    fprintf(out, "---------bh:%d rp: %s---------\n", (this->root ? this->root->vtable->bh(this->root) : 0), (this->root ? (this->root->vtable->rp(this->root) ? "✅" : "❌ ") : "✅"));
}

int rbtree_checkValid(struct rbtree *this) {
    if (!this->root) {
        return true;
    }
    return this->root->vtable->rp(this->root) && (this->root->vtable->bh(this->root) != -1);
}

void rbtree_print(struct IMap * mapThis, FILE * out,void (*printKey)(void * thing,FILE * out),void (*printVal)(void * thing,FILE * out)) {
    struct rbtree * this = (struct rbtree *)mapThis;
    rbtreePrintBookends(this, out);
    if (!this->root) {
        return;
    }
    this->root->vtable->printTree(this->root, 0, out, printKey, printVal);
    rbtreePrintBookends(this, out);
}

int rbtree_getKeys(struct IMap *mapThis, void ***keys) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return 0;
    }
    return this->root->vtable->getKeys(this->root, keys);
}

void rbtree_clear(struct IMap *mapThis) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return;
    }
    destroy_rbnode(this->root);
    free(this->root);
    this->root = nullptr;
}

struct IMap *rbtree_copy(struct IMap *mapThis) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return construct_rbtree();
    }
    struct rbtree *ret = (struct rbtree *)construct_rbtree();
    ret->root = this->root->vtable->copy(this->root, nullptr);
    return (struct IMap *)ret;
}

bool rbtree_contains(struct IMap *mapThis, void * key) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return false;
    }
    return this->root->vtable->contains(this->root, key);
};

void* rbtree_getValue(struct IMap *mapThis, void * key) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return 0;
    }
    return this->root->vtable->getValue(this->root, key);
};

bool rbtree_tryGetValue(struct IMap *mapThis, void * key, void **out) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return false;
    }
    return this->root->vtable->tryGetValue(this->root, key, out);
};

bool rbtree_setValue(struct IMap *mapThis, void * key, void * value) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return false;
    }
    return this->root->vtable->setValue(this->root, key, value);
}

bool rbtree_insert(struct IMap *mapThis, void * key, void * value) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        this->root = construct_rbnode(key, value, this->root, &this->root);
        return true;
    }
    int ret = this->root->vtable->insert(this->root, key, value);
    assert(rbtree_checkValid(this));
    return ret;
};

bool rbtree_remove(struct IMap *mapThis, void * key) {
    struct rbtree * this = (struct rbtree *)mapThis;
    if (!this->root) {
        return false;
    }
    int ret = this->root->vtable->remove(this->root, key, &this->root);
    assert(rbtree_checkValid(this));
    return ret;
}

void destroy_rbtree(struct IMap *mapThis) {
    mapThis->clear(mapThis);
}

static struct IMap vTable = {
    .print = rbtree_print,
    .clear = rbtree_clear,
    .contains = rbtree_contains,
    .destroy = destroy_rbtree,
    .getKeys = rbtree_getKeys,
    .getValue = rbtree_getValue,
    .tryGetValue = rbtree_tryGetValue,
    .insert = rbtree_insert,
    .remove = rbtree_remove,
    .setValue = rbtree_setValue,
    .copy = rbtree_copy
};

struct IMap *construct_rbtree() {
    struct rbtree *new = calloc(1, sizeof *new);
    new->root = nullptr;

    new->super = vTable;

    return (struct IMap *)new;
}