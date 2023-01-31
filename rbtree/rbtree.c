#include "./rbtree.h"

#define nullptr 0x00
#define false 0
#define true 1

/**
 * @brief enum of the color of the node
 */
enum color {
    red,
    black
};

enum insertFixupCase {
    root,
    uncleRed,
    triangle,
    line,
    good
};
enum insertFixupOrientation {
    case_left,
    case_right
};
struct insertFixupInfo {
    enum insertFixupCase fc;
    enum insertFixupOrientation fo;
};

/**
 * @brief a class that represents a rbtree node
 */
struct rbnode {
    /**
     * @brief a pointer to the pointer of the tree object's root
     */
    struct rbnode **root; // = nullptr;
    /**
     * @brief this nodes color
     */
    enum color c; // = red;
    /**
     * @brief the left child of this node
     */
    struct rbnode *l; // = nullptr;
    /**
     * @brief the right child of this node
     */
    struct rbnode *r; // = nullptr;
    /**
     * @brief the parent of this node
     */
    struct rbnode *p; // = nullptr;
    /**
     * @brief the key of this node
     */
    int key;
    /**
     * @brief the value stored in this node
     */
    int value;

    /**
     * @brief provides a total ordering of keys.
     * K must overload operator-, ie. of a numeric type or IComparable
     *
     * @param[in] other an object of type K to compare
     * @return int 0 if equal, positive if this is greater, negative if other is greater
     */
    int (*compare)(struct rbnode *this, int other);
    /**
     * @brief delete this node from the tree, fixing pointers accordingly
     *
     * @param[in] parentsChilda pointer to the pointer the parent holds of the node to delete (used to update the parent's child pointer easily)
     */
    void (*commitApoptosis)(struct rbnode *this, struct rbnode **parentsChild);
    /**
     * @brief Get this node's uncle, if it exists
     *
     * @return struct rbnode* the uncle
     */
    struct rbnode *(*getUncle)(struct rbnode *this);
    /**
     * @brief Get the insert case
     *
     * @return insertFixupInfo the fixup case info
     */
    struct insertFixupInfo (*getInsertCase)(struct rbnode *this);
    /**
     * @brief the black magic to fix the red black tree after an insertion
     *
     */
    void (*insertFixup)(struct rbnode *this);
    /**
     * @brief perform a left rotation around this node
     */
    void (*rotateLeft)(struct rbnode *this);
    /**
     * @brief perform a right rotation around this node
     */
    void (*rotateRight)(struct rbnode *this);
    /**
     * @brief checks if a node is black (nil nodes are black)
     *
     * @param[in] n the node to check
     * @return true if it is black
     * @return false otherwise
     */
    int (*static_isBlack)(struct rbnode *n);
    /**
     * @brief checks if a node is red (nil nodes are black)
     *
     * @param[in] n the node to check
     * @return true if it is red
     * @return false otherwise
     */
    int (*static_isRed)(struct rbnode *n);
    /**
     * @brief Get the dbnode's sibling, if it exists
     *
     * @param[in] dbnode the double black node that needs fixing
     * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
     * @return struct rbnode* the sibling
     */
    struct rbnode *(*static_getSibling)(struct rbnode *dbnode, struct rbnode *parent);
    /**
     * @brief Get the dbnode's sibling's child furthest from the dbnode, if it exists
     *
     * @param[in] dbnode the double black node that needs fixing
     * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
     * @return struct rbnode* the sibling's far child
     */
    struct rbnode *(*static_getSiblingFarChild)(struct rbnode *dbnode, struct rbnode *parent);
    /**
     * @brief Get the dbnode's sibling's child nearest to the dbnode, if it exists
     *
     * @param[in] dbnode the double black node that needs fixing
     * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
     * @return struct rbnode* the sibling's near child
     */
    struct rbnode *(*static_getSiblingNearChild)(struct rbnode *dbnode, struct rbnode *parent);
    /**
     * @brief the black magic to fix the red black tree after a removal
     * cases and how to fix them referenced from here: https://medium.com/analytics-vidhya/deletion-in-red-black-rb-tree-92301e1474ea
     *
     * @param[in] dbnode the double black node that needs fixing
     * @param[in] parent the parent of the dbnode (needed because the db node could be a nil node)
     */
    void (*static_doubleBlackFixup)(struct rbnode *dbnode, struct rbnode *parent);
    /**
     * @brief a string representation of this (singular) node
     *
     * @return string representation
     */
    char *(*toString)(struct rbnode *this);
    /**
     * @brief update the root reference of the tree object
     * @param[in] root a pointer to the new root
     */
    void (*setRoot)(struct rbnode *this, struct rbnode *root);

