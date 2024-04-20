#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->nil = p->root = nil;
  return p;
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

void left_rotate(rbtree *t, node_t *x)
{
  node_t *y;
  y = x->right;
  x->right = y->left;
  if (y->left != t->nil)
    y->left->parent = x;
  y->parent = x->parent;
  if (x->parent == t->nil)
    t->root = y;
  else if (x == x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void right_rotate(rbtree *t, node_t *y)
{
  node_t *x;
  x = y->left;
  y->left = x->right;
  if (x->right != t->nil)
    x->right->parent = y;
  x->parent = y->parent;
  if (y->parent == t->nil)
    t->root = x;
  else if (y == y->parent->right)
    y->parent->right = x;
  else
    y->parent->left = x;
  x->right = y;
  y->parent = x;
}

void rbtree_insert_fixup(rbtree *t, node_t *newNode)
{
  node_t *uncle;
  while (newNode->parent->color == RBTREE_RED)
  {
    if (newNode->parent == newNode->parent->parent->left)
    {
      uncle = newNode->parent->parent->right;
      if (uncle->color == RBTREE_RED)
      {
        newNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        newNode = newNode->parent->parent;
      }
      else
      {
        if (newNode == newNode->parent->right)
        {
          newNode = newNode->parent;
          left_rotate(t, newNode);
        }
        newNode->parent->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        right_rotate(t, newNode->parent->parent);
      }
    }
    else
    {
      uncle = newNode->parent->parent->left;
      if (uncle->color == RBTREE_RED)
      {
        newNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        newNode = newNode->parent->parent;
      }
      else
      {
        if (newNode == newNode->parent->left)
        {
          newNode = newNode->parent;
          right_rotate(t, newNode);
        }
        newNode->parent->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        left_rotate(t, newNode->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *current = t->root;
  node_t *previous = t->nil;
  node_t *newNode = (node_t *)calloc(1, sizeof(node_t));
  newNode->key = key;
  while (current != t->nil)
  {
    previous = current;
    if (newNode->key < current->key)
      current = current->left;
    else
      current = current->right;
  }
  newNode->parent = previous;
  if (previous == t->nil)
    t->root = newNode;
  else if (newNode->key < previous->key)
    previous->left = newNode;
  else
    previous->right = newNode;
  newNode->left = t->nil;
  newNode->right = t->nil;
  newNode->color = RBTREE_RED;
  rbtree_insert_fixup(t, newNode);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *x = t->root;
  node_t *y = t->nil;
  while (x != t->nil)
  {
    y = x;
    if (key == x->key)
      return x;
    else if (key < x->key)
      x = x->left;
    else
      x = x->right;
  }
  if (y == t->nil)
    return NULL;
  else
    return y;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;
  node_t *y = t->nil;
  while (x != t->nil)
  {
    y = x;
    x = x->left;
  }
  if (y == t->nil)
    return NULL;
  else
    return y;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  node_t *x = t->root;
  node_t *y = t->nil;
  while (x != t->nil)
  {
    y = x;
    x = x->right;
  }
  if (y == t->nil)
    return NULL;
  else
    return y;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
