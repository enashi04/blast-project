/*
 * This file is part of AKlib, a sturdy C99 data structure library.
 * Copyright (C) 2022 Universite de Strasbourg
 *
 * AKlib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
/*
 * Generic hashmap for C99
 *
 *   DECLARE_DICT(int) // macro to define structure for desired type (here the struct dict_int is created), can be put in header
 *   DEFINE_DICT(int)  // actual code definition
 *
 *   dict_int *m = dict_int_init();
 *   dict_int_set(m, "key", 12);
 *   bool present;      // set to true if key present in map
 *   int v = dict_int_get(m, "key", &present);
 *   dict_int_unset(m, "key");
 *   dict_free(m);
 *
 */

#ifndef DICT_H
#define DICT_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define u32 uint32_t

#define DICT_DEFAULT_CAPACITY 128

#define __is_free(b) (b->flag == 0)
#define __is_allocated(b) (b->flag == 1)
#define __is_deleted(b) (b->flag == 2)
#define __set_allocated(b) b->flag = 1
#define __set_deleted(b) b->flag = 2

#define DECLARE_DICT(type) \
        typedef struct dict_##type dict_##type; \
        \
        dict_##type* dict_##type##_init(); \
        dict_##type* dict_##type##_ninit(u32 start_capacity); \
        void dict_##type##_free(dict_##type *d); \
        u32 dict_##type##_size(dict_##type *d); \
        bool dict_##type##_set(dict_##type *d, const char *key, type value); \
        type dict_##type##_get(dict_##type *d, const char *key, bool *present); \
        bool dict_##type##_exists(dict_##type *d, const char *key); \
        bool dict_##type##_unset(dict_##type *d, const char *key); \
        uint32_t dict_##type##_iter(dict_##type *d, uint32_t index, const char **key, type *value);

#define DEFINE_DICT(type) \
        typedef struct bucket_##type { \
                char *key; \
                type value; \
                char flag; \
        } bucket_##type; \
        \
        struct dict_##type { \
                u32 capacity; \
                u32 size; \
                bucket_##type *buckets; \
        }; \
        \
        static bucket_##type* __dict_##type##_find(dict_##type *h, const char *key, bool *present); \
        static unsigned long __dict_##type##_hash(const char *s); \
        static dict_##type* dict_##type##_straight_ninit(u32 start_capacity); \
        \
        bool dict_##type##_resize(dict_##type *h, u32 new_capacity) { \
                if (new_capacity < h->size) return 0; \
                \
                bucket_##type *old_buckets = h->buckets; \
                h->buckets = calloc(sizeof(bucket_##type), new_capacity); \
                u32 old_capacity = h->capacity; \
                h->capacity = new_capacity; \
                h->size = 0; \
                \
                bucket_##type *b; \
                for (u32 i=0; i<old_capacity; ++i) { \
                        b = &(old_buckets[i]); \
                        if (__is_allocated(b)) { \
                                dict_##type##_set(h, b->key, b->value); \
                                free(b->key); \
                        } \
                } \
                free(old_buckets); \
                return true; \
        } \
        \
        bool dict_##type##_extend(dict_##type *old) { \
                return dict_##type##_resize(old, old->capacity*2); \
        } \
        \
        dict_##type* dict_##type##_init() { \
                return dict_##type##_ninit(DICT_DEFAULT_CAPACITY); \
        } \
        \
        dict_##type* dict_##type##_ninit(u32 start_capacity) { \
                u32 actual_capacity = 16; \
                while (actual_capacity < start_capacity) \
                        actual_capacity *= 2; \
                return dict_##type##_straight_ninit(actual_capacity); \
        } \
        \
        static dict_##type* dict_##type##_straight_ninit(u32 start_capacity) { \
                dict_##type *h = malloc(sizeof(dict_##type)); \
                h->capacity = start_capacity*2; \
                h->size = 0; \
                h->buckets = calloc(sizeof(bucket_##type), h->capacity); \
                return h; \
        } \
        \
        void dict_##type##_free(dict_##type *h) { \
                bucket_##type *b; \
                for (u32 i=0; i<h->capacity; ++i) { \
                        b = &(h->buckets[i]); \
                        if (__is_allocated(b)) \
                                free(b->key); \
                } \
                free(h->buckets); \
                free(h); \
        } \
        \
        u32 dict_##type##_size(dict_##type *h) { \
                return h->size; \
        } \
        \
        uint32_t dict_##type##_iter(dict_##type *h, uint32_t index, const char **key, type* value) { \
                bucket_##type *b; \
                *key = NULL; \
                for (u32 j=index; j<h->capacity; ++j) { \
                        b = &(h->buckets[j]); \
                        if (__is_allocated(b)) { \
                                *key = b->key; \
                                *value = b->value; \
                                return j+1; \
                        } \
                } \
                return 0; \
        } \
        \
        bool dict_##type##_set(dict_##type *h, const char *key, type value) { \
                bool present; \
                if (h->size+1 * 2 > h->capacity) \
                        dict_##type##_extend(h); \
                \
                bucket_##type *b = __dict_##type##_find(h, key, &present); \
                __set_allocated(b); \
                b->value = value; \
                if (!present) { \
                        b->key = malloc(strlen(key)+1); \
                        strcpy(b->key, key); \
                        h->size++; \
                } \
                return present; \
        } \
        \
        type dict_##type##_get(dict_##type *h, const char *key, bool *present) { \
                bucket_##type *b = __dict_##type##_find(h, key, present); \
                return b->value; \
        } \
        \
        bool dict_##type##_exists(dict_##type *h, const char *key) { \
                bool present; \
                __dict_##type##_find(h, key, &present); \
                return present; \
        } \
        \
        bool dict_##type##_unset(dict_##type *h, const char *key) { \
                bool present; \
                bucket_##type *b = __dict_##type##_find(h, key, &present); \
                if (!present) return false; \
                __set_deleted(b); \
                free(b->key); \
                h->size--; \
                return true; \
        } \
        \
        static bucket_##type* __dict_##type##_find(dict_##type *h, const char *key, bool *present) { \
                u32 i = 0; \
                u32 probe = __dict_##type##_hash(key) % h->capacity; \
                bucket_##type *b; \
                \
                do { \
                        probe = (probe + i*(i+1)/2) % h->capacity; \
                        b = &(h->buckets[probe]); \
                        \
                        if (__is_free(b)) { \
                                *present = false; \
                                return b; \
                        } \
                        if (__is_allocated(b) && strcmp(b->key, key) == 0) { \
                                *present = true; \
                                return b; \
                        } \
                        ++i; \
                } while (1); \
        } \
        \
        static inline unsigned long __dict_##type##_hash(const char *s) { \
                unsigned long hash = 5381; \
                unsigned int i = 0; \
                char c; \
                \
                while ((c = s[i++])) \
                        hash = ((hash << 5) + hash) + c; \
                return hash; \
        }

#endif