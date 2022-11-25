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

#ifndef MEMPOOL_H
#define MEMPOOL_H

/**
 * Simple memory pool for char*
 */

#include <stdint.h>

typedef struct mempool mempool;

mempool* mempool_init(uint32_t cap);
void mempool_free(mempool *pool);
char* mempool_alloc(mempool *pool, uint32_t len);
char* mempool_store(mempool *pool, const char *s);

#endif
