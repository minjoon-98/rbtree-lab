#include "rbtree.h"

#include <stdlib.h>

// 새로운 레드-블랙 트리를 생성하는 함수
rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;    // nil 노드의 색을 검정으로 설정합니다.
  p->nil = p->root = nil;       // nil 노드를 트리의 루트이자 nil로 설정합니다.
  nil->left = nil->right = nil; // nil 노드의 왼쪽과 오른쪽 포인터를 자기 자신으로 설정합니다.  // rbtree.c:235 에서 sibling이 nil일 때, NULL 인 자식을 참조 할 때 오류 발생! 해결하기 위해 추가.
  return p;                     // 새로 생성된 레드-블랙 트리를 반환합니다.
}

// 서브트리를 삭제하는 함수
void delete_subtree(rbtree *t, node_t *node)
{
  if (node != t->nil)
  {
    delete_subtree(t, node->left);  // 왼쪽 서브트리를 삭제합니다.
    delete_subtree(t, node->right); // 오른쪽 서브트리를 삭제합니다.
    free(node);                     // 노드를 해제합니다.
  }
}

// 레드-블랙 트리를 삭제하는 함수
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  // 트리 노드의 메모리를 반환합니다.
  if (t->root != t->nil)
    delete_subtree(t, t->root);

  free(t->nil); // nil 노드의 메모리를 반환합니다.
  free(t);      // 트리의 메모리를 반환합니다.
}

// 왼쪽으로 회전하는 함수
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

// 오른쪽으로 회전하는 함수
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

// 노드가 삽입된 트리의 위반 여부를 확인하고 수정하는 함수
void rbtree_insert_fixup(rbtree *t, node_t *newNode)
{
  node_t *uncle;
  while (newNode->parent->color == RBTREE_RED)
  {
    if (newNode->parent == newNode->parent->parent->left) // 삽입된 노드의 부모가 조부모의 왼쪽 자식일 경우 (삽입된 노드의 삼촌이 오른쪽에 있는 경우)
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
    else // 삽입된 노드의 부모가 조부모의 왼쪽 자식일 경우 (삽입된 노드의 삼촌이 왼쪽에 있는 경우)
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

// 트리에 새로운 노드를 삽입하는 함수
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

// 주어진 키를 가진 노드를 레드-블랙 트리에서 검색하는 함수
node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *current = t->root; // 현재 노드를 루트로 설정하여 시작합니다.
  while (current != t->nil)  // 현재 노드가 nil 노드가 아닐 때까지 반복합니다.
  {
    if (key == current->key)
      return current; // 현재 노드의 키와 주어진 키가 일치하는 경우 해당 노드를 반환합니다.
    else if (key < current->key)
      current = current->left; // 주어진 키가 현재 노드의 키보다 작으면 왼쪽 서브트리로 이동합니다.
    else
      current = current->right; // 주어진 키가 현재 노드의 키보다 크면 오른쪽 서브트리로 이동합니다.
  }
  return NULL; // 주어진 키를 가진 노드를 찾지 못한 경우 NULL을 반환합니다.
}

// 최소 키를 가진 노드를 찾는 함수
node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil) // 루트가 nil인 경우 트리가 비어있으므로 NULL을 반환합니다.
    return NULL;
  node_t *current = t->root;      // 현재 노드를 루트로 설정합니다.
  while (current->left != t->nil) // 현재 노드의 왼쪽 자식이 nil이 아닐 때까지 반복하여 왼쪽 자식으로 이동합니다.
  {
    current = current->left;
  }
  return current; // 반복 후 현재 노드가 최소 키를 가진 노드입니다.
}

// 최대 키를 가진 노드를 찾는 함수
node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil) // 루트가 nil인 경우 트리가 비어있으므로 NULL을 반환합니다.
    return NULL;
  node_t *current = t->root;       // 현재 노드를 루트로 설정합니다.
  while (current->right != t->nil) // 현재 노드의 오른쪽 자식이 nil이 아닐 때까지 반복하여 오른쪽 자식으로 이동합니다.
  {
    current = current->right;
  }
  return current; // 반복 후 현재 노드가 최대 키를 가진 노드입니다.
}

// 노드 u의 부모를 노드 v의 부모로 이식하는 함수
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

