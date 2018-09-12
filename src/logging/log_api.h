#ifndef LOG_H
#define LOG_H

#define __LOG_TAG_(level) WEBSERVER__PROGRAM_NAME " v. " WEBSERVER__VERSION " <" level ">\t"

#define __LOG_TAG_D __LOG_TAG_("debug")
#define __LOG_TAG_I __LOG_TAG_("info")
#define __LOG_TAG_W __LOG_TAG_("warn")
#define __LOG_TAG_E __LOG_TAG_("error")

enum _log_level {
	__LL_INFO = 1,
	__LL_WARN,
	__LL_ERROR
};

typedef enum _log_level LOG_LEVEL;

/**
 * Sets the global logging level to the specified
 * priority. Log events below the specified priority
 * will not be processed after the completion of this
 * call.
 *
 * Accepts three values:
 *
 *	-	__LL_INFO : The lowest log level accepted. If
 *		__LL_INFO is set, all log events will be
 *		processed except for debug logs.
 *	-	__LL_WARN : The middle log level. If __LL_WARN
 *		is set, all info and debug log events will be
 *		ignored, but warn and error log events will be
 *		processed.
 *	-	__LL_ERROR : The highest log level. If
 *		__LL_ERROR is set, only error log events will
 *		be processed.
 *
 * In debug mode, this method is a no-op: the log
 * level is stuck at info in that case.
 */
void Set_log_level(LOG_LEVEL priority);

/**
 * Logs the specified message at debug level.
 */
void Log_d(const char *const message);

/**
 * Logs the specified message at info level.
 */
void Log_i(const char *const message);

/**
 * Logs the specified message at warn level.
 */
void Log_w(const char *const message);

/**
 * Logs the specified message at error level.
 */
void Log_e(const char *const message);

#endif	// LOG_H
