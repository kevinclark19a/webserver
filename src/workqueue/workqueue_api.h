#ifndef WORKQUEUE_H
#define WORKQUEUE_H

/**
 * Ensures that the specified task with given parameters will
 * be executed by the thread pool when the next worker becomes
 * available.
 */
void submit_task_to_pool(void (*task)(void *), void *param);

#endif	// WORKQUEUE_H
