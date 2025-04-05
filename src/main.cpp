#include "core.h"
#include <stdio.h>
#include "saha.h"

struct vec3 {
    f32 x;
    f32 y;
    f32 z;
};

#include "Array.h"

void Array_test(Arena *arena) {
    Array<char> string = Array_reserve<char>(arena, 27);
    for (i32 i = 0; i < string.length; ++i) {
        string.data[i] = 0b01100000 | i + 1;
    }
    string.data[string.length - 1] = '\0';
    printf("string: %s\n", string.data);
    Array_destroy(arena, &string);

    Array<vec3> vectors = Array_reserve<vec3>(arena, 10);
    for (i32 i = 0; i < vectors.length; ++i) {
        vectors.data[i].x = 1.0f;
        vectors.data[i].y = (f32)i;
        vectors.data[i].z = 3.141592f;
    }
    for (i32 i = 0; i < vectors.length; ++i) { 
        printf("vectors[%d] = {%f, %f, %f}\n", i, vectors.data[i].x, vectors.data[i].y, vectors.data[i].z); 
    }
    Array_destroy<vec3>(arena, &vectors);

    Array<i8> arr = {0};
    Array_reserve<i8>(arena, 32);
    Array_append<i8>(arena, &arr, 127);
    Array_append<i8>(arena, &arr, 23);
    Array_append<i8>(arena, &arr, 11);
    Array_append<i8>(arena, &arr, 8);
    Array_append<i8>(arena, &arr, 127);
    Array_append<i8>(arena, &arr, 23);
    Array_append<i8>(arena, &arr, 11);
    Array_append<i8>(arena, &arr, 8);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    arr.length = 0;
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    Array_append<i8>(arena, &arr, 0xBA);
    Array_append<i8>(arena, &arr, 0xBA);
    Array_append<i8>(arena, &arr, 0xBA);
    Array_append<i8>(arena, &arr, 0xBA);
    Array_append<i8>(arena, &arr, 0xBA);
    Array_append<i8>(arena, &arr, 0xBA);
    Array_append<i8>(arena, &arr, 0xBA);
    Array_append<i8>(arena, &arr, 0xBA);
    for (i32 i = 0; i < arr.length; ++i) { printf("arr[%d] = %d\n", i, arr.data[i]); }
    Array_destroy<i8>(arena, &arr);
    printf("\n");
}

auto main(i32 argc, char *argv[]) -> i32 {
    printf("haikalxx\n");
    Arena arena = {};
    arenaInit(&arena);
    printf("Array_test:\n");
    printf("----------------------------\n");
    Array_test(&arena);
    return 0;
}
