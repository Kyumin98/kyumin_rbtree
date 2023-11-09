#include "rbtree.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

rbtree *new_rbtree(void) {
    rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));
    tree->nil = (node_t *)calloc(1, sizeof(node_t));
    tree->nil->color = RBTREE_BLACK;
    tree->root = tree->nil;
    return tree;
}

void left_rotate(rbtree *t, node_t *x) {
    node_t *y = x->right;
    x->right = y->left;
    if (y->left != t->nil){
        y->left->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == t->nil){
        t->root = y;
    }
    else if (x == x->parent->left){
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

void right_rotate(rbtree *t, node_t *x) {
    node_t *y = x->left;
    x->left = y->right;
    if (y->right != t->nil){
        y->right->parent = x;
    }
    y->parent = x->parent;

    if (x->parent == t->nil){
        t->root = y;
    }
    else if (x == x->parent->right){
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }

    y->right = x;
    x->parent = y;
}

void delete_node(rbtree *t, node_t *n){
    if(n != t->nil){
        delete_node(t, n->left);
        delete_node(t, n->right);
        free(n);
    }
}

void delete_rbtree(rbtree *t) {
    //recursion
    delete_node(t, t->root);
    free(t->nil);
    free(t);
}

void rbtree_insert_fixup(rbtree *t, node_t *new_node){
    node_t *uncle;
    while (new_node->parent->color == RBTREE_RED){
        if (new_node->parent == new_node->parent->parent->left){
            uncle = new_node->parent->parent->right;
            if (uncle->color == RBTREE_RED){
                // case 1
                new_node->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                new_node->parent->parent->color = RBTREE_RED;
                new_node = new_node->parent->parent;
            }
            else {
                // case 2
                if (new_node == new_node->parent->right){
                    new_node = new_node->parent;
                    left_rotate(t, new_node);
                }
                // case 3
                new_node->parent->color = RBTREE_BLACK;
                new_node->parent->parent->color = RBTREE_RED;
                right_rotate(t, new_node->parent->parent);
            }
        }
        else {
            uncle = new_node->parent->parent->left;

            if (uncle->color == RBTREE_RED){
                new_node->parent->color = RBTREE_BLACK;
                uncle->color = RBTREE_BLACK;
                new_node->parent->parent->color = RBTREE_RED;
                new_node = new_node->parent->parent;
            }
            else {
                if (new_node == new_node->parent->left){
                    new_node = new_node->parent;
                    right_rotate(t, new_node);
                }
                new_node->parent->color = RBTREE_BLACK;
                new_node->parent->parent->color = RBTREE_RED;
                left_rotate(t, new_node->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *new_node = malloc(sizeof(node_t));
    node_t *root = t->root;
    node_t *nil = t->nil;
    new_node->key = key;
    while(root != t->nil){
        nil = root;
        if(new_node->key < nil->key){
            root = root->left;
        }else {
            root = root->right;
        }
    }
    new_node->parent = nil;
    if(nil == t->nil){
        t->root = new_node;
    }else if(new_node->key < nil->key){
        nil->left = new_node;
    }else{
        nil->right = new_node;
    }
    new_node->left = t->nil;
    new_node->right = t->nil;
    new_node->color = RBTREE_RED;
    rbtree_insert_fixup(t, new_node);

    return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
    node_t *current_node = t->root;
    while(current_node != t->nil && current_node->key != key) {
        if (current_node->key > key) {
            current_node = current_node->left;
        }
        else{
            current_node = current_node->right;
        }
    }

    if (current_node->key == key){
        return current_node;
    }

    return NULL;
}

node_t *rbtree_min(const rbtree *t) {
  node_t *current_minimum = t->root;

  while(current_minimum->left != t->nil){
      current_minimum = current_minimum->left;
  }

  return current_minimum;
}

node_t *node_min(const rbtree *t, node_t *n) {
    node_t *target;
    target = n;
    while(target->left != t->nil){
        target = target->left;
    }
    return target;
}

node_t *rbtree_max(const rbtree *t) {
    node_t *current_maximum = t->root;

    while(current_maximum->right != t->nil){
        current_maximum = current_maximum->right;
    }

    return current_maximum;
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v){
    v->parent = u->parent;
    if (u->parent == t->nil){
        t->root = v;
    }
    else if (u == u->parent->left){
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
}

void rbtree_erase_fixup(rbtree *t, node_t *target) {
    node_t *uncle;
    while(target != t->root && target->color == RBTREE_BLACK){
        if(target->parent->left == target){
            uncle = target->parent->right;
            if(uncle->color == RBTREE_RED){
                // case 1
                uncle->color = RBTREE_BLACK;
                target->parent->color = RBTREE_RED;
                left_rotate(t, target->parent);
                uncle = target->parent->right;
            }
            if(uncle->left->color == RBTREE_BLACK && uncle->right->color == RBTREE_BLACK){
                // case 2
                uncle->color = RBTREE_RED;
                target = target->parent;
            }
            else {
                if(uncle->right->color == RBTREE_BLACK){
                    // case 3
                    uncle->color = RBTREE_RED;
                    uncle->left->color = RBTREE_BLACK;
                    right_rotate(t, uncle);
                    uncle = target->parent->right;
                }
                // case 4
                uncle->color = target->parent->color;
                target->parent->color = RBTREE_BLACK;
                uncle->right->color = RBTREE_BLACK;
                left_rotate(t, target->parent);
                target = t->root;
            }
        }
        else{
            uncle = target->parent->left;
            if(uncle->color == RBTREE_RED){
                uncle->color = RBTREE_BLACK;
                target->parent->color = RBTREE_RED;
                right_rotate(t, target->parent);
                uncle = target->parent->left;
            }
            if(uncle->right->color == RBTREE_BLACK && uncle->left->color == RBTREE_BLACK){
                uncle->color = RBTREE_RED;
                target = target->parent;
            }
            else {
                if(uncle->left->color == RBTREE_BLACK){
                    uncle->color = RBTREE_RED;
                    uncle->right->color = RBTREE_BLACK;
                    left_rotate(t, uncle);
                    uncle = target->parent->left;
                }
                uncle->color = target->parent->color;
                target->parent->color = RBTREE_BLACK;
                uncle->left->color = RBTREE_BLACK;
                right_rotate(t, target->parent);
                target = t->root;
            }
        }
    }
    target->color = RBTREE_BLACK;
}

int rbtree_erase(rbtree *t, node_t *p) {
    if(p == NULL){
        return 0;
    }
    node_t *uncle= p;
    node_t *target;
    color_t stored_color = p->color;

    if (p->left == t->nil){
        target = p->right;
        rbtree_transplant(t, p, p->right);
    }
    else if (p->right == t->nil){
        target = p->left;
        rbtree_transplant(t, p, p->left);
    }
    else {
        uncle = node_min(t, p->right);
        stored_color = uncle->color;
        target = uncle->right;
        if (uncle->parent == p){
            target->parent = uncle;
        }
        else {
            rbtree_transplant(t, uncle, uncle->right);
            p->right->parent = uncle;
            uncle->right = p->right;
        }
        rbtree_transplant(t, p, uncle);
        uncle->left = p->left;
        p->left->parent = uncle;
        uncle->color = p->color;
    }
    if (stored_color == RBTREE_BLACK){
        rbtree_erase_fixup(t, target);
    }
    free(p);
    return 0;
}

int inorder_array(node_t *p, key_t *arr, const rbtree *t, int i){
    if (p == t->nil) {
        return i;
    }
    i = inorder_array(p->left, arr, t, i);
    arr[i++] = p->key;
    i = inorder_array(p->right, arr, t, i);
    return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n){
    node_t *target = t->root;
    inorder_array(target, arr, t, 0);
    return 0;
}