#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

struct _queue_node {
	struct _queue_node *prev;
	struct _queue_node *next;

	void *elem;
};

struct _queue {
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	struct _queue_node *head;
};

typedef struct _queue queue_t;

/**
 * Pushes the specified element onto the queue in
 * a threadsafe way.
 */
void push_onto_queue(queue_t *queue, void *elem);

/**
 * Pops the next element from the queue when one
 * becomes available in a threadsafe way.
 */
void *pop_from_queue(queue_t *queue);

#endif	// QUEUE_H
