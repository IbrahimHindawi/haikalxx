#pragma once
#include "core.h"
#include "saha.h"

template <typename T>
struct Array {
    T *data;
    i32 length;
    i32 border;
};

template <typename T>
Array<T> Array_reserve(Arena *arena, u64 length) {
    Array<T> array = {0};
    if (length == 0) {
        return array;
    }
    array.length = length;
    array.data = (T *)arenaPushArray(arena, T, length);
    // array.data = malloc(sizeof(TYPE) * length);
    if (array.data == NULL) {
        printf("haikal::Memory Allocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    memset(array.data, 0, sizeof(T) * length);
    // #endif
    array.border = length;
    return array;
}

template <typename T>
void Array_destroy(Arena *arena, Array<T> *array) {
    array->border = 0;
    array->length = 0;
    // free(array->data);
    array->data = NULL;
}

template <typename T>
T *Array_resize(Arena *arena, Array<T> *array) {
    u64 old_border = array->border;
    array->border *= 2;
    array->data = (T *)arenaRealloc(arena, T, array->border, array->data, old_border);
    // array->data = realloc(arena, array->data, sizeof(TYPE) * array->border);
    if (array->data == NULL) {
        printf("haikal::Memory Reallocation Failure!\n");
        exit(-1);
    }
    // #ifdef DEBUG
    if (array->border > old_border) {
        u64 border_difference = array->border - old_border;
        char *cursor = (char *)array->data;
        memset(cursor + (sizeof(T) * old_border), 0, sizeof(T) * border_difference);
    }
    // #endif
    return array->data;
}

template <typename T>
T *Array_append(Arena *arena, Array<T> *array, T elem) {
    if (array->length == 0 && array->border == 0) { 
        array->length += 1;
        array->border += 1;
        array->data = (T *)arenaPushArray(arena, T, array->border);
        // array->data = realloc(NULL, sizeof(TYPE) * array->border);
        array->data[array->length - 1] = elem;
        // char *cursor = (char *)array->data;
        // memcpy(cursor, elem, array->unit_size);
        return array->data;
    }
    if (array->length == array->border ) {
        array->border *= 2;
        // TODO: check if it zeroes out properly
        Array_resize<T>(arena, array);
    }
    array->length += 1;
    array->data[array->length - 1] = elem;
    // char *cursor = (char *)array->data;
    // memcpy(cursor + ((array->length - 1) * array->unit_size), elem, array->unit_size);
    return &array->data[array->length - 1];
}

template <typename T>
int Array_is_empty(Arena *arena, Array<T> *array) {
    return array->length == 0 ? 1 : 0;
}
