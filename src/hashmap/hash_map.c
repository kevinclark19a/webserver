#include "hashmap/hash_map.h"

#include "logging/log_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Computes a (hopefully) very unique hash for the
 * specified key.
 */
static int compute_hash(const char *const key);

//////////////////
//	API	//
//////////////////

void add_mapping(hash_map_t *map, const char *const key, void *const elem) {
	int hashcode = compute_hash(key);

	struct _map_node *list = map->nodes[hashcode];

	while (list != NULL && list->next != NULL) list = list->next;

	if (list == NULL) {
		map->nodes[hashcode] = (struct _map_node *)calloc(1, sizeof(struct _map_node));
		list = map->nodes[hashcode];
	} else {
		list->next = (struct _map_node *)calloc(1, sizeof(struct _map_node));
		list = list->next;
	}

	list->elem = elem;
	list->key = (const char *)key;
}

void *get_mapping(hash_map_t *map, const char *const key) {
	int hashcode = compute_hash(key);

	struct _map_node *list = map->nodes[hashcode];

	if (list == NULL) return NULL;

	while (list != NULL) {
		if (strcmp(list->key, key) == 0)
			return list->elem;

		list = list->next;
	}

	return NULL;
}

bool has_mapping(hash_map_t *map, const char *const key) {
	return get_mapping(map, key) != NULL;
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static int compute_hash(const char *const key) {
	int primes[7] = { 491, 503, 509, 523, 547, 557, 631 };

	int sum = 0;
	for (int i = 0; i < strlen(key); i++) {
		sum += (key[i] * primes[i % 7]);
	}

	return sum % __HASH_TABLE_SIZE;
}
