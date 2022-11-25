/*
 * This file is part of AKlib, a sturdy data structure C99 library.
 * Copyright (C) 2022 Arnaud Kress
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

#ifndef VECTOR_H
#define VECTOR_H

/*
 * Generic dynamic array
 * Member 'size' is for manual update only
 */

#include <stdint.h>
#include <string.h>

#define DECLARE_VECTOR(type) \
	typedef struct darray { \
		uint32_t capacity; \
		uint32_t size; \
		type *data; \
	} vector_##type; \
 \
	vector_##type* vector_##type##_init(uint32_t start_capacity); \
	void vector_##type##_free(vector_##type *v); \
	void vector_##type##_set(vector_##type *v, uint32_t index, type val);

#define DEFINE_VECTOR(type) \
	vector_##type* vector_##type##_init(uint32_t start_capacity) { \
		vector_##type *v = malloc(sizeof(vector_##type)); \
		v->capacity = start_capacity; \
		v->size = 0; \
		v->data = calloc(v->capacity, sizeof(type)); \
		return v; \
	} \
	void vector_##type##_free(vector_##type *v) { \
		free(v->data); \
		free(v); \
	} \
	void vector_##type##_set(vector_##type *v, uint32_t index, type val) { \
		uint32_t newcap = v->capacity; \
		while (index >= newcap) \
			newcap *= 2; \
		if (newcap > v->capacity) { \
			v->data = realloc(v->data, newcap*sizeof(type)); \
			memset(&(v->data[v->capacity]), 0, (newcap-v->capacity)*sizeof(type)); \
			v->capacity = newcap; \
		} \
		v->data[index] = val; \
	}

#endif
