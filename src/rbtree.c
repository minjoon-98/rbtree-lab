#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;
  p->nil = p->root = nil;
  nil->left = nil->right = nil;
  return p;
}

void delete_subtree(rbtree *t, node_t *node)
{
  if (node != t->nil)
  {
    delete_subtree(t, node->left);
    delete_subtree(t, node->right);
    free(node);
  }
}

void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
    delete_subtree(t, t->root);

  free(t->nil);
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
      if (uncle->color == RBTREE_RED) // case 1
      {
        newNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        newNode = newNode->parent->parent;
      }
      else
      {
        if (newNode == newNode->parent->right) // case 2
        {
          newNode = newNode->parent;
          left_rotate(t, newNode);
        }
        newNode->parent->color = RBTREE_BLACK; // case 3
        newNode->parent->parent->color = RBTREE_RED;
        right_rotate(t, newNode->parent->parent);
      }
    }
    else
    {
      uncle = newNode->parent->parent->left;
      if (uncle->color == RBTREE_RED) // case 1
      {
        newNode->parent->color = RBTREE_BLACK;
        uncle->color = RBTREE_BLACK;
        newNode->parent->parent->color = RBTREE_RED;
        newNode = newNode->parent->parent;
      }
      else
      {
        if (newNode == newNode->parent->left) // case 2
        {
          newNode = newNode->parent;
          right_rotate(t, newNode);
        }
        newNode->parent->color = RBTREE_BLACK; // case 3
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
  node_t *current = t->root;
  while (current != t->nil)
  {
    if (key == current->key)
      return current;
    else if (key < current->key)
      current = current->left;
    else
      current = current->right;
  }
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
    return NULL;
  node_t *current = t->root;
  while (current->left != t->nil)
  {
    current = current->left;
  }
  return current;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
    return NULL;
  node_t *current = t->root;
  while (current->right != t->nil)
  {
    current = current->right;
  }
  return current;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v)
{
  if (u->parent == t->nil)
    t->root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;
  v->parent = u->parent;
}

void rbtree_erase_fixup(rbtree *t, node_t *p)
{
  node_t *sibling;
  while (p != t->root && p->color == RBTREE_BLACK)
  {
    if (p == p->parent->left)
    {
      sibling = p->parent->right;
      if (sibling->color == RBTREE_RED) // case 1
      {
        sibling->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        left_rotate(t, p->parent);
        sibling = p->parent->right;
      }
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) // case 2
      {
        sibling->color = RBTREE_RED;
        p = p->parent;
      }
      else if (sibling->right->color == RBTREE_BLACK) // case 3
      {
        sibling->left->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        right_rotate(t, sibling);
        sibling = p->parent->right;
      }
      sibling->color = p->parent->color; // case 4
      p->parent->color = RBTREE_BLACK;
      sibling->right->color = RBTREE_BLACK;
      left_rotate(t, p->parent);
      p = t->root;
    }
    else
    {
      sibling = p->parent->left;
      if (sibling->color == RBTREE_RED) // case 1
      {
        sibling->color = RBTREE_BLACK;
        p->parent->color = RBTREE_RED;
        right_rotate(t, p->parent);
        sibling = p->parent->left;
      }
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) // case 2
      {
        sibling->color = RBTREE_RED;
        p = p->parent;
      }
      else if (sibling->left->color == RBTREE_BLACK) // case 3
      {
        sibling->right->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        left_rotate(t, sibling);
        sibling = p->parent->left;
      }
      sibling->color = p->parent->color; // case 4
      p->parent->color = RBTREE_BLACK;
      sibling->left->color = RBTREE_BLACK;
      right_rotate(t, p->parent);
      p = t->root;
    }
  }
  p->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *replaced;
  node_t *sucsessor;
  sucsessor = p;
  color_t erased_color = sucsessor->color;
  if (p->left == t->nil)
  {
    replaced = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    replaced = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    rbtree tmp;
    tmp.root = p->right;
    tmp.nil = t->nil;
    sucsessor = rbtree_min(&tmp);
    erased_color = sucsessor->color;
    replaced = sucsessor->right;
    if (sucsessor->parent == p)
    {
      replaced->parent = sucsessor;
    }
    else
    {
      rbtree_transplant(t, sucsessor, sucsessor->right);
      sucsessor->right = p->right;
      sucsessor->right->parent = sucsessor;
    }
    rbtree_transplant(t, p, sucsessor);
    sucsessor->left = p->left;
    sucsessor->left->parent = sucsessor;
    sucsessor->color = p->color;
  }
  if (erased_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, replaced);
  }

  free(p);
  return 0;
}

void inorder_traversal(const rbtree *t, const node_t *p, key_t *arr, size_t n, size_t index)
{
  if (p != t->nil)
  {
    if (index < n)
    {
      inorder_traversal(t, p->left, arr, n, index);  // 왼쪽 서브트리 순회
      arr[index++] = p->key;                         // 현재 노드의 값을 배열에 복사하고 인덱스 증가
      inorder_traversal(t, p->right, arr, n, index); // 오른쪽 서브트리 순회
    }
  }
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  size_t index = 0;                             // 배열 인덱스
  inorder_traversal(t, t->root, arr, n, index); // 트리를 오름차순으로 순회하면서 배열에 저장
  return 0;
}

// // insert 함수 확인
// // 트리를 출력하는 함수
// void print_rbtree(rbtree *t, node_t *node, int space)
// {
//   if (node == t->nil)
//     return;

//   space += 10;
//   print_rbtree(t, node->right, space);

//   printf("\n");
//   for (int i = 10; i < space; i++)
//     printf(" ");
//   printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

//   print_rbtree(t, node->left, space);
// }

// int main()
// {
//   rbtree *t = new_rbtree(); // 레드-블랙 트리 생성 함수
//   int key;

//   printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//   while (scanf("%d", &key) && key >= 0)
//   {
//     rbtree_insert(t, key);
//     print_rbtree(t, t->root, 0);
//   }

//   // 트리 메모리 해제
//   delete_rbtree(t);

//   return 0;
// }