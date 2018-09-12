#include "workqueue/workqueue_api.h"

#include "workqueue/queue.h"
#include "logging/log_api.h"

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>

struct _task_entry {
	void (*task)(void *);
	void *param;
};

typedef struct _task_entry task_entry_t;

/**
 * TODO: Documentation
 */
void initialize_thread_pool(void);

/**
 * TODO: Documentation
 */
void *do_tasks(void *unused);

static queue_t *work_queue;

static bool __initialized = false;

//////////////////
//	API	//
//////////////////

void submit_task_to_pool(void (*task)(void *), void *param) {
	if (!__initialized) initialize_thread_pool();

	task_entry_t *new_entry = (task_entry_t *)calloc(1, sizeof(task_entry_t));

	new_entry->task = task;
	new_entry->param = param;

	push_onto_queue( work_queue, (void *)new_entry );
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////

void initialize_thread_pool() {
	work_queue = (queue_t *const)calloc(1, sizeof(queue_t));

	pthread_t thread_ids[WEBSERVER__N_WORKER_THREADS];
	for (int i = 0; i < WEBSERVER__N_WORKER_THREADS; i++) {
		pthread_create(&thread_ids[i], NULL, do_tasks, NULL);
	}

	__initialized = true;
}

void *do_tasks(void *unused) {
	while (1) {
		task_entry_t *next_entry = (task_entry_t *)pop_from_queue(work_queue);
		next_entry->task(next_entry->param);
	}
}
