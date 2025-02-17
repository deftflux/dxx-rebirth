/*
 * This file is part of the DXX-Rebirth project <http://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */

#pragma once

struct d_event;
struct d_create_event;
struct d_change_event;
struct d_select_event;

#ifdef __cplusplus

enum event_type : unsigned;

int event_init();

// Sends input events to event handlers
void event_poll();
void event_flush();

// Set and call the default event handler
int call_default_handler(const d_event &event);

// Send an event to the front window as first priority, then to the windows behind if it's not modal (editor), then the default handler
void event_send(const d_event &event);

// Sends input, idle and draw events to event handlers
void event_process();

void event_enable_focus();
void event_disable_focus();
static inline void event_toggle_focus(int activate_focus)
{
	if (activate_focus)
		event_enable_focus();
	else
		event_disable_focus();
}

// See how long we were idle for
void event_reset_idle_seconds();

#endif