    //    public:
    /**
     * @brief Construct a new rbnode object
     *
     * @param[in] key it's key
     * @param[in] value it's value
     * @param[in] parent it's parent
     * @param[in] r a pointer to the root node pointer, in case it needs to be updated
     */
    /**
     * @brief print the tree rooted at this node using a reverse pre order traversal
     *
     * @param[in] depth the depth of this node
     */
    void (*printTree)(struct rbnode *this, int depth);

    int (*getKeys)(struct rbnode *this, int *keys);
    /**
     * @brief makes a copy of the tree rooted at this node
     *
     * @param[in] parent the pointer to the parent of this copy
     * @return rbnode* the copy of the tree rooted here
     */
    struct rbnode *(*copy)(struct rbnode *this, struct rbnode *parent);
    /**
     * @brief check if the node with this key is in the tree rooted at this node
     *
     * @param[in] key the key to look for
     * @return true if the key is in this tree
     * @return false otherwise
     */
    int (*contains)(struct rbnode *this, int key);
    /**
     * @brief get the value of the node with the key in the tree rooted at this node
     *
     * NOTE: this method has undefined behavior if the key is not in this subtree
     *
     * @param[in] key the key to look for
     * @return V the value of the key if it exists in the tree, undefined value otherwise
     */
    int (*getValue)(struct rbnode *this, int key);
    /**
     * @brief gets the key's value if it is in the tree rooted at this node
     *
     * @param[in] key the key to look for
     * @param[out] out the location to put the value if found
     * @return true if the key is in this tree
     * @return false otherwise
     */
    int (*tryGetValue)(struct rbnode *this, int key, int *out);
    /**
     * @brief set the value of a node with corrsponding key in the tree rooted at this node
     *
     * @param[in] key the key of the node to set
     * @param[in] value the value to set
     * @return true if the node was in the tree and set
     * @return false otherwise
     */
    int (*setValue)(struct rbnode *this, int key, int value);
    /**
     * @brief insert a key value pair into the tree rooted at this node
     *
     * @param[in] key the key of the node to insert
     * @param[in] value the value to be stored in the node
     * @return true if the node wasn't in the subtree and thus inserted
     * @return false otherwise
     */
    int (*insert)(struct rbnode *this, int key, int value);
    /**
     * @brief remove a node with a key from the tree rooted at this node
     *
     * @param[in] key the key of the node to remove
     * @param[in] parentsChild a pointer to the pointer the parent holds of the node to delete (used to update the parent's child pointer easily)
     * @return true if the node was in the subtree and deleted
     * @return false otherwise
     */
    int (*remove)(struct rbnode *this, int key, struct rbnode **parentsChild);
    /**
     * @brief get the black height of the tree rooted at this node
     *
     * @return int the bh if it is valid or -1 if the black height property is broken
     */
    int (*bh)(struct rbnode *this);
    /**
     * @brief check to make sure the red property holds in the tree rooted at this node
     * true in the tree rooted at this node
     *
     * @return true if it does
     * @return false otherwise
     */
    int (*rp)(struct rbnode *this);
    /**
     * @brief Destroy the rbnode object and all its children recursively
     */
};

void rbnode_static_doubleBlackFixup(struct rbnode *dbnode, struct rbnode *parent);
void destroy_rbnode(struct rbnode *this);
struct rbnode *construct_rbnode(int key, int value, struct rbnode *parent, struct rbnode **r);

int rbnode_compare(struct rbnode *this, int other) {
    return this->key - other;
}

