// Copyright (C) 2011  Kevin Smith
//
// hash algorithms where designed by other developers
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

#include "hashtable.h"

#include "stdlib.h"

// Modified Bernstein
unsigned HT_hash_djb(void *key, int len)
{
	unsigned char *p = (unsigned char *)key;
	unsigned h = 0;
	int i;

	for ( i = 0; i < len; i++ )
		h = 33 * h ^ p[i];

	return h;
}

// Shift-Add-XOR
unsigned HT_hash_sax(void *key, int len)
{
	unsigned char *p = (unsigned char *)key;
	unsigned h = 0;
	int i;

	for ( i = 0; i < len; i++ )
		h ^= ( h << 5 ) + ( h >> 2 ) + p[i];

	return h;
}

// FNV (Fowler/Noll/Vo)
unsigned HT_hash_fnv(void *key, int len)
{
	unsigned char *p = (unsigned char *)key;
	unsigned h = 2166136261;
	int i;

	for ( i = 0; i < len; i++ )
		h = ( h * 16777619 ) ^ p[i];

	return h;
}

// One-at-a-Time
unsigned HT_hash_oat(void *key, int len)
{
	unsigned char *p = (unsigned char *)key;
	unsigned h = 0;
	int i;

	for ( i = 0; i < len; i++ ) {
		h += p[i];
		h += ( h << 10 );
		h ^= ( h >> 6 );
	}

	h += ( h << 3 );
	h ^= ( h >> 11 );
	h += ( h << 15 );

	return h;
}

// ELF
unsigned HT_hash_elf(void *key, int len)
{
	unsigned char *p = (unsigned char *)key;
	unsigned h = 0, g;
	int i;

	for ( i = 0; i < len; i++ ) {
		h = ( h << 4 ) + p[i];
		g = h & 0xf0000000L;

		if ( g != 0 )
			h ^= g >> 24;

		h &= ~g;
	}

	return h;
}

hashtable_t *HT_init(int size, unsigned (*hash)(void *, int), int (*cmp)(void *, void *))
{
	hashtable_t *ht = (hashtable_t *)calloc(1, sizeof(*ht));
	if(!ht)
		return NULL;

	ht->table = (hashnode_t **)calloc(size, sizeof(*(ht->table)));
	ht->hash = hash;
	ht->cmp = cmp;
	ht->size = size;

	return ht;
}

int HT_insert(hashtable_t *ht, void *key, int len, void *value)
{
	hashnode_t *node;
	unsigned m;

	m = ht->hash(key, len) % ht->size;

	node = (hashnode_t *)calloc(1, sizeof(*node));

	if(!node)
		return 0;

	node->key = key;
	node->len = len;
	node->value = value;

	if(ht->table[m])
		node->next = ht->table[m];

	ht->table[m] = node;
	ht->count++;

	return 1;
}

void *HT_delete(hashtable_t *ht, void *key, int len, void *value)
{
	hashnode_t *n, *p;
	unsigned m;

	m = ht->hash(key, len) % ht->size;

	if(!ht->table[m])
		return NULL;

	// Check if it is the first link
	if(ht->table[m] != NULL && ht->cmp(ht->table[m]->value,value) == 0) {
		n = ht->table[m];
		ht->table[m] = ht->table[m]->next;
		value = n->value;
		free(n->key);
		free(n);
		ht->count--;
		return value;
	}

	// Check the other links
	for(n = ht->table[m]->next, p = ht->table[m]; n != NULL; p = n, n = n->next) {
		if(ht->cmp(n->value,value) == 0) {
			p->next = n->next;
			value = n->value;
			free(n->key);
			free(n);
			ht->count--;
			return value;
		}
	}

	return NULL;
}

//TODO: add debug statements to print out the find runtime
void *HT_find(hashtable_t *ht, void *key, int len, void *value)
{
	hashnode_t *n;
	unsigned m;

	m = ht->hash(key, len) % ht->size;

	for(n = ht->table[m]; n != NULL && ht->cmp(n->value,value) != 0; n = n->next)
		;

	if(n)
		return n->value;
	return NULL;
}

void HT_flush(hashtable_t *ht)
{
	hashnode_t *n, *t;
	int i;

	for(i = 0; i < ht->size; i++) {
		for(n = ht->table[i]; n != NULL;) {
			t = n;
			n = n->next;
			free(t->key);
			free(t);
		}
		ht->table[i] = NULL;
	}
	ht->count = 0;
}

int HT_resize(hashtable_t **ht, int size)
{
	hashtable_t *new_ht;
	hashnode_t *n, *t;
	int i;

	new_ht = HT_init(size, (*ht)->hash, (*ht)->cmp);
	if(!new_ht)
		return 0;

	for(i = 0; i < (*ht)->size; i++) {
		for(n = (*ht)->table[i]; n != NULL;) {
			if(!HT_insert(new_ht, n->key, n->len, n->value))
				return 0;
			t = n;
			n = n->next;
			free(t);
		}
	}
	
	*ht = new_ht;
	return 1;
}

void HT_free(hashtable_t *ht)
{
	HT_flush(ht);
	free(ht->table);
	free(ht);
}