// 노드가 삭제된 트리의 위반 여부를 확인하고 수정하는 함수
void rbtree_erase_fixup(rbtree *t, node_t *p)
{
  node_t *sibling;
  while (p != t->root && p->color == RBTREE_BLACK)
  {
    if (p == p->parent->left) // 삭제된 노드가 부모의 왼쪽 자식일 경우 (삭제된 노드의 형제가 오른쪽 자식일 경우)
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
      else
      {
        if (sibling->right->color == RBTREE_BLACK) // case 3
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
    }
    else // 삭제된 노드가 부모의 오른쪽 자식일 경우 (삭제된 노드의 형제가 왼쪽 자식일 경우)
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
      else
      {
        if (sibling->left->color == RBTREE_BLACK) // case 3
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
  }
  p->color = RBTREE_BLACK;
}

// 트리에서 노드를 삭제하는 함수
int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  node_t *replaced;  // 후임자가 삭제되고 후임자의 자리를 대체할 노드
  node_t *successor; // 삭제될 노드의 후임자
  successor = p;
  color_t erased_color = successor->color;
  // 삭제할 노드의 자식이 한 개 이하일 때 (삭제되는 노드의 색은 해당 노드의 색)
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
  else // 삭제할 노드의 자식이 두 개일 때 (삭제되는 노드의 색은 후임자의 노드의 색)
  {
    rbtree tmp;
    tmp.root = p->right;
    tmp.nil = t->nil;
    successor = rbtree_min(&tmp);    // 후임자는 삭제될 노드의 오른쪽 노드가 루트인 트리에서의 최소값
    erased_color = successor->color; // 삭제되는 색은 후임자의 색
    replaced = successor->right;     // 후임자 노드가 삭제되고 대체될 노드
    if (successor->parent == p)
    {
      replaced->parent = successor;
    }
    else
    {
      rbtree_transplant(t, successor, successor->right);
      successor->right = p->right;
      successor->right->parent = successor;
    }
    rbtree_transplant(t, p, successor);
    successor->left = p->left;
    successor->left->parent = successor;
    successor->color = p->color;
  }
  if (erased_color == RBTREE_BLACK) // 만약 삭제된 색이 검정색이라면?
  {
    rbtree_erase_fixup(t, replaced); // (doubly black 해결)
  }

  free(p);
  return 0;
}

// 중위 순회를 통해 트리를 배열로 변환하는 함수
void inorder_traversal(const rbtree *t, const node_t *p, key_t *arr, size_t n, int *index)
{
  if (p != t->nil)
  {
    if (*index < n)
    {
      inorder_traversal(t, p->left, arr, n, index);  // 왼쪽 서브트리 순회
      arr[(*index)++] = p->key;                      // 현재 노드의 값을 배열에 복사하고 인덱스 증가
      inorder_traversal(t, p->right, arr, n, index); // 오른쪽 서브트리 순회
    }
  }
}

// 트리를 배열로 변환하는 함수
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  int index = 0;                                 // 배열 인덱스 초기화
  inorder_traversal(t, t->root, arr, n, &index); // 트리를 오름차순으로 순회하면서 배열에 저장
  return 0;
}

// node_t *get_next_node(const rbtree *t, node_t *p)
// {
//   node_t *current = p->right;
//   if (current == t->nil) // 오른쪽 자식이 없으면
//   {
//     current = p;
//     while (1)
//     {
//       if (current->parent->right == current) // current가 오른쪽 자식인 경우
//         current = current->parent;           // 부모 노드로 이동 후 이어서 탐색
//       else
//         return current->parent; // current가 왼쪽 자식인 경우 부모 리턴
//     }
//   }
//   while (current->left != t->nil) // 왼쪽 자식이 있으면
//     current = current->left;      // 왼쪽 끝으로 이동
//   return current;
// }

// int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
// {
//   // TODO: implement to_array
//   if (t->root == t->nil || n == 0)
//   {
//     return 0; // 트리가 비어 있거나 배열의 크기가 0이면 0을 반환하고 아무 작업도 수행하지 않음
//   }

//   size_t index = 0; // 배열 인덱스를 추적하기 위한 변수

//   // RB 트리에서 가장 작은 노드부터 시작하여 배열에 값을 저장
//   node_t *current = rbtree_min(t);
//   while (current != t->nil && index < n)
//   {
//     arr[index++] = current->key;         // 현재 노드의 키 값을 배열에 저장하고 인덱스를 증가시킴
//     current = get_next_node(t, current); // 다음으로 큰 노드로 이동
//   }

//   return 0;
// }

////////////////////////////////////////////////////////////////////////
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

//   printf("삽입할 노드의 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//   while (scanf("%d", &key) && key >= 0)
//   {
//     rbtree_insert(t, key);
//     print_rbtree(t, t->root, 0);
//   }

// printf("삭제할 노드의 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//   while (scanf("%d", &key) && key >= 0)
//   {
//     node_t *tmp = rbtree_find(t, key);
//     rbtree_erase(t, tmp);
//     print_rbtree(t, t->root, 0);
//   }

//   // 트리 메모리 해제
//   delete_rbtree(t);

//   return 0;
// }
////////////////////////////////////////////////////////////////////////