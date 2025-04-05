#include "core.h"
#include <stdio.h>
#include "saha.h"

struct vec3 {
    f32 x;
    f32 y;
    f32 z;
};

#include "Array.h"
#include "Map.h"
#include "Node.h"
#include "List.h"

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

    Array<i8> arr = Array_reserve<i8>(arena, 32);
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

void Map_test(Arena *arena) {
    puts("");
    printf("Map_i32:\n");
    Map<i32> *hashmap = Map_create<i32>(arena);
    printf("hashmap length = %llu\n", Map_length<i32>(arena, hashmap));
    if (!hashmap) {
        printf("nomem\n");
        exit(-1);
    }
    if (!Map_set<i32>(arena, hashmap, "dog", 3)) {
        printf("nomem\n");
        exit(-1);
    }
    i32 *result = Map_get<i32>(arena, hashmap, "dog");
    if (result) {
        printf("key = %s, val = %d\n", "dog", *result);
    }
    printf("hashmap length = %llu\n", Map_length<i32>(arena, hashmap));
    Map_destroy<i32>(arena, hashmap);

    puts("");
    printf("Map_vec:\n");
    Map<vec3> *hashmapvec = Map_create<vec3>(arena);
    printf("hashmapvec length = %llu\n", Map_length<vec3>(arena, hashmapvec));
    if (!hashmapvec) {
        printf("nomem\n");
        exit(-1);
    }
    if (!Map_set<vec3>(arena, hashmapvec, "dog", vec3{1.f, 0.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", Map_length<vec3>(arena, hashmapvec));
    if (!Map_set<vec3>(arena, hashmapvec, "frog", vec3{0.f, 1.f, 0.f})) {
        printf("nomem\n");
        exit(-1);
    }
    printf("hashmapvec length = %llu\n", Map_length<vec3>(arena, hashmapvec));
    vec3 *resultvec = Map_get<vec3>(arena, hashmapvec, "dog");
    if (resultvec) {
        printf("key = %s, val = {%f, %f, %f}\n", "dog", resultvec->x, resultvec->y, resultvec->z);
    }
    printf("hashmapvec length = %llu\n", Map_length<vec3>(arena, hashmapvec));
    printf("hash iterator...\n");
    MapIterator<vec3> itvec = MapIterator_create<vec3>(arena, hashmapvec);
    while (MapIterator_next<vec3>(arena, &itvec)) {
        printf("key = %s, val = {%f, %f, %f}\n", itvec.key, itvec.val.x, itvec.val.y, itvec.val.z);
    }
    Map_destroy<vec3>(arena, hashmapvec);

    puts("");
    printf("Map<Array<i32>>:\n");
    Map<Array<i32>> *hashmaparray = Map_create<Array<i32>>(arena);
    Array<i32> *resultarray = Map_get<Array<i32>>(arena, hashmaparray, "dog");
    if (!resultarray) {
        Map_set<Array<i32>>(arena, hashmaparray, "dog", Array<i32>{0});
        resultarray = Map_get<Array<i32>>(arena, hashmaparray, "dog");
    }
    printf("key = %s, val = %p", "dog", resultarray);
    *resultarray = Array_reserve<i32>(arena, 12);
    for (i32 i = 0; i < 12; i++) {
        resultarray->data[i] = i * i;
    }
    for (i32 i = 0; i < 12; i++) {
        printf("Array.data[%d] = %d\n", i, resultarray->data[i]);
    }
    printf("hashmapvec length = %llu\n", Map_length<Array<i32>>(arena, hashmaparray));

    printf("hash iterator...\n");
    MapIterator<Array<i32>> itarr = MapIterator_create<Array<i32>>(arena, hashmaparray);
    while (MapIterator_next<Array<i32>>(arena, &itarr)) {
        printf("key = %s, val = {%llu, %llu, %p}\n", itarr.key, itarr.val.length, itarr.val.border, itarr.val.data);
        Array_destroy<i32>(arena, &itarr.val);
    }
    Map_destroy<Array<i32>>(arena, hashmaparray);
    printf("\n");
}

void List_test(Arena *arena) {
    printf("List_test:\n");
    List<i32> loi = {0};
    Node<i32> *node = NULL;
    List_append<i32>(arena, &loi, 11);
    List_append<i32>(arena, &loi, 22);
    List_append<i32>(arena, &loi, 33);
    List_append<i32>(arena, &loi, 44);
    List_print<i32>(arena, &loi);

    node = List_remove_at<i32>(arena, &loi, 0);
    if (node) {
        Node_destroy<i32>(arena, &node);
    }
    List_print<i32>(arena, &loi);
    node = List_remove_at<i32>(arena, &loi, 1);
    if (node) {
        Node_destroy<i32>(arena, &node);
    }
    List_print<i32>(arena, &loi);
    node = List_remove_at<i32>(arena, &loi, 1);
    if (node) {
        Node_destroy<i32>(arena, &node);
    }
    List_print<i32>(arena, &loi);
    node = List_remove_at<i32>(arena, &loi, 0);
    if (node) {
        Node_destroy<i32>(arena, &node);
    }
    List_print<i32>(arena, &loi);
    if (loi.length == 0) {
        printf("list is empty\n");
    }
    List_destroy<i32>(arena, &loi);

    printf("Array_List_i32:\n");
    Array<List<i32>> arrayoflists = {0};
    List<i32> *list = Array_append<List<i32>>(arena, &arrayoflists, List<i32>{0});
    if (!list) { printf("list invalid!\n"); }
    List_append<i32>(arena, list, 32);
    List_append<i32>(arena, list, 22);
    List_append<i32>(arena, list, 12);
    List_print<i32>(arena, list);
    list = Array_append<List<i32>>(arena, &arrayoflists, List<i32>{0});
    if (!list) { printf("list invalid!\n"); }
    List_append<i32>(arena, list, 16);
    List_append<i32>(arena, list, 26);
    List_append<i32>(arena, list, 36);
    List_print<i32>(arena, list);
    printf("array.length = %llu\n", arrayoflists.length);
    for (i32 i = 0; i < arrayoflists.length; ++i) {
        printf("list[%d] = \n", i);
        List<i32> list = arrayoflists.data[i];
        List_print<i32>(arena, &list);
    }
    printf("\n");
}


auto main(i32 argc, char *argv[]) -> i32 {
    printf("haikalxx\n");
    Arena arena = {};
    arenaInit(&arena);
    printf("Array_test:\n");
    printf("----------------------------\n");
    Array_test(&arena);
    printf("Map_test:\n");
    printf("----------------------------\n");
    Map_test(&arena);
    printf("List_test:\n");
    printf("----------------------------\n");
    List_test(&arena);
    return 0;
}