void rbnode_commitApoptosis(struct rbnode *this, struct rbnode **parentsChild) {
    // if there are two children (hardest case)
    //  make this node's key the key of it's successor, then delete the successor node
    if (this->l && this->r) {
        struct rbnode *prev = this;
        struct rbnode *cur = this->r;
        while (cur->l) {
            prev = cur;
            cur = cur->l;
        }
        this->key = cur->key;
        if (prev->r == cur) {
            // prev->r = nullptr;
            cur->commitApoptosis(cur, &(prev->r));
        } else {
            // prev->l = nullptr;
            cur->commitApoptosis(cur, &(prev->l));
        }
        return;
    }
    // if there is a left or right child
    // replace this node with it, and fixup if need be
    else if (this->l) {
        *parentsChild = this->l;
        this->l->p = this->p;
        if (this->c == red) {
            // nothing, all good
        } else if (this->l->c == red) {
            this->l->c = black;
        } else if (this->c == black) {
            rbnode_static_doubleBlackFixup(this->l, this->l->p);
        }
    } else if (this->r) {
        *parentsChild = this->r;
        this->r->p = this->p;
        if (this->c == red) {
            // nothing, all good
        } else if (this->r->c == red) {
            this->r->c = black;
        } else if (this->c == black) {
            rbnode_static_doubleBlackFixup(this->r, this->r->p);
        }
    }
    // if it is a leaf, just fixup if need be
    else {
        *parentsChild = nullptr;
        if (this->c == black) {
            rbnode_static_doubleBlackFixup(nullptr, this->p);
        }
    }
    // clear the pointers so it dosen't think it has children when it destructs
    this->l = nullptr;
    this->r = nullptr;
    // destruct
    destroy_rbnode(this);
    free(this);
}

struct rbnode *rbnode_getUncle(struct rbnode *this) {
    if (!this->p->p)
        return nullptr;
    int leftBranch = this->p == this->p->p->l;
    return leftBranch ? (this->p->p->r ? this->p->p->r : nullptr) : (this->p->p->l ? this->p->p->l : nullptr);
}

struct insertFixupInfo rbnode_getInsertCase(struct rbnode *this) {
    struct insertFixupInfo info = {0, 0};
    if (!this->p) {
        info.fc = root;
    } else if (!this->p->p || this->p->c == black) {
        info.fc = good;
    } else if (this->getUncle(this) && this->getUncle(this)->c == red) {
        info.fc = uncleRed;
        info.fo = (this->p == this->p->p->l) ? case_right : case_left;
    } else if (this == this->p->l && this->p == this->p->p->l) {
        info.fc = line;
        info.fo = case_left;
    } else if (this == this->p->r && this->p == this->p->p->l) {
        info.fc = triangle;
        info.fo = case_left;
    } else if (this == this->p->r && this->p == this->p->p->r) {
        info.fc = line;
        info.fo = case_right;
    } else if (this == this->p->l && this->p == this->p->p->r) {
        info.fc = triangle;
        info.fo = case_right;
    }
    return info;
};

void rbnode_insertFixup(struct rbnode *this) {
    int left = false;
    switch (this->getInsertCase(this).fc) {
    case root:
        this->c = black;
        break;
    case uncleRed:
        this->p->c = 1 - this->p->c;
        this->p->p->c = 1 - this->p->p->c;
        this->getUncle(this)->c = 1 - this->getUncle(this)->c;
        this->p->p->insertFixup(this->p->p);
        break;
    case line:
        left = (this->getInsertCase(this).fo == case_left);
        if (!left) {
            this->p->p->rotateLeft(this->p->p);
            this->p->c = black;
            this->p->l->c = red;
        } else {
            this->p->p->rotateRight(this->p->p);
            this->p->c = black;
            this->p->r->c = red;
        }
        break;
    case triangle:
        left = (this->getInsertCase(this).fo == case_left);
        if (!left) {
            this->p->rotateRight(this->p);
            this->r->insertFixup(this->r);
        } else {
            this->p->rotateLeft(this->p);
            this->l->insertFixup(this->l);
        }
        break;
    default:
        break;
    }
};

void rbnode_rotateLeft(struct rbnode *this) {
    struct rbnode *x = this;
    struct rbnode *y = this->r;
    this->r = y->l;
    if (y->l) {
        y->l->p = x;
    }
    y->p = x->p;
    if (!x->p) {
        this->setRoot(this, y);
    } else if (x == x->p->l) {
        x->p->l = y;
    } else {
        x->p->r = y;
    }
    y->l = x;
    x->p = y;
}

void rbnode_rotateRight(struct rbnode *this) {
    struct rbnode *x = this;
    struct rbnode *y = this->l;
    this->l = y->r;
    if (y->r) {
        y->r->p = x;
    }
    y->p = x->p;
    if (!x->p) {
        this->setRoot(this, y);
    } else if (x == x->p->r) {
        x->p->r = y;
    } else {
        x->p->l = y;
    }
    y->r = x;
    x->p = y;
}

int rbnode_static_isBlack(struct rbnode *n) {
    return !n || n->c == black;
}

int rbnode_static_isRed(struct rbnode *n) {
    return n && n->c == red;
}

struct rbnode *rbnode_static_getSibling(struct rbnode *dbnode, struct rbnode *parent) {
    if (!parent)
        return nullptr;
    int left = dbnode == parent->l;
    return left ? (parent->r ? parent->r : nullptr) : (parent->l ? parent->l : nullptr);
}

