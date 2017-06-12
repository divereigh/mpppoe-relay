#ifndef __EVENT_H
#define __EVENT_H
#include "config.h"

#include <event2/util.h>
#include <event2/event.h>

void initEvent();
void dispatchEvent();
struct event *eventSocket(evutil_socket_t fd, event_callback_fn cb, void *data);
struct event *newTimer(event_callback_fn cb, void *data);
int startTimer(struct event *ev, int secs);
int stopTimer(struct event *ev);

#endif
