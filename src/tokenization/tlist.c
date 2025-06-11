#include "tlist.h"
#include <stdio.h>
#include <stdlib.h>

struct tnode {
    token_t data;
    tlist_t next;
};

// Allocates memory for a new node and takes ownership
// of the token
tlist_t tnode_new(const token_t t) {
    tlist_t n = (tlist_t)malloc(sizeof(tnode_t));
    if (!n) {
        return NULL;
    }

    n->data = t;
    n->next = NULL;

    return n;
}

// Inserts the node in the head of the list.
int tlist_insert_node(tlist_t* lp, tlist_t n) {
    if (!lp) {
        return 0;
    }

    n->next = *lp;
    *lp = n;

    return 1;
}

// Inserts the node created with the token
// in the head of the list.
int tlist_insert_token(tlist_t* restrict lp, const token_t restrict t) {
    tlist_t n = tnode_new(t);
    if (!n) {
        return 0;
    }

    return tlist_insert_node(lp, n);
}

// Inserts the node in the tail of the list.
int tlist_append_node(tlist_t* lp, tlist_t n) {
    if (!lp) {
        return 0;
    }

    if (!*lp) {
        *lp = n;
    } else {
        tlist_t l = *lp;
        while (l->next) {
            l = l->next;
        }
        l->next = n;
    }

    return 1;
}

// Inserts the node created with the token
// in the tail of the list.
int tlist_append_token(tlist_t* restrict lp, const token_t restrict t) {
    tlist_t n = tnode_new(t);
    if (!n) {
        return 0;
    }

    return tlist_append_node(lp, n);
}

void tlist_print(tlist_t l) {
    printf("tlist[");
    while (l) {
        token_print(l->data);
        if (l->next) {
            printf(", ");
        }
        l = l->next;
    }
    printf("]");
}

void tlist_free(tlist_t* lp) {
    if (!lp || !*lp) {
        return;
    }

    while (*lp) {
        tlist_t next = (*lp)->next;
        token_free(&(*lp)->data);
        free(*lp);
        *lp = next;
    }
}