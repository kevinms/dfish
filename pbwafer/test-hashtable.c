// Copyright (C) 2011  Kevin Smith
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
// MA 02110-1301, USA.

// NOTE: This is not an exhaustive test!!!

// NOTE: 7-23-2011 - passed where COUNT was: 1,2,3,20,31,200,206,207,500,512

#include "stdlib.h"
#include "stdio.h"

#include "hashtable.h"

#define COUNT 512

typedef struct bob_s
{
	int x, y;
} bob_t;

int rv = 0;
bob_t *vtab[COUNT][COUNT] = {NULL};

void init_data()
{
	int x, y;
	
	for (x = 0; x < COUNT; x++)
	{
		for (y = 0; y < COUNT; y++)
		{
			vtab[x][y] = (bob_t *)malloc(sizeof(*vtab));
			vtab[x][y]->x = x;
			vtab[x][y]->y = y;
		}
	}
}

void insert_data(hashtable_t *ht)
{
	int x, y;
	void *key = NULL;

	// Test insert
	for (x = 0; x < COUNT; x++)
	{
		for (y = 0; y < COUNT; y++)
		{
			key = malloc(sizeof(int) * 2);
			*((int *)key) = x;
			*(((int *)key) + 1) = y;
			HT_insert(ht, key, sizeof(int) *2, (void *)vtab[x][y]);
		}
	}
}

void find_data(hashtable_t *ht)
{
	int x, y;
	void *key = NULL;
	bob_t *test;

	// Test find
	key = malloc(sizeof(int) * 2);
	for (x = 0; x < COUNT; x++)
	{
		for (y = 0; y < COUNT; y++)
		{
			*((int *)key) = x;
			*(((int *)key) + 1) = y;
			test = (bob_t *)HT_find(ht, key, sizeof(int) * 2, vtab[x][y]);
			if(!test) {
				//printf("ERROR: did not find key");
				rv++;
				continue;
			}

			if(test->x != x || test->y != y)
				rv++;
		}
	}
}

void test_deletion(hashtable_t *ht, int x, int y)
{
	void *key = NULL;
	bob_t *test;

	key = malloc(sizeof(int) * 2);
	*((int *)key) = x;
	*(((int *)key) + 1) = y;
	test = (bob_t *)HT_delete(ht, key, sizeof(int) * 2, (void *)vtab[x][y]);

	if(!test) {
		printf("ERROR: index out of range");
		exit(4200);
	}

	if(test->x != x || test->y != y)
		rv++;
}

int main()
{
	hashtable_t *ht;
	
	ht = HT_init(COUNT * COUNT * 2, &HT_hash_djb);

	init_data();

	// Test insert all and find all
	insert_data(ht);
	find_data(ht);
	if(rv != 0)
		return 1;
	rv =0;

	// Test insert all twice, delete one, and pass on find all
	HT_flush(ht);
	insert_data(ht);
	insert_data(ht);
	test_deletion(ht, 0, 1);
	find_data(ht);
	if(rv != 0)
		return 2;
	rv = 0;

	// Test insert all, delete one, and fail on find all [will not be first in list with HT_hash_djb]
	HT_flush(ht);
	insert_data(ht);
	test_deletion(ht, 0, 1);
	find_data(ht);
	if(rv != 1)
		return 3;
	rv = 0;

	// Test insert all, delete one, and fail on find all [will be first in list with HT_hash_djb]
	HT_flush(ht);
	insert_data(ht);
	test_deletion(ht, 1, 0);
	find_data(ht);
	if(rv != 1)
		return 4;
	rv = 0;

	// Test flush
	HT_flush(ht);
	insert_data(ht);
	HT_flush(ht);
	find_data(ht);
	if(rv != COUNT*COUNT)
		return 5;
	rv = 0;

	// Test resize
	HT_flush(ht);
	insert_data(ht);
	HT_resize(&ht, COUNT * 2);
	find_data(ht);
	if(rv != 0)
		return 6;
	rv = 0;

	return 0; // Passed all tests
}