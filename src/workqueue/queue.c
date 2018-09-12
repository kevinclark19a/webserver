#include "workqueue/queue.h"

#include "logging/log_api.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Returns true if there is no elements in the
 * queue and false otherwise.
 */
static bool is_empty(queue_t *queue);

//////////////////
//	API	//
//////////////////

void push_onto_queue(queue_t *queue, void *elem) {
	struct _queue_node *new_node = (struct _queue_node *)
		calloc(1, sizeof(struct _queue_node));
	new_node->elem = elem;

	pthread_mutex_lock(&queue->mutex);

	struct _queue_node *old_head = queue->head;

	if (old_head == NULL) {
		new_node->prev = new_node;
		new_node->next = new_node;
	} else {
		new_node->prev = old_head->prev;
		old_head->prev->next = new_node;

		new_node->next = old_head;
		old_head->prev = new_node;
	}

	queue->head = new_node;

	pthread_mutex_unlock(&queue->mutex);

	pthread_cond_signal(&queue->cond);
}

void *pop_from_queue(queue_t *queue) {

	pthread_mutex_lock(&queue->mutex);

	while (is_empty(queue)) {
		pthread_cond_wait(&queue->cond, &queue->mutex);
	}

	struct _queue_node *old_head = queue->head;
	if (old_head == old_head->next) {
		queue->head = NULL;
	} else {
		queue->head = old_head->next;
		old_head->next->prev = queue->head;

		queue->head->prev = old_head->prev;
		old_head->prev->next = queue->head;
	}

	void *elem = old_head->elem;
	free(old_head);

	pthread_mutex_unlock(&queue->mutex);

	return elem;
}


//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

static bool is_empty(queue_t *queue) {
	return queue->head == NULL;
}
