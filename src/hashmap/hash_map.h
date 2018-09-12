#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdbool.h>

#define __HASH_TABLE_SIZE 277

struct _map_node {
	struct _map_node *next;

	const char *key;
	void *elem;
};

struct _hash_map {
	struct _map_node *nodes[__HASH_TABLE_SIZE];
};

typedef struct _hash_map hash_map_t;

/**
 * Adds the specified mapping from key -> value.
 */
void add_mapping(hash_map_t *map, const char *const key, void *const value);

/**
 * Returns true if a mapping exists from the specified key
 * to some value and false otherwise.
 */
bool has_mapping(hash_map_t *map, const char *const key);

/**
 * Returns the value such that key -> value, if such a value
 * exists, and null otherwise.
 */
void *get_mapping(hash_map_t *map, const char *const key);

#endif	// HASH_MAP_H
