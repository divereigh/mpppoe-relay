#include "config.h"

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>

static struct event_base *base;

void initEvent()
{
	base = event_base_new();
}

void dispatchEvent()
{
	event_base_dispatch(base);
}

struct event *eventSocket(evutil_socket_t fd, event_callback_fn cb, void *data)
{
	struct event *ev;
	if (ev=event_new(base, fd, EV_READ|EV_PERSIST, cb, data)) {
		event_add(ev, NULL);
	}
	return(ev);
}

struct event *newTimer(event_callback_fn cb, void *data)
{
	return(event_new(base, -1, EV_TIMEOUT, cb, data));
}

int startTimer(struct event *ev, int secs)
{
	struct timeval timer;

	timer.tv_sec=secs;
	timer.tv_usec=0;

	return(event_add(ev, &timer));
}

int stopTimer(struct event *ev)
{
	return((ev) ? event_del(ev) : -1);
}
