#pragma once
#include "core.h"
#include "saha.h"

template <typename T>
struct List {
    Node<T> *head;
    usize length;
};

template <typename T>
List<T> *List_create(Arena *arena) {
    List<T> *list = arenaPushStruct(arena, List<T>);
    list->head = nullptr;
    list->length = 0;
    return list;
}

template <typename T>
void List_insert_at(Arena *arena, List<T> *list, T item, i32 index) {
    if (!list->head) {
        printf("list is empty\n");
        return;
    }
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return;
    } else if (list->length == index) {
        List_append(arena, list, item);
    } else if (index == 0) {
        List_prepend(arena, list, item);
    }
    list->length += 1;
    Node<T> *iter = list->head;
    i32 count = 0;
    while (iter) {
        if (count == index) {
            Node<T> *next_node = iter->next;
            Node<T> *new_node = Node_create(arena, item);
            if (next_node) {
                iter->next = new_node;
                new_node->next = next_node;
                return;
            } else {
                iter->next = new_node;
                return;
            }
        }
        iter = iter->next;
        count += 1;
    }
}

template <typename T>
void List_append(Arena *arena, List<T> *list, T item) {
    if (!list->head) {
        list->head = Node_create(arena, item);
        list->length += 1;
        return;
    }
    Node<T> *iter = list->head; 
    while (iter->next) { 
        iter = iter->next; 
    }
    iter->next = Node_create(arena, item);
    list->length += 1;
    return;
}

template <typename T>
void List_prepend(Arena *arena, List<T> *list, T item) {
    Node<T> *prepend_node = Node_create(arena, item);
    list->length += 1;
    if (!list->head) {
        list->head = prepend_node;
        return;
    }
    prepend_node->next = list->head;
    list->head = prepend_node;
}

template <typename T>
usize List_get_length(Arena *arena, List<T> *list) { return list->length; }

template <typename T>
Node<T> *List_remove(Arena *arena, List<T> *list, T item) {
    if (list->length == 0) {
        return NULL;
    }
    Node<T> *result = NULL;
    Node<T> *iter = list->head;
    Node<T> *prev = NULL;
    while (iter) {
        if (TYPE_eq(iter->data, item)) {
            if (iter == list->head) {
                result = list->head;
                if (list->head->next) {
                    list->head = list->head->next;
                } else {
                    list->head = NULL;
                }
                list->length -= 1;
                return result;
            }
            result = iter;
            if(iter->next) {
                prev->next = iter->next;
            } else {
                prev->next = NULL;
            }
            list->length -= 1;
            return result;
        }
        prev = iter;
        iter = iter->next;
    }
    return result;
}

template <typename T>
Node<T> *List_remove_at(Arena *arena, List<T> *list, i32 index) {
    if (index > list->length - 1 || index < 0) {
        printf("invalid index\n");
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    Node<T> *result = NULL;
    Node<T> *iter = list->head;
    Node<T> *prev = NULL;
    i32 count = 0;
    while (iter) {
        if (count == index) {
            if (iter == list->head && !list->head->next) {
                result = list->head;
                list->head = NULL;
                list->length -= 1;
                return result;
            } else if (iter == list->head && list->head->next) {
                result = list->head;
                list->head = list->head->next;
                list->length -= 1;
                return result;
            } else {
                result = iter;
                if (!iter->next) {
                    prev->next = NULL;
                    list->length -= 1;
                    return result;
                } else {
                    prev->next = iter->next;
                    list->length -= 1;
                    return result;
                }
            }
        }
        prev = iter;
        iter = iter->next;
        count += 1;
    }
    return result;
}

template <typename T>
Node<T> *List_get_at(Arena *arena, List<T> *list, i32 index) {
    if (list->length < index) {
        return NULL;
    }
    if (list->length == 0) {
        return NULL;
    }
    Node<T> *result = NULL;
    Node<T> *iter = list->head;
    i32 count = 0;
    while (iter != NULL) {
        if (count == index) {
            return iter;
        }
        iter = iter->next;
        count += 1;
    }
    return result;
}

template <typename T>
void List_destroy(Arena *arena, List<T> *list) {
    Node<T> *iter = list->head;
    while (iter) {
        Node<T> *destroyer = iter;
        iter = iter->next;
        Node_destroy(arena, &destroyer);
    }
    // free(*list);
    list->head = NULL;
    list->length = 0;
}

template <typename T>
void List_print(Arena *arena, List<T> *list) {
    Node<T> *iter = list->head; 
    printf("list.length: %llu\n", list->length); 
    while (iter) { 
        printf("list: {%d, %p}\n", iter->data, iter->next); 
        iter = iter->next; 
    }
}

