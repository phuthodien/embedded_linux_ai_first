/*
 * edr_agent.h — Header for EDR agent functions (for testing)
 */

#ifndef _EDR_AGENT_H
#define _EDR_AGENT_H

#include <linux/types.h>
#include "../common/edr_events.h"

/*
 * Convert event type to string.
 */
const char *event_type_str(__u32 type);

/*
 * Format timestamp from nanoseconds to human-readable format.
 */
void format_timestamp(__u64 ts_ns, char *buf, size_t len);

/*
 * Format event to string buffer.
 * Returns number of characters written (excluding null terminator).
 */
int format_event(const struct edr_event *evt, char *buf, size_t len);

#endif /* _EDR_AGENT_H */
