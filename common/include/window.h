/*
 * This file is part of the DXX-Rebirth project <http://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */
/*
 *  A 'window' is simply a canvas that can receive events.
 *  It can be anything from a simple message box to the
 *	game screen when playing.
 *
 *  See event.c for event handling code.
 *
 *	-kreator 2009-05-06
 */

#pragma once

#include "gr.h"
#include "console.h"

#ifdef __cplusplus
#include "fwd-window.h"

enum class window_event_result : uint8_t
{
	// Window ignored event.  Bubble up.
	ignored,
	// Window handled event.
	handled,
	close,
};

static const unused_window_userdata_t *const unused_window_userdata = nullptr;

struct embed_window_pointer_t
{
	window *wind;
};

struct ignore_window_pointer_t
{
};

static inline void set_embedded_window_pointer(embed_window_pointer_t *wp, window *w)
{
	wp->wind = w;
}

static inline void set_embedded_window_pointer(ignore_window_pointer_t *, window *) {}

template <typename T1, typename T2 = const void>
static inline window *window_create(grs_canvas *src, int x, int y, int w, int h, window_subfunction<T1> event_callback, T1 *data, T2 *createdata = nullptr)
{
	auto win = window_create(src, x, y, w, h, reinterpret_cast<window_subfunction<void>>(event_callback), static_cast<void *>(data), static_cast<const void *>(createdata));
	set_embedded_window_pointer(data, win);
	return win;
}

template <typename T1, typename T2 = const void>
static inline window *window_create(grs_canvas *src, int x, int y, int w, int h, window_subfunction<const T1> event_callback, const T1 *userdata, T2 *createdata = nullptr)
{
	return window_create(src, x, y, w, h, reinterpret_cast<window_subfunction<void>>(event_callback), static_cast<void *>(const_cast<T1 *>(userdata)), static_cast<const void *>(createdata));
}

static inline window *window_set_visible(window *wind, int visible)
{
	return window_set_visible(*wind, visible);
}
static inline int window_is_visible(window *wind)
{
	return window_is_visible(*wind);
}
static inline void window_set_modal(window *wind, int modal)
{
	window_set_modal(*wind, modal);
}

static inline window_event_result (WINDOW_SEND_EVENT)(window &w, const d_event &event, const char *file, unsigned line, const char *e)
{
	auto &c = window_get_canvas(w);
	con_printf(CON_DEBUG, "%s:%u: sending event %s to window of dimensions %dx%d", file, line, e, c.cv_bitmap.bm_w, c.cv_bitmap.bm_h);
	return window_send_event(w, event);
}

#endif
