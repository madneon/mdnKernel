/*
	MIT License

	Copyright © 2017 by madneon
	http://madneon.com
*/

#ifndef mdnKernel_h
#define mdnKernel_h

#include <Arduino.h>

class mdnKernel {
	public:
		struct node {
			unsigned long interval;
			unsigned long timeout;
			void (*callback)();
			node *previous, *next;
			bool running;
		};

		mdnKernel(); // constructor

		unsigned long tick_current = 0; // current tick
		unsigned long tick_last = 0; // last tick
		unsigned long tick_interval = 0; // current interval between ticks

		void tick(); // in loop() handler

		node* loop(unsigned long _interval, void (*_callback)()); // register new loop function
		node* timer(unsigned long _interval, void (*_callback)()); // register new timer function

		void stop(node *target);
		void start(node *target);

	private:

		node *_loop_first, *_loop_last, *_timer_first, *_timer_last = NULL;

		void _timer_delete(node *timer); // remove timer from list
};

#endif
