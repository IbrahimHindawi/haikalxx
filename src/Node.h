#pragma once
#include "core.h"
#include "saha.h"

template <typename T>
struct Node {
    Node<T> *next;
    T data;
};

template <typename T>
Node<T> *Node_create(Arena *arena, T data) {
    // Node<T> *node = malloc(sizeof(Node<T>));
    Node<T> *node = (Node<T> *)arenaPushStruct(arena, Node<T>);
    if (node) {
        node->next = nullptr;
        node->data = data;
    }
    return node;
}

template <typename T>
void Node_destroy(Arena *arena, Node<T> **node) {
    *node = nullptr;
}

template <typename T>
T *Node_value(Arena *arena, Node<T> *node) { }

template <typename T>
T *Node_get(Arena *arena, Node<T> *node) { return node->data; }