struct rbnode *rbnode_static_getSiblingFarChild(struct rbnode *dbnode, struct rbnode *parent) {
    if (!parent)
        return nullptr;
    int left = dbnode == parent->l;
    return left ? (parent->r ? parent->r->r : nullptr) : (parent->l ? parent->l->l : nullptr);
}

struct rbnode *rbnode_static_getSiblingNearChild(struct rbnode *dbnode, struct rbnode *parent) {
    if (!parent)
        return nullptr;
    int left = dbnode == parent->l;
    return left ? (parent->r ? parent->r->l : nullptr) : (parent->l ? parent->l->r : nullptr);
}

void rbnode_static_doubleBlackFixup(struct rbnode *dbnode, struct rbnode *parent) {
    if (!dbnode && !parent) {
        return;
    } else if (dbnode && !dbnode->p) {
        return;
    }
    struct rbnode *sib = rbnode_static_getSibling(dbnode, parent);
    if (!sib) {
        printf("⚠️⚠️⚠️ shouldn't be here ⚠️⚠️⚠️\n");
        assert(false);
        return;
    } else if (rbnode_static_isBlack(sib) && rbnode_static_isBlack(sib->l) && rbnode_static_isBlack(sib->r)) {
        sib->c = red;
        if (rbnode_static_isBlack(parent)) {
            rbnode_static_doubleBlackFixup(parent, parent->p);
        } else {
            parent->c = black;
        }
    } else if (rbnode_static_isRed(sib)) {
        enum color t = sib->c;
        sib->c = parent->c;
        parent->c = t;
        (parent->r == dbnode ? parent->rotateRight(parent) : parent->rotateLeft(parent));
        rbnode_static_doubleBlackFixup(dbnode, parent);
    } else if (rbnode_static_isBlack(sib) && rbnode_static_isBlack(rbnode_static_getSiblingFarChild(dbnode, parent)) && rbnode_static_isRed(rbnode_static_getSiblingNearChild(dbnode, parent))) {
        rbnode_static_getSiblingNearChild(dbnode, parent)->c = black;
        sib->c = red;
        (sib == parent->r) ? sib->rotateRight(sib) : sib->rotateLeft(sib);
        rbnode_static_doubleBlackFixup(dbnode, parent);
    } else if (rbnode_static_isBlack(sib) && rbnode_static_isRed(rbnode_static_getSiblingFarChild(dbnode, parent))) {
        enum color t = sib->c;
        sib->c = parent->c;
        parent->c = t;
        rbnode_static_getSiblingFarChild(dbnode, parent)->c = black;
        (parent->r == dbnode ? parent->rotateRight(parent) : parent->rotateLeft(parent));
    }
}

char *rbnode_toString(struct rbnode *this) {
    char *color;
    switch (this->c) {
    case red:
        color = "🟥";
        break;
    case black:
        color = "⬛";
        break;
    }
    char *ss = malloc(sizeof *ss * 50);
    sprintf(ss, "%s %d->%d", color, this->key, this->value);
    return ss;
}

void rbnode_setRoot(struct rbnode *this, struct rbnode *root) {
    *(this->root) = root;
}

void rbnode_printTree(struct rbnode *this, int depth) {
    printf(" ");
    for (int i = 0; i < depth; i++) {
        printf("│ ");
    }
    printf("\b%s\n", this->toString(this));
    if (this->r) {
        this->r->printTree(this->r, depth + 1);
    }
    if (this->l)
        this->l->printTree(this->l, depth + 1);
}

int rbnode_getKeys(struct rbnode *this, int *keys) {

    int numRKeys = 0;
    int numLKeys = 0;
    int *rkeys = nullptr;
    int *lkeys = nullptr;

    if (this->r) {
        numRKeys = this->r->getKeys(this, rkeys);
    }
    if (this->l) {
        numLKeys = this->l->getKeys(this, lkeys);
    }

    keys = malloc(sizeof(int) * numRKeys + numLKeys + 1);
    *keys = this->key;
    if (rkeys) {
        memcpy(keys + 1, rkeys, numRKeys);
    }
    if (lkeys) {
        memcpy(keys + 1 + numRKeys, lkeys, numLKeys);
    }
    free(rkeys);
    free(lkeys);
    return numRKeys + numLKeys + 1;
}

struct rbnode *rbnode_copy(struct rbnode *this, struct rbnode *parent) {
    // make a copy of this node
    struct rbnode *newme = construct_rbnode(this->key, this->value, parent, this->root);
    newme->c = this->c;
    // copy its subtrees, if they exist
    struct rbnode *newl = this->l ? this->l->copy(this->l, newme) : nullptr;
    struct rbnode *newr = this->r ? this->r->copy(this->r, newme) : nullptr;
    // update the pointers
    newme->l = newl;
    newme->r = newr;
    return newme;
}

int rbnode_contains(struct rbnode *this, int key) {
    // if this is the node we are looking for, it is
    if (this->key == key) {
        return true;
    }
    // see if we need to look in the left or right subtree
    struct rbnode **recurseOn = (this->compare(this, key) >= 0) ? &this->l : &this->r;
    // if that child dosen't exist, the node isn't in the tree,
    // otherwise, see if the subtree contains it
    if (!*recurseOn) {
        return false;
    }
    return (*recurseOn)->contains((*recurseOn), key);
}

int rbnode_getValue(struct rbnode *this, int key) {
    // if this is the node we are looking for, get it
    if (this->key == key) {
        return this->value;
    }
    // see if we need to look in the left or right subtree
    struct rbnode **recurseOn = (this->compare(this, key) >= 0) ? &this->l : &this->r;
    // if that child dosen't exist, the node isn't in the tree,
    // otherwise, get the value of the node in the subtree
    if (!*recurseOn) {
        return nullptr;
    }
    return (*recurseOn)->getValue((*recurseOn), key);
}

int rbnode_tryGetValue(struct rbnode *this, int key, int *out) {
    // if this is the node we are looking for, get it
    if (this->key == key) {
        *out = this->value;
        return true;
    }
    // see if we need to look in the left or right subtree
    struct rbnode **recurseOn = (this->compare(this, key) >= 0) ? &this->l : &this->r;
    // if that child dosen't exist, the node isn't in the tree,
    // otherwise, get the value of the node in the subtree
    if (!*recurseOn) {
        return false;
    }
    return (*recurseOn)->tryGetValue((*recurseOn), key, out);
}

int rbnode_setValue(struct rbnode *this, int key, int value) {
    // if this is the node we are looking for, update it
    if (this->key == key) {
        this->value = value;
        return true;
    }
    // see if we need to look in the left or right subtree
    struct rbnode **recurseOn = (this->compare(this, key) >= 0) ? &this->l : &this->r;
    // if that child dosen't exist, the node isn't in the tree,
    // otherwise, set the value of the node in the subtree
    if (!*recurseOn) {
        return false;
    }
    return (*recurseOn)->setValue((*recurseOn), key, value);
}

int rbnode_insert(struct rbnode *this, int key, int value) {
    // if we are they key, it is already in the tree
    if (this->compare(this, key) == 0)
        return false;
    // see if we need to look in the left or right subtree
    struct rbnode **recurseOn = (this->compare(this, key) >= 0) ? &this->l : &this->r;
    // if that child dosen't exist, the node isn't in the tree, so insert it
    // otherwise, insert the node in the subtree
    if (!*recurseOn) {
        *recurseOn = construct_rbnode(key, value, this, this->root);
        (*recurseOn)->insertFixup((*recurseOn));
        return true;
    } else {
        return (*recurseOn)->insert((*recurseOn), key, value);
    }
}

int rbnode_remove(struct rbnode *this, int key, struct rbnode **parentsChild) {
    // if this is the node we are looking for, delete
    if (this->key == key) {
        this->commitApoptosis(this, parentsChild);
        return true;
    }
    // see if we need to look in the left or right subtree
    struct rbnode **recurseOn = (this->compare(this, key) >= 0) ? &this->l : &this->r;
    // if that child dosen't exist, the node isn't in the tree
    // otherwise, remove the node from the subtree
    if (!*recurseOn) {
        return false;
    } else {
        return (*recurseOn)->remove((*recurseOn), key, recurseOn);
    }
}

int rbnode_bh(struct rbnode *this) {
    // if this is a leaf its bh is its color
    if (!this->l && !this->r) {
        return (this->c == red) ? 0 : 1;
    }
    // check left subtree, if it exists
    int lbh = this->l ? this->l->bh(this->l) : 0;
    // check right subtree, if it exists
    int rbh = this->r ? this->r->bh(this->r) : 0;
    // if one or both is broken, this is broken by default
    if (lbh == -1 || rbh == -1) {
        return -1;
    }
    // if they are equal, return their bh plus this node's color
    // else invalid
    return (lbh == rbh ? rbh + (this->c == red ? 0 : 1) : -1);
}

