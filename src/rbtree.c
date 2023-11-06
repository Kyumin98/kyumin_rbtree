#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
    rbtree *tree = (rbtree *)calloc(1, sizeof(rbtree));
    tree->nil = (node_t *)calloc(1, sizeof(node_t));
    tree->nil->color = RBTREE_BLACK;
    tree->root = tree->nil
    return tree;
}

void rotate(rbtree *t, node_t *target, char direction) {
    node_t **child_1, **child_2;

    if (direction == 'left') {
        child_1 = &target->right;
        child_2 = &target->left;
    } else {
        child_1 = &target->left;
        child_2 = &target->right;
    }

    node_t *swap = *child_1;
    *child_1 = *child_2;
    if (*child_2 != swap->nil) {
        (*child_2)->parent = target;
    }
    swap->parent = target->parent;

    if (target->parent == t->nil) {
        t->root = swap;
    } else if (target == target->parent->left) {
        target->parent->left = swap;
    } else {
        target->parent->right = swap;
    }

    *child_2 = target;
    target->parent = swap;
}

//component
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
    node_t *swap;
    while(new_node->parent->color == RBTREE_RED){
        if(new_node->parent == new_node->parent->parent->left){
            direction_1 = "left"
            direction_2 = "right"
        }else{
            direction_1 = "right"
            direction_2 = "left"
        }
        swap = new_node->parent->parent->right;
        if(swap->color == RBTREE_RED){
            new_node->parent->color = RBTREE_BLACK;
            swap->color = RBTREE_BLACK;
            new_node->parent->parent->color = RBTREE_RED;
            new_node = new_node->parent->parent
        }else if(new_node == new_node->parent->right){
            new_node = new_node->parent
            rotate(t, new_node, direction_1)
        }
        new_node->parent->color = RBTREE_BLACK;
        new_node->parent->parent = RBTREE_RED;
        rotate(t, new_node, direction_2)
    }
    t->root->color = RBTREE_BLACK
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
    node_t *new_node = malloc(sizeof(node_t));
    node_t *root = t->root
    node_t *nil = t->nil
    new_node->key = key
    while(root != t->nil){
        nil = root
        if(new_node->key < nil->key){
            root = root->left
        }else {
            root = root->right
        }
    }
    new_node->parent = nil
    if(nil == t->nil){
        t->root = new_node
    }else if(new_node->key < nil->key){
        nil->left = new_node
    }else{
        nil->right = new_node
    }
    new_node->left = t->nil
    new_node->right = t->nil
    new_node->color = RBTREE_RED
    rbtree_insert_fixup(t, new_node)
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}


int main(){

}