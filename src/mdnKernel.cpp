/*
	MIT License

	Copyright © 2017 by madneon
	http://madneon.com
*/

#include <mdnKernel.h>
#include <avr/power.h>

mdnKernel::mdnKernel() {
}

void mdnKernel::tick() {
	// calculate tick
	tick_last = tick_current;
	tick_current = millis();
	tick_interval = tick_current - tick_last;

	// lowest timeout
	unsigned long _min_timeout = -1;

	// process loop list
	node *i = _loop_first;
	while (i != NULL) {
		if (i->timeout <= tick_interval) {
			i->callback();
			i->timeout = i->interval;
		} else {
			i->timeout -= tick_interval;
		}

		// calculate lowest timeout
		if (i->timeout < _min_timeout) {
			_min_timeout = i->timeout;
		}

		// next item in list
		i = i->next;
	}

	// process timer list
	i = _timer_first;
	while (i != NULL) {
		if (i->timeout <= tick_interval) {
			i->callback();

			// tmp pointer
			node *j = i;

			// next item in list
			i = i->next;

			_timer_delete(j);
		} else {
			i->timeout -= tick_interval;

			// calculate lowest timeout
			if (i->timeout < _min_timeout) {
				_min_timeout = i->timeout;
			}

			// next item in list
			i = i->next;
		}
	}

	// loop slowdown
	if (_min_timeout > 10) {
		delay(_min_timeout - 10);
	}
}

void mdnKernel::loop(unsigned long _interval, void (*_callback)()) {
	// create new item
	node *_item = new node;
	_item->interval = _interval;
	_item->timeout = _interval;
	_item->callback = _callback;
	_item->next = NULL;

	// add item to list
	if (_loop_first == NULL) {
		_item->previous = NULL;
		_loop_first = _item;
		_loop_last = _item;
	} else {
		_item->previous =_loop_last;
		_loop_last->next = _item;
		_loop_last = _item;
	}
}

void mdnKernel::timer(unsigned long _interval, void (*_callback)()) {
	// create new item
	node *_item = new node;
	_item->interval = _interval;
	_item->timeout = _interval;
	_item->callback = _callback;
	_item->next = NULL;

	// add item to list
	if (_timer_first == NULL) {
		_item->previous = NULL;
		_timer_first = _item;
		_timer_last = _item;
	} else {
		_item->previous =_timer_last;
		_timer_last->next = _item;
		_timer_last = _item;
	}
}

void mdnKernel::_timer_delete(node *timer) {
	// cleanup, hopefully
	if (timer->next != NULL) {
		node *n = timer->next;
		n->previous = timer->previous;
	} else {
		// it was last node
		_timer_last = timer->previous;
	}

	if (timer->previous != NULL) {
		node *n = timer->previous;
		n->next = timer->next;
	} else {
		// it was first node
		_timer_first = timer->next;
	}

	timer->next = NULL;
	timer->previous = NULL;
	delete timer;
}
