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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mempool.h"

#define u32 uint32_t

struct mempool {
	u32 block_size;
	u32 cursize;
	u32 nblocks;
	char **blocks;
	char *curblock;
};

mempool* mempool_init(u32 cap) {
	mempool *pool = malloc(sizeof(mempool));
	pool->block_size = cap;
	pool->nblocks = 1;
	pool->blocks = malloc(sizeof(char*));
	pool->blocks[0] = malloc(cap);
	pool->curblock = pool->blocks[0];
	pool->cursize = 0;
	return pool;
}

void mempool_free(mempool *pool) {
	for (u32 i=0; i<pool->nblocks; ++i)
		free(pool->blocks[i]);
	free(pool->blocks);
	free(pool);
}

static void check_size(mempool *pool, u32 len) {
	if (pool->cursize+len > pool->block_size) {
		pool->nblocks++;
		pool->blocks = realloc(pool->blocks, pool->nblocks*sizeof(char*));
		pool->blocks[pool->nblocks-1] = malloc(pool->block_size);
		pool->curblock = pool->blocks[pool->nblocks-1];
		pool->cursize = 0;
	}
}

char* mempool_alloc(mempool *pool, u32 len) {
	check_size(pool, len);
	char *res = pool->curblock + pool->cursize;
	pool->cursize += len;
	return res;
}

char* mempool_store(mempool *pool, const char *s) {
	char *stored = mempool_alloc(pool, strlen(s)+1);
	sprintf(stored, "%s", s);
	return stored;
}
