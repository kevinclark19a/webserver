#include "logging/log_api.h"
#include "workqueue/workqueue_api.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define __LOG_FILENAME WEBSERVER__WORKING_DIRECTORY "/log"

/**
 * Helper function to initialize the logger. Logging
 * functions should call this function at least once
 * before execution.
 */
static void __initialize_logger(void);

/**
 * A function to be run asyncronously that flushes the
 * specified file every second.
 */
static void flush_log_file(void *fp);

/**
 * Global variable containing information about
 * which log events should be processed.
 */
#ifdef DEBUG_MODE
static const LOG_LEVEL __priority = __LL_INFO;
#else
static LOG_LEVEL __priority = __LL_INFO;
#endif	// DEBUG_MODE

/**
 * Global variable allowing the logging service to
 * write to file.
 */
static FILE *__log_filep;

//////////////////
//	API	//
//////////////////

void Set_log_level(LOG_LEVEL priority) {

	// TODO: We eventually want this thread safe.

	#ifndef DEBUG_MODE
	__priority = priority;
	#endif	// DEBUG_MODE
}

void Log_d(const char *const message) {
	#ifdef DEBUG_MODE

	if (__log_filep == NULL) __initialize_logger();

	fprintf(__log_filep, "%s%s\n", __LOG_TAG_D, message);

	#ifdef ECHO_LOGS
	if (__log_filep != stdout)
		printf("%s%s\n", __LOG_TAG_D, message);
	#endif	// ECHO_LOGS

	#endif	// DEBUG_MODE
}

void Log_i(const char *const message) {
	if (__log_filep == NULL) __initialize_logger();

	if (__priority <= __LL_INFO) {
		fprintf(__log_filep, "%s%s\n", __LOG_TAG_I, message);

		#ifdef ECHO_LOGS
		if (__log_filep != stdout)
			printf("%s%s\n", __LOG_TAG_I, message);
		#endif	// ECHO_LOGS
	}
}

void Log_w(const char *const message) {
	if (__log_filep == NULL) __initialize_logger();

	if (__priority <= __LL_WARN) {
		fprintf(__log_filep, "%s%s\n", __LOG_TAG_W, message);

		#ifdef ECHO_LOGS
		if (__log_filep != stdout)
			printf("%s%s\n", __LOG_TAG_W, message);
		#endif	// ECHO_LOGS
	}
}

void Log_e(const char *const message) {
	if (__log_filep == NULL) __initialize_logger();

	if (__priority <= __LL_ERROR) {
		fprintf(__log_filep, "%s%s\n", __LOG_TAG_E, message);

		#ifdef ECHO_LOGS
		if (__log_filep != stdout)
			printf("%s%s\n", __LOG_TAG_E, message);
		#endif	// ECHO_LOGS
	}
}

//////////////////////////////////
//	HELPER FUNCTIONS	//
//////////////////////////////////


static void __initialize_logger() {
	__log_filep = fopen(__LOG_FILENAME, "a+");

	if (__log_filep == NULL) {
		printf("%sCannot open log file. Defaulting to console logging.\n",
			__LOG_TAG_E);
		__log_filep = stdout;
	} else {
		// Here we only want to submit our flushing job if
		// We are actually planning on writing to a file,
		// and not just stdout.
		submit_task_to_pool(flush_log_file, __log_filep);
	}
}

static void flush_log_file(void *fp) {
	FILE *log_filep = (FILE *)fp;

	while (1) {
		sleep(1);	// Sleep for 1 s
		fflush(log_filep);
	}
}
