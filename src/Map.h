#pragma once
#include "core.h"
#include "saha.h"

#define FNV_OFFSET 14695981039346656037UL
#define FNV_PRIME 1099511628211UL

u64 Map_hash_key(const char* key) {
    u64 hash = FNV_OFFSET;
    for (const char* p = key; *p; p++) {
        hash ^= (u64)(unsigned char)(*p);
        hash *= FNV_PRIME;
    }
    return hash;
}

template <typename T>
struct MapEntry {
    const char *key;
    T val;
};

template <typename T>
struct Map {
    MapEntry<T> *entries;
    size_t length;
    size_t border;
};

template <typename T>
const char *Map_set_entry(MapEntry<T> *entries, usize border, const char *key, T val, usize *plength) {
    u64 hash = Map_hash_key(key);
    usize index = (usize)(hash & (u64)(border - 1));
    while (entries[index].key) {
        if (strcmp(key, entries[index].key) == 0) {
            entries[index].val = val;
            return entries[index].key;
        }
        index += 1;
        if (index >= border) {
            index = 0;
        }
    }
    if (plength) {
        key = _strdup(key);
        if (!key) {
            return NULL;
        }
        (*plength)++;
    }
    entries[index].key = (char *)key;
    entries[index].val = val;
    return key;
}


template <typename T>
bool Map_expand(Arena *arena, Map<T>* hashmap) {
    // Allocate new entries array.
    size_t new_capacity = hashmap->border * 2;
    if (new_capacity < hashmap->border) {
        return false;  // overflow (border would be too big)
    }
    // MapEntry_TYPE *new_entries = calloc(new_capacity, sizeof(MapEntry_TYPE));
    MapEntry<T> *new_entries = (MapEntry<T> *)arenaPushArray(arena, MapEntry<T>, new_capacity);
    if (!new_entries) {
        return false;
    }

    // Iterate entries, move all non-empty ones to new hashmap's entries.
    for (size_t i = 0; i < hashmap->border; i++) {
        MapEntry<T> entry = hashmap->entries[i];
        if (entry.key) {
            Map_set_entry<T>(new_entries, new_capacity, entry.key, entry.val, NULL);
        }
    }

    // Free old entries array and update this hashmap's details.
    // free(hashmap->entries);
    hashmap->entries = new_entries;
    hashmap->border = new_capacity;
    return true;
}

#define Map_TYPE_initial_capacity 16
template <typename T>
Map<T> *Map_create(Arena *arena) {
    // Map_TYPE *hashmap = malloc(sizeof(Map_TYPE));
    Map<T> *hashmap = (Map<T> *)arenaPushArray(arena, Map<T>, 1);
    if (!hashmap) {
        return NULL;
    }
    hashmap->length = 0;
    hashmap->border = Map_TYPE_initial_capacity;

    // hashmap->entries = calloc(hashmap->border, sizeof(MapEntry_TYPE));
    hashmap->entries = (MapEntry<T> *)arenaPushArray(arena, MapEntry<T>, hashmap->border);
    if (!hashmap->entries) {
        // free(hashmap);
        return NULL;
    }
    return hashmap;
}

template <typename T>
void Map_destroy(Arena *arena, Map<T> *hashmap) { }

template <typename T>
T *Map_get(Arena *arena, Map<T> *hashmap, const char *key) {
    u64 hash = Map_hash_key(key);
    usize index = (usize)(hash & (u64)(hashmap->border - 1));
    while (hashmap->entries[index].key) {
        if (strcmp(key, hashmap->entries[index].key) == 0) {
            return &hashmap->entries[index].val;
        }
        index += 1;
        if (index >= hashmap->border) {
            index = 0;
        }
    }
    return NULL;
}

template <typename T>
const char *Map_set(Arena *arena, Map<T> *hashmap, const char *key, T val) {
    // assert(val != NULL);
    // if (!val) {
    //     return NULL;
    // }
    if (hashmap->length >= hashmap->border / 2) {
        if(!Map_expand<T>(arena, hashmap)) {
            return NULL;
        }
    }
    return Map_set_entry<T>(hashmap->entries, hashmap->border, key, val, &hashmap->length);
}

template <typename T>
usize Map_length(Arena *arena, Map<T> *hashmap) { return hashmap->length; }

template <typename T>
struct MapIterator {
    const char *key;
    T val;
    Map<T> *_hashmap;
    usize _index;
};

template <typename T>
MapIterator<T> MapIterator_create(Arena *arena, Map<T>* hashmap) {
    MapIterator<T> it = {0};
    if (hashmap) {
        it._hashmap = hashmap;
        it._index = 0;
    }
    return it;
}

template <typename T>
bool MapIterator_next(Arena *arena, MapIterator<T>* it) {
    // Loop till we've hit end of entries array.
    Map<T> *hashmap = it->_hashmap;
    while (it->_index < hashmap->border) {
        size_t i = it->_index;
        it->_index++;
        if (hashmap->entries[i].key != NULL) {
            // Found next non-empty item, update iterator key and val.
            MapEntry<T> entry = hashmap->entries[i];
            it->key = entry.key;
            it->val = entry.val;
            return true;
        }
    }
    return false;
}

