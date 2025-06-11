#ifndef TLIST_H
#define TLIST_H

#include "token.h"

typedef struct tnode tnode_t, *tlist_t;

// Allocates memory for a new node and copies the token pointer
tlist_t tnode_new(const token_t t);

// Inserts the node in the head of the list.
int tlist_insert_node(tlist_t* lp, tlist_t n);

// Inserts the node created with the token
// in the head of the list.
int tlist_insert_token(tlist_t* restrict lp, const token_t restrict t);

// Inserts the node in the tail of the list.
int tlist_append_node(tlist_t* lp, tlist_t n);

// Inserts the node created with the token
// in the tail of the list.
int tlist_append_token(tlist_t* restrict lp, const token_t restrict t);

void tlist_print(tlist_t l);

void tlist_free(tlist_t* lp);

#endif