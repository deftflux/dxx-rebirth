/*
 * This file is part of the DXX-Rebirth project <http://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */
/*
 *
 * Header for joystick functions
 *
 */

#ifndef _JOY_H
#define _JOY_H

#include "pstypes.h"
#include "maths.h"
#include <SDL.h>

#ifdef __cplusplus

struct d_event;

#define MAX_JOYSTICKS				8
#define MAX_AXES_PER_JOYSTICK		128
#define MAX_BUTTONS_PER_JOYSTICK	128
#define MAX_HATS_PER_JOYSTICK		4
#define JOY_MAX_AXES				(MAX_AXES_PER_JOYSTICK * MAX_JOYSTICKS)
#define JOY_MAX_BUTTONS				(MAX_BUTTONS_PER_JOYSTICK * MAX_JOYSTICKS)

struct d_event_joystick_axis_value
{
	unsigned axis;
	int value;
};

extern void joy_init();
extern void joy_close();
const d_event_joystick_axis_value &event_joystick_get_axis(const d_event &event);
extern void joy_flush();
extern int event_joystick_get_button(const d_event &event);
extern void joy_button_handler(SDL_JoyButtonEvent *jbe);
extern void joy_hat_handler(SDL_JoyHatEvent *jhe);
extern int joy_axis_handler(SDL_JoyAxisEvent *jae);

#endif

#endif // _JOY_H