int rbnode_rp(struct rbnode *this) {
    // check left subtree, if it exists
    int lrp = this->l ? this->l->rp(this->l) : true;
    // check right subtree, if it exists
    int rrp = this->r ? this->r->rp(this->r) : true;
    // make sure this and its children don't make a new violation
    // then and everything together
    return (lrp && rrp && (this->c == red ? ((this->l ? this->l->c != red : true) && (this->r ? this->r->c != red : true)) : true));
}

void destroy_rbnode(struct rbnode *this) {
    if (this->r) {
        destroy_rbnode(this->r);
        free(this->r);
    }
    if (this->l) {
        destroy_rbnode(this->l);
        free(this->l);
    }
}

struct rbnode *construct_rbnode(int key, int value, struct rbnode *parent, struct rbnode **r) {
    struct rbnode *node = malloc(sizeof *node);
    node->l = nullptr;
    node->r = nullptr;

    node->root = r;
    node->p = parent;
    node->key = key;
    node->value = value;
    if (!parent) {
        node->c = black;
    }

    node->compare = rbnode_compare;
    node->commitApoptosis = rbnode_commitApoptosis;
    node->getUncle = rbnode_getUncle;
    node->getInsertCase = rbnode_getInsertCase;
    node->insertFixup = rbnode_insertFixup;
    node->rotateLeft = rbnode_rotateLeft;
    node->rotateRight = rbnode_rotateRight;
    node->static_isBlack = rbnode_static_isBlack;
    node->static_isRed = rbnode_static_isRed;
    node->static_getSibling = rbnode_static_getSibling;
    node->static_getSiblingFarChild = rbnode_static_getSiblingFarChild;
    node->static_getSiblingNearChild = rbnode_static_getSiblingNearChild;
    node->static_doubleBlackFixup = rbnode_static_doubleBlackFixup;
    node->toString = rbnode_toString;
    node->setRoot = rbnode_setRoot;
    node->printTree = rbnode_printTree;
    node->getKeys = rbnode_getKeys;
    node->copy = rbnode_copy;
    node->contains = rbnode_contains;
    node->getValue = rbnode_getValue;
    node->tryGetValue = rbnode_tryGetValue;
    node->setValue = rbnode_setValue;
    node->insert = rbnode_insert;
    node->remove = rbnode_remove;
    node->bh = rbnode_bh;
    node->rp = rbnode_rp;

    return node;
};

void rbtree_print(struct rbtree *this) {
    printf("---------bh:%d---------\n", (this->root ? this->root->bh(this->root) : 0));
    if (!this->root) {
        return;
    }
    this->root->printTree(this->root, 0);
    printf("----------------------\n");
}

int rbtree_getKeys(struct rbtree *this, int *keys) {
    if (!this->root) {
        return 0;
    }
    return this->root->getKeys(this->root, keys);
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
    ret->root = this->root->copy(this->root, nullptr);
    return ret;
}

int rbtree_contains(struct rbtree *this, int key) {
    if (!this->root) {
        return false;
    }
    return this->root->contains(this->root, key);
};

int rbtree_getValue(struct rbtree *this, int key) {
    if (!this->root) {
        return 0;
    }
    return this->root->getValue(this->root, key);
};

int rbtree_tryGetValue(struct rbtree *this, int key, int *out) {
    if (!this->root) {
        return false;
    }
    return this->root->tryGetValue(this->root, key, out);
};

int rbtree_setValue(struct rbtree *this, int key, int value) {
    if (!this->root) {
        return false;
    }
    return this->root->setValue(this->root, key, value);
}

int rbtree_insert(struct rbtree *this, int key, int value) {
    if (!this->root) {
        this->root = construct_rbnode(key, value, this->root, &this->root);
        return true;
    }
    int ret = this->root->insert(this->root, key, value);
    assert(this->checkValid(this));
    return ret;
};

int rbtree_remove(struct rbtree *this, int key) {
    if (!this->root) {
        return false;
    }
    int ret = this->root->remove(this->root, key, &this->root);
    assert(this->checkValid(this));
    return ret;
}

int rbtree_checkValid(struct rbtree *this) {
    if (!this->root) {
        return true;
    }
    return this->root->rp(this->root) && (this->root->bh(this->root) != -1);
}

void destroy_rbtree(struct rbtree *this) {
    this->clear(this);
}

struct rbtree *construct_rbtree() {
    struct rbtree *new = malloc(sizeof *new);
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