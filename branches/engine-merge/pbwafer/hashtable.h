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

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

typedef struct hashnode_s
{
	void *key;
	int len;
	void *value;
	struct hashnode_s *next;
} hashnode_t;

typedef struct hashtable_s
{
	hashnode_t **table;
	int size;
	int count;
	unsigned (*hash)(void *, int);
	int (*cmp)(void *, void *);
} hashtable_t;

unsigned HT_hash_djb(void *key, int len); // Modified Bernstein
unsigned HT_hash_sax(void *key, int len); // Shift-Add-XOR
unsigned HT_hash_fnv(void *key, int len); // FNV (Fowler/Noll/Vo)
unsigned HT_hash_oat(void *key, int len); // One-at-a-Time
unsigned HT_hash_elf(void *key, int len); // ELF

hashtable_t *HT_init(int size, unsigned (*hash)(void *, int), int (*cmp)(void *, void *));

int HT_insert(hashtable_t *ht, void *key, int len, void *value);
void *HT_delete(hashtable_t *ht, void *key, int len, void *value);
void *HT_find(hashtable_t *ht, void *key, int len, void *value);

void HT_flush(hashtable_t *ht);
void HT_free(hashtable_t *ht);
int HT_resize(hashtable_t **ht, int size);
void HT_free(hashtable_t *ht);

#endif __HASHTABLE_